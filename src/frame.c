#define DeadPy_NO_ALIAS
#include "deadparrot.h"
#include "private.h"


#ifndef PYPY_VERSION
PyCodeObject* DeadPyFrame_GetCode(PyFrameObject *frame)
{
#if PY_VERSION_HEX >= 0x030900B1
    return PyFrame_GetCode(frame);
#else
    assert(frame != _Py_NULL);
    assert(frame->f_code != _Py_NULL);
    return _DeadPy_CAST(PyCodeObject*, Py_NewRef(frame->f_code));
#endif
}
#endif


#ifndef PYPY_VERSION
PyFrameObject* DeadPyFrame_GetBack(PyFrameObject *frame)
{
#if PY_VERSION_HEX >= 0x030900B1
    return PyFrame_GetBack(frame);
#else
    assert(frame != _Py_NULL);
    return _DeadPy_CAST(PyFrameObject*, Py_XNewRef(frame->f_back));
#endif
}
#endif


#ifndef PYPY_VERSION
PyObject* DeadPyFrame_GetLocals(PyFrameObject *frame)
{
#if PY_VERSION_HEX >= 0x030B00A7
    return PyFrame_GetLocals(frame);
#else
#if PY_VERSION_HEX >= 0x030400B1
    if (PyFrame_FastToLocalsWithError(frame) < 0) {
        return NULL;
    }
#else
    PyFrame_FastToLocals(frame);
#endif
    return Py_NewRef(frame->f_locals);
#endif
}
#endif


PyObject* DeadPyFrame_GetGlobals(PyFrameObject *frame)
{
#if PY_VERSION_HEX >= 0x030B00A7
    return PyFrame_GetGlobals(frame);
#else
    return Py_NewRef(frame->f_globals);
#endif
}


#ifndef PYPY_VERSION
PyObject* DeadPyFrame_GetBuiltins(PyFrameObject *frame)
{
#if PY_VERSION_HEX >= 0x030B00A7
    return PyFrame_GetBuiltins(frame);
#else
    return Py_NewRef(frame->f_builtins);
#endif
}
#endif


#ifndef PYPY_VERSION
int DeadPyFrame_GetLasti(PyFrameObject *frame)
{
#if PY_VERSION_HEX >= 0x030B00B1
    return PyFrame_GetLasti(frame);
#else
#  if PY_VERSION_HEX >= 0x030A00A7
    // bpo-27129: Since Python 3.10.0a7, f_lasti is an instruction offset,
    // not a bytes offset anymore. Python uses 16-bit "wordcode" (2 bytes)
    // instructions.
    if (frame->f_lasti < 0) {
        return -1;
    }
    return frame->f_lasti * 2;
#  else
    return frame->f_lasti;
#  endif
#endif
}
#endif


#ifndef PYPY_VERSION
PyObject* DeadPyFrame_GetVar(PyFrameObject *frame, PyObject *name)
{
#if PY_VERSION_HEX >= 0x030C00A2
    return PyFrame_GetVar(frame, name);
#else
    PyObject *locals, *value;

    locals = DeadPyFrame_GetLocals(frame);
    if (locals == NULL) {
        return NULL;
    }
#if PY_VERSION_HEX >= 0x03000000
    value = PyDict_GetItemWithError(locals, name);
#else
    value = _PyDict_GetItemWithError(locals, name);
#endif
    Py_DECREF(locals);

    if (value == NULL) {
        if (PyErr_Occurred()) {
            return NULL;
        }
#if PY_VERSION_HEX >= 0x03000000
        PyErr_Format(PyExc_NameError, "variable %R does not exist", name);
#else
        PyErr_SetString(PyExc_NameError, "variable does not exist");
#endif
        return NULL;
    }
    return Py_NewRef(value);
#endif
}
#endif


#ifndef PYPY_VERSION
PyObject* DeadPyFrame_GetVarString(PyFrameObject *frame, const char *name)
{
#if PY_VERSION_HEX >= 0x030C00A2
    return PyFrame_GetVarString(frame, name);
#else
    PyObject *name_obj, *value;
#if PY_VERSION_HEX >= 0x03000000
    name_obj = PyUnicode_FromString(name);
#else
    name_obj = PyString_FromString(name);
#endif
    if (name_obj == NULL) {
        return NULL;
    }
    value = DeadPyFrame_GetVar(frame, name_obj);
    Py_DECREF(name_obj);
    return value;
#endif
}
#endif
