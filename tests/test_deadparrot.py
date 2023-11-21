#!/usr/bin/python3
from __future__ import absolute_import
from __future__ import print_function
import argparse
import gc
import os.path
import shutil
import subprocess
import sys
import warnings
try:
    import faulthandler
except ImportError:
    # Python 2
    faulthandler = None

# test.utils
from utils import run_command, MS_WINDOWS, SRC_DIR, TEST_DIR, LIBPARROT_LIBDIR


def display_title(title):
    ver = sys.version_info
    title = "Python %s.%s: %s" % (ver.major, ver.minor, title)

    print(title)
    print("=" * len(title))
    print()
    sys.stdout.flush()


def rmtree(path, verbose):
    if os.path.exists("build"):
        shutil.rmtree("build")


def install_setuptools(verbose):
    cmd = [sys.executable, "-c", "import setuptools"]
    exitcode = run_command(cmd, verbose=False, check=False)
    if not exitcode:
        # setuptools is already installed
        return

    cmd = [sys.executable, "-m", "ensurepip", "--user"]
    run_command(cmd, verbose)

    cmd = [sys.executable, "-m", "pip", "install", "setuptools"]
    run_command(cmd, verbose)


def build_test_cext(module_name, verbose):
    if verbose:
        display_title("Build %s extensions" % module_name)
    os.chdir(TEST_DIR)

    rmtree("build", verbose)
    cmd = [sys.executable, "setup.py", "build"]
    run_command(cmd, verbose)

    names = [name for name in os.listdir('build')
             if not name.startswith('temp.')]
    if len(names) != 1:
        print("Fail to find build directory")
        sys.exit(1)
    return os.path.abspath(os.path.join('build', names[0]))


def import_tests(module_name):
    return __import__(module_name)


def _run_tests(tests, verbose):
    for name, test_func in tests:
        if verbose:
            print("%s()" % name)
            sys.stdout.flush()
        test_func()


_HAS_CLEAR_TYPE_CACHE = hasattr(sys, '_clear_type_cache')

def _refleak_cleanup():
    if _HAS_CLEAR_TYPE_CACHE:
        sys._clear_type_cache()
    gc.collect()


def _check_refleak(test_func, verbose):
    nrun = 6
    for i in range(1, nrun + 1):
        if verbose:
            if i > 1:
                print()
            print("Run %s/%s:" % (i, nrun))
            sys.stdout.flush()

        init_refcnt = sys.gettotalrefcount()
        test_func()
        _refleak_cleanup()
        diff = sys.gettotalrefcount() - init_refcnt

        if i > 3 and diff:
            raise AssertionError("refcnt leak, diff: %s" % diff)


def python_version():
    ver = sys.version_info
    build = 'debug' if hasattr(sys, 'gettotalrefcount') else 'release'
    if hasattr(sys, 'implementation'):
        python_impl = sys.implementation.name
        if python_impl == 'cpython':
            python_impl = 'CPython'
        elif python_impl == 'pypy':
            python_impl = 'PyPy'
    else:
        if "PyPy" in sys.version:
            python_impl = "PyPy"
        else:
            python_impl = 'Python'
    return "%s %s.%s (%s build)" % (python_impl, ver.major, ver.minor, build)


def run_tests(module_name, verbose):
    if verbose:
        title = "Test %s" % module_name
        display_title(title)

    if faulthandler is not None:
        faulthandler.enable()

    testmod = import_tests(module_name)

    check_refleak = hasattr(sys, 'gettotalrefcount')

    tests = [(name, getattr(testmod, name))
             for name in dir(testmod)
             if name.startswith("test")]

    def test_func():
        _run_tests(tests, verbose)

    # Ignore DeprecationWarning
    warnings.simplefilter("ignore", DeprecationWarning)

    if check_refleak:
        _check_refleak(test_func, verbose)
    else:
        test_func()

    if verbose:
        print()

    msg = "%s tests succeeded!" % len(tests)
    msg = "%s: %s" % (python_version(), msg)
    if check_refleak:
        msg = "%s (no reference leak detected)" % msg
    print(msg)


def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument('-v', '--verbose', action="store_true",
                        help='Verbose mode')
    parser.add_argument('--dont-build', dest="build", action="store_false",
                        help="Don't build (internal flag, don't use it)")
    parser.add_argument('--deadparrot-libdir')
    parser.add_argument('--cext-dir')
    return parser.parse_args()


def build_libdeadparrot(verbose):
    if verbose:
        display_title("Build deadparrot library")
    os.chdir(SRC_DIR)

    build_dir = os.path.abspath("build")
    #config = "Debug"
    config = "Release"

    rmtree(build_dir, verbose)

    # Configure
    ver = sys.version_info
    Python_VERSION = "%s.%s" % (ver.major, ver.minor)
    cmd = [
        "cmake",
        "-B", build_dir,
        "-D", "CMAKE_BUILD_TYPE=%s" % config,
        "-D", "Python_VERSION=%s" % Python_VERSION,
    ]
    run_command(cmd, verbose)

    # Build
    cmd = ["cmake", "--build", build_dir, "--config", config]
    run_command(cmd, verbose)

    return LIBPARROT_LIBDIR


def main():
    args = parse_args()
    verbose = args.verbose
    build = args.build

    module_name = "test_deadparrot_cext"

    if build:
        script = os.path.abspath(__file__)

        if sys.version_info >= (3, 12):
            install_setuptools(verbose)

        library_path = build_libdeadparrot(verbose)
        if verbose:
            print()

        build_dir = build_test_cext(module_name, verbose)
        if verbose:
            print()

        env = dict(os.environ)
        if not MS_WINDOWS:
            env['LD_LIBRARY_PATH'] = library_path
        cmd = [
            sys.executable, script,
            "--dont-build",
            "--deadparrot-libdir", library_path,
            "--cext-dir", build_dir]
        if args.verbose:
            cmd.append('--verbose')
        run_command(cmd, verbose=True, env=env)
    else:
        if MS_WINDOWS:
            os.add_dll_directory(args.deadparrot_libdir)
        sys.path.append(args.cext_dir)
        run_tests(module_name, verbose)


if __name__ == "__main__":
    main()
