import subprocess
import sys


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


def run_command(cmd, verbose=False):
    if not verbose:
        exitcode, stdout = command_stdout(cmd)
        if exitcode:
            print(stdout.rstrip())
            sys.exit(exitcode)
        return

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
    if exitcode:
        sys.exit(exitcode)
