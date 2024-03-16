#!/usr/bin/env python3
"""
Run the test suite on multiple Python versions.

Usage::

    python3 runtests.py
    python3 runtests.py --verbose
    python3 runtests.py --current --verbose
"""
from __future__ import absolute_import
from __future__ import print_function
import argparse
import os.path
import shutil
import subprocess
import sys
try:
    from shutil import which
except ImportError:
    # Python 2
    from distutils.spawn import find_executable as which

from tests.utils import run_command


SRC_DIR = os.path.dirname(__file__)
TEST_SCRIPT = os.path.join(SRC_DIR, "tests", "test_deadparrot.py")

PYTHONS = (
    "python3-debug",
    "python3",
    "python2.7",
    "python3.4",
    "python3.5",
    "python3.6",
    "python3.7",
    "python3.8",
    "python3.9",
    "python3.10",
    "python3.11",
    "python3.12",
    "python3.13",
)


def run_tests_exe(executable, verbose, tested):
    tested_key = os.path.realpath(executable)
    if tested_key in tested:
        return

    # Don't use realpath() for the executed command to support virtual
    # environments
    cmd = [executable, "-u", TEST_SCRIPT]
    if verbose:
        cmd.append("--verbose")
    run_command(cmd, True)
    tested.add(tested_key)


def run_tests(python, verbose, tested):
    executable = which(python)
    if not executable:
        print("Ignore missing Python executable: %s" % python)
        return
    run_tests_exe(executable, verbose, tested)


def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument('-v', '--verbose', action="store_true",
                        help='Verbose mode')
    parser.add_argument('-c', '--current', action="store_true",
                        help="Only test the current Python executable "
                             "(don't test multiple Python versions)")
    return parser.parse_args()


def main():
    args = parse_args()

    path = os.path.join(SRC_DIR, 'build')
    if os.path.exists(path):
        shutil.rmtree(path)

    tested = set()
    if not args.current:
        for python in PYTHONS:
            run_tests(python, args.verbose, tested)
        run_tests_exe(sys.executable, args.verbose, tested)

        print()
        print("Tested: %s Python executables" % len(tested))
    else:
        run_tests_exe(sys.executable, args.verbose, tested)


if __name__ == "__main__":
    main()
