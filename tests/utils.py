import os.path
import shlex
import subprocess
import sys
try:
    from shlex import quote as shlex_quote
except ImportError:
    # Python 2.7
    from pipes import quote as shlex_quote


DEBUG = False


MS_WINDOWS = (sys.platform == 'win32')
MSVC = MS_WINDOWS
CMAKE_CONFIG = 'Debug' if DEBUG else 'Release'

TEST_DIR = os.path.abspath(os.path.dirname(__file__))
SRC_DIR = os.path.normpath(os.path.join(TEST_DIR , '..', 'src'))
LIBPARROT = "deadparrot"
if not MSVC:
    LIBPARROT_LIBDIR = os.path.join(SRC_DIR, 'build')
else:
    # MSVC
    LIBPARROT_LIBDIR = os.path.join(SRC_DIR, 'build', CMAKE_CONFIG)
LIBPARROT_INCDIR = SRC_DIR


def command_stdout(cmd, **kw):
    kw['stdout'] = subprocess.PIPE
    kw['stderr'] = stderr=subprocess.STDOUT
    kw['universal_newlines'] = True
    kw['shell'] = False
    if hasattr(subprocess, 'run'):
        proc = subprocess.run(cmd, **kw)
        return (proc.returncode, proc.stdout)
    else:
        proc = subprocess.Popen(cmd, **kw)
        try:
            stdout = proc.communicate()[0]
        except:
            proc.kill()
            proc.wait()
            raise
        return (proc.returncode, stdout)


def run_command(cmd, verbose=False, check=True, env=None):
    if not verbose:
        exitcode, stdout = command_stdout(cmd, env=env)
        if check and exitcode:
            print(stdout.rstrip())
            sys.exit(exitcode)
        return exitcode

    print("Run %s" % ' '.join(shlex_quote(arg) for arg in cmd))
    if hasattr(subprocess, 'run'):
        proc = subprocess.run(cmd, env=env)
    else:
        proc = subprocess.Popen(cmd, env=env)
        try:
            proc.wait()
        except:
            proc.kill()
            proc.wait()
            raise

    exitcode = proc.returncode
    if check and exitcode:
        sys.exit(exitcode)
    return exitcode
