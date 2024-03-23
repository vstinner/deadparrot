#define DeadPy_NO_ALIAS
#include "deadparrot.h"
#include "private.h"
#ifndef PYPY_VERSION


PyObject* DeadPyCode_GetCode(PyCodeObject *code)
{
#if PY_VERSION_HEX >= 0x030B00B1
    return PyCode_GetCode(code);
#else
    return Py_NewRef(code->co_code);
#endif
}

PyObject* DeadPyCode_GetVarnames(PyCodeObject *code)
{
#if PY_VERSION_HEX >= 0x030B00C1
    return PyCode_GetVarnames(code);
#else
    return Py_NewRef(code->co_varnames);
#endif
}

PyObject* DeadPyCode_GetFreevars(PyCodeObject *code)
{
#if PY_VERSION_HEX >= 0x030B00C1
    return PyCode_GetFreevars(code);
#else
    return Py_NewRef(code->co_freevars);
#endif
}

PyObject* DeadPyCode_GetCellvars(PyCodeObject *code)
{
#if PY_VERSION_HEX >= 0x030B00C1
    return PyCode_GetCellvars(code);
#else
    return Py_NewRef(code->co_cellvars);
#endif
}


#endif  // !PYPY_VERSION
