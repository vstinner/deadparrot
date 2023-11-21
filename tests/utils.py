import os.path
import subprocess
import sys


MS_WINDOWS = (sys.platform == 'win32')
MSVC = MS_WINDOWS
CMAKE_CONFIG = 'Release'

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


def run_command(cmd, verbose=False, check=True):
    if not verbose:
        exitcode, stdout = command_stdout(cmd)
        if check and exitcode:
            print(stdout.rstrip())
            sys.exit(exitcode)
        return exitcode

    if hasattr(subprocess, 'run'):
        proc = subprocess.run(cmd)
    else:
        proc = subprocess.Popen(cmd)
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
