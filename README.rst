++++++++++
deadparrot
++++++++++

Provide removed C API functions to new Python versions.

Provided functions:

* PyEval_CallObject()
* PyEval_CallObjectWithKeywords()
* PyEval_InitThreads()
* PyFPE_END_PROTECT()
* PyFPE_START_PROTECT()
* PyUnicode_GetMax()
* PyUnicode_InternImmortal(): Python >= 3.0
* _PyInterpreterState_Get(): Python >= 3.8

Support Python 2.7 and Python 3.6-3.13. PyPy is not supported.

This project is distributed under the `Zero Clause BSD (0BSD) license
<https://opensource.org/licenses/0BSD>`_ and is covered by the `PSF Code of
Conduct <https://www.python.org/psf/codeofconduct/>`_.

Run tests on all available Python versions::

    python3 runtests.py

cmake 3.5 or newer is required to build libparrot. Build command::

    cd src
    cmake -B build/ -D CMAKE_BUILD_TYPE=Release
    cmake --build build --config Release

Change the destination to install to a specific directory::

    cmake -B build/ -D CMAKE_BUILD_TYPE=Release -D CMAKE_INSTALL_PREFIX=/path/to/custom/prefix
