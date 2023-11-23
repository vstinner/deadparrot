#!/usr/bin/env python3
import os.path
try:
    from setuptools import setup, Extension
except ImportError:
    from distutils.core import setup, Extension
from utils import (
    MSVC, TEST_DIR, DEBUG,
    LIBPARROT, LIBPARROT_LIBDIR, LIBPARROT_INCDIR)


if not MSVC:
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
    if DEBUG:
        CFLAGS.extend(("-Og", "-ggdb"))
else:
    # Windows
    CFLAGS = []


def main():
    sources = [os.path.join(TEST_DIR, 'test_deadparrot_cext.c')]
    cflags = list(CFLAGS)
    if not MSVC:
        cflags.extend(('-I', LIBPARROT_INCDIR))
        ldflags = ["-L", LIBPARROT_LIBDIR]
    else:
        cflags.extend(('/I', LIBPARROT_INCDIR))
        ldflags = ["/LIBPATH:%s" % LIBPARROT_LIBDIR]
    ext = Extension(
        'test_deadparrot_cext',
        sources=sources,
        libraries=[LIBPARROT],
        extra_compile_args=cflags,
        extra_link_args=ldflags)
    setup(name="test_deadparrot_cext",
          ext_modules=[ext])


if __name__ == "__main__":
    main()
