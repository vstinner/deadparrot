++++++++++
deadparrot
++++++++++

deadparrot library
==================

The deadparrot library provides old removed C API functions to new Python
versions.

Support Python 2.7 and Python 3.6-3.13. PyPy is not supported.

This project is distributed under the `Zero Clause BSD (0BSD) license
<https://opensource.org/licenses/0BSD>`_ and is covered by the `PSF Code of
Conduct <https://www.python.org/psf/codeofconduct/>`_.

See also the `pythoncapi-compat project
<https://pythoncapi-compat.readthedocs.io/>`_: header file providing new
functions to old Python versions.


Functions
=========

Functions provided by the deadparrot library.

Call
----

* ``PyCFunction_Call()``
* ``PyEval_CallFunction()``
* ``PyEval_CallMethod()``
* ``PyEval_CallObject()``
* ``PyEval_CallObjectWithKeywords()``
* ``PyObject_CallNoArgs()``
* ``_PyObject_FastCall()``: Python >= 3.6

PyFrame
-------

* ``PyFrame_GetBack()``
* ``PyFrame_GetBuiltins()``
* ``PyFrame_GetCode()``
* ``PyFrame_GetGlobals()``
* ``PyFrame_GetLasti()``
* ``PyFrame_GetLocals()``
* ``PyFrame_GetVar()``
* ``PyFrame_GetVarString()``

PyInterpreterState
------------------

* ``PyInterpreterState_Get()``
* ``_PyInterpreterState_Get()``

PyThreadState
-------------

* ``PyThreadState_EnterTracing()``
* ``PyThreadState_GetFrame()``
* ``PyThreadState_GetID()``
* ``PyThreadState_GetInterpreter()``
* ``PyThreadState_LeaveTracing()``

PyObject
--------

* ``Py_NewRef()``
* ``Py_SET_REFCNT()``
* ``Py_SET_SIZE()``
* ``Py_SET_TYPE()``
* ``Py_XNewRef()``

PyUnicode
---------

* ``PyUnicode_GetMax()``
* ``PyUnicode_InternImmortal()``: Python >= 3.0

Misc
----

* ``PyEval_InitThreads()``
* ``PyFPE_END_PROTECT()``
* ``PyFPE_START_PROTECT()``

Tests
=====

Run tests on all available Python versions::

    python3 runtests.py


Build
=====

Build dependencies:

* cmake 3.5 or newer is required to build libparrot
* setuptools is needed by tests

Build command::

    cd src
    cmake -B build/ -D CMAKE_BUILD_TYPE=Release
    cmake --build build --config Release

Change the destination to install to a specific directory::

    cmake -B build/ -D CMAKE_BUILD_TYPE=Release -D CMAKE_INSTALL_PREFIX=/custom/prefix


Unsupported removed C API functions
===================================

Removed in Python 3.13
----------------------

* PyEval_AcquireLock()
* PyEval_ReleaseLock()
* PyEval_ThreadsInitialized()
* PyObject_AsCharBuffer()
* PyObject_AsReadBuffer()
* PyObject_AsWriteBuffer()
* PyObject_CheckReadBuffer()
* PySys_AddWarnOption()
* PySys_AddWarnOptionUnicode()
* PySys_AddXOption()
* PySys_HasWarnOptions()
* PySys_SetArgv()
* PySys_SetArgvEx()
* PySys_SetPath()
* Py_SetPath()
* Py_SetProgramName()
* Py_SetPythonHome()
* Py_SetStandardStreamEncoding()
* Py_TRASHCAN_SAFE_BEGIN
* Py_TRASHCAN_SAFE_END
* _Py_SetProgramFullPath()

Removed in Python 3.12
----------------------

* PyUnicode_AS_DATA()
* PyUnicode_AS_UNICODE()
* PyUnicode_AsUnicode()
* PyUnicode_AsUnicodeAndSize()
* PyUnicode_FromUnicode()
* PyUnicode_GET_DATA_SIZE()
* PyUnicode_GET_SIZE()
* PyUnicode_GetSize()
* PyUnicode_WCHAR_KIND()

Removed in Python 3.11
----------------------

* HAVE_PY_SET_53BIT_PRECISION
* PyFrame_BlockPop()
* PyFrame_BlockSetup()
* PyHeapType_GET_MEMBERS()
* PyUnicode_Encode()
* PyUnicode_EncodeASCII()
* PyUnicode_EncodeCharmap()
* PyUnicode_EncodeDecimal()
* PyUnicode_EncodeLatin1()
* PyUnicode_EncodeRawUnicodeEscape()
* PyUnicode_EncodeUTF16()
* PyUnicode_EncodeUTF32()
* PyUnicode_EncodeUTF7()
* PyUnicode_EncodeUTF8()
* PyUnicode_EncodeUnicodeEscape()
* PyUnicode_TransformDecimalToASCII()
* PyUnicode_TranslateCharmap()
* Py_ADJUST_ERANGE1()
* Py_ADJUST_ERANGE2()
* Py_FORCE_DOUBLE()
* Py_OVERFLOWED()
* Py_SET_ERANGE_IF_OVERFLOW()
* Py_SET_ERRNO_ON_MATH_ERROR()
* Py_UNICODE_COPY()
* Py_UNICODE_FILL()

Removed in Python 3.10
----------------------

* PyAST_Compile()
* PyAST_CompileEx()
* PyAST_CompileObject()
* PyAST_Validate()
* PyArena_AddPyObject()
* PyArena_Free()
* PyArena_Malloc()
* PyArena_New()
* PyFuture_FromAST()
* PyFuture_FromASTObject()
* PyLong_FromUnicode()
* PyOS_InitInterrupts()
* PyParser_ASTFromFile()
* PyParser_ASTFromFileObject()
* PyParser_ASTFromFilename()
* PyParser_ASTFromString()
* PyParser_ASTFromStringObject()
* PyST_GetScope()
* PySymtable_Build()
* PySymtable_BuildObject()
* PySymtable_Free()
* PyUnicode_AsUnicodeCopy()
* Py_ALLOW_RECURSION
* Py_END_ALLOW_RECURSION
* Py_SymtableString()
* Py_SymtableStringObject()
* Py_UNICODE_strcat()
* Py_UNICODE_strchr()
* Py_UNICODE_strcmp()
* Py_UNICODE_strcpy()
* Py_UNICODE_strlen()
* Py_UNICODE_strncmp()
* Py_UNICODE_strncpy()
* Py_UNICODE_strrchr()

Removed in Python 3.9
---------------------

* PyAsyncGen_ClearFreeLists()
* PyCFunction_ClearFreeList()
* PyCmpWrapper_Type()
* PyContext_ClearFreeList()
* PyDict_ClearFreeList()
* PyFloat_ClearFreeList()
* PyFrame_ClearFreeList()
* PyFrame_ExtendStack()
* PyList_ClearFreeList()
* PyMethod_ClearFreeList()
* PyNoArgsFunction()
* PyNullImporter_Type()
* PySet_ClearFreeList()
* PySortWrapper_Type()
* PyTuple_ClearFreeList()
* PyUnicode_ClearFreeList()
* Py_UNICODE_MATCH()
* _PyAIterWrapper_Type()
* _PyBytes_InsertThousandsGrouping()
* _PyBytes_InsertThousandsGroupingLocale()
* _PyFloat_Digits(): implementation was removed in Python 2.6
* _PyFloat_DigitsInit(): implementation was removed in Python 2.6
* _PyFloat_Repr(): implementation was removed in Python 3.1
* _PyThreadState_GetFrame()
* _PyUnicode_ClearStaticStrings()
* _Py_InitializeFromArgs()
* _Py_InitializeFromWideArgs()

Removed in Python 3.8
---------------------

* PyByteArray_Init()
* PyByteArray_Fini()

Removed in Python 3.2
---------------------

* PyCObject_AsVoidPtr()
* PyCObject_Check()
* PyCObject_FromVoidPtr()
* PyCObject_FromVoidPtrAndDesc()
* PyCObject_GetDesc()
* PyCObject_Import()
* PyCObject_SetVoidPtr()
* PyOS_ascii_atof()
* PyOS_ascii_formatd()
* PyOS_ascii_strtod()

Removed in Python 3.1
---------------------

* PyNumber_Int()

Removed in Python 3.0
---------------------

* PyString prefix was replaced with PyUnicode
* PyInt prefix was replaced with PyLong
* METH_OLDARGS
* PyMember_Get()
* PyMember_Set()
* PyNumber_Coerce()
* PyNumber_CoerceEx()
* WITH_CYCLE_GC
* Many other C API removals.


Unsupported removed C API
=========================

Removed in Python 3.10
----------------------

* PyThreadState.use_tracing member
* _Py_CheckRecursionLimit variable

Removed in Python 3.9
---------------------

* PyTypeObject.tp_print member
* _PyRuntime.getframe member

Changed in Python 3.8
---------------------

* Heap type instances now store a strong reference to the type. Their dealloc
  function must call ``Py_DECREF(Py_TYPE(obj));``.

Removed in Python 3.7
---------------------

* PyExc_RecursionErrorInst variable

Removed in Python 3.4
---------------------

* PyThreadState.tick_counter member

Removed in Python 3.2
---------------------

* PyCObject_Type variable
