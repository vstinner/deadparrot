#!/usr/bin/env python3
import os.path
try:
    from setuptools import setup, Extension
except ImportError:
    from distutils.core import setup, Extension


LIBRARY = "deadparrot"
TEST_DIR = os.path.dirname(__file__)
SRC_DIR = os.path.normpath(os.path.join(TEST_DIR , '..', 'src'))
LIBPARROT_LIBDIR = os.path.join(SRC_DIR, 'build')
LIBPARROT_INCDIR = SRC_DIR
CFLAGS = [
    # Treat warnings as error
    '-Werror',
    # Enable all warnings
    '-Wall', '-Wextra',
    # Extra warnings
    '-Wconversion',
    # /usr/lib64/pypy3.7/include/pyport.h:68:20: error: redefinition of typedef
    # 'Py_hash_t' is a C11 feature
    "-Wno-typedef-redefinition",
    # Use C99
    '-std=c99',
]


def main():
    sources = [os.path.join(TEST_DIR, 'test_deadparrot_cext.c')]
    cflags = list(CFLAGS)
    cflags.extend(('-I', LIBPARROT_INCDIR))
    ldflags = ["-L", LIBPARROT_LIBDIR]
    ext = Extension(
        'test_deadparrot_cext',
        sources=sources,
        libraries=[LIBRARY],
        extra_compile_args=cflags,
        extra_link_args=ldflags)
    setup(name="test_deadparrot_cext",
          ext_modules=[ext])


if __name__ == "__main__":
    main()
