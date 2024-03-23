#define DeadPy_NO_ALIAS
#include "deadparrot.h"
#include "private.h"


PyObject* DeadPy_NewRef(PyObject *obj)
{
    Py_INCREF(obj);
    return obj;
}


PyObject* DeadPy_XNewRef(PyObject *obj)
{
    Py_XINCREF(obj);
    return obj;
}


void DeadPy_SET_REFCNT(PyObject *obj, Py_ssize_t refcnt)
{
    obj->ob_refcnt = refcnt;
}


void DeadPy_SET_TYPE(PyObject *obj, PyTypeObject *type)
{
    obj->ob_type = type;
}


void DeadPy_SET_SIZE(PyVarObject *obj, Py_ssize_t size)
{
    obj->ob_size = size;
}


int DeadPy_Is(PyObject *x, PyObject *y)
{
    return (x == y);
}


int DeadPy_IsNone(PyObject *x)
{
    return (x == Py_None);
}


int DeadPy_IsFalse(PyObject *x)
{
    return (x == Py_False);
}


int DeadPy_IsTrue(PyObject *x)
{
    return (x == Py_True);
}


int DeadPy_IS_TYPE(PyObject *ob, PyTypeObject *type) {
#if PY_VERSION_HEX >= 0x030900A4
    return Py_IS_TYPE(ob, type);
#else
    return Py_TYPE(ob) == type;
#endif
}

PyObject* DeadPy_GetConstant(unsigned int constant_id)
{
#if PY_VERSION_HEX >= 0x030D00A6
    return Py_GetConstant(constant_id);
#else
    static PyObject* constants[Py_CONSTANT_EMPTY_TUPLE + 1] = {NULL};

    if (constants[Py_CONSTANT_NONE] == NULL) {
        constants[Py_CONSTANT_NONE] = Py_None;
        constants[Py_CONSTANT_FALSE] = Py_False;
        constants[Py_CONSTANT_TRUE] = Py_True;
        constants[Py_CONSTANT_ELLIPSIS] = Py_Ellipsis;
        constants[Py_CONSTANT_NOT_IMPLEMENTED] = Py_NotImplemented;

        constants[Py_CONSTANT_ZERO] = PyLong_FromLong(0);
        if (constants[Py_CONSTANT_ZERO] == NULL) {
            goto fatal_error;
        }

        constants[Py_CONSTANT_ONE] = PyLong_FromLong(1);
        if (constants[Py_CONSTANT_ONE] == NULL) {
            goto fatal_error;
        }

        constants[Py_CONSTANT_EMPTY_STR] = PyUnicode_FromStringAndSize("", 0);
        if (constants[Py_CONSTANT_EMPTY_STR] == NULL) {
            goto fatal_error;
        }

        constants[Py_CONSTANT_EMPTY_BYTES] = PyBytes_FromStringAndSize("", 0);
        if (constants[Py_CONSTANT_EMPTY_BYTES] == NULL) {
            goto fatal_error;
        }

        constants[Py_CONSTANT_EMPTY_TUPLE] = PyTuple_New(0);
        if (constants[Py_CONSTANT_EMPTY_TUPLE] == NULL) {
            goto fatal_error;
        }
        // goto dance to avoid compiler warnings about Py_FatalError()
        goto init_done;

fatal_error:
        // This case should never happen
        Py_FatalError("Py_GetConstant() failed to get constants");
    }

init_done:
    if (constant_id <= Py_CONSTANT_EMPTY_TUPLE) {
        return Py_NewRef(constants[constant_id]);
    }
    else {
        PyErr_BadInternalCall();
        return NULL;
    }
#endif
}


PyObject* DeadPy_GetConstantBorrowed(unsigned int constant_id)
{
#if PY_VERSION_HEX >= 0x030D00A6
    return Py_GetConstantBorrowed(constant_id);
#else
    PyObject *obj = DeadPy_GetConstant(constant_id);
    Py_XDECREF(obj);
    return obj;
#endif
}
