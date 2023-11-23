#define DeadPy_NO_ALIAS
#include "deadparrot.h"
#include "private.h"

IGNORE_DEPR_WARNINGS


PyObject*
DeadPyEval_CallObjectWithKeywords(PyObject *callable,
                                  PyObject *args, PyObject *kwargs)
{
#if PY_VERSION_HEX >= 0x030D00A1
    if (args != NULL && !PyTuple_Check(args)) {
        PyErr_SetString(PyExc_TypeError,
                        "argument list must be a tuple");
        return NULL;
    }

    if (kwargs != NULL && !PyDict_Check(kwargs)) {
        PyErr_SetString(PyExc_TypeError,
                        "keyword list must be a dictionary");
        return NULL;
    }

    if (args == NULL) {
        args = PyTuple_New(0);
        if (args == NULL) {
            return NULL;
        }
        PyObject *res = PyObject_Call(callable, args, kwargs);
        Py_DECREF(args);
        return res;
    }
    else {
        return PyObject_Call(callable, args, kwargs);
    }
#else
    return PyEval_CallObjectWithKeywords(callable, args, kwargs);
#endif
}


PyObject*
DeadPyEval_CallObject(PyObject *callable, PyObject *args)
{
#if PY_VERSION_HEX >= 0x030D00A1
    return DeadPyEval_CallObjectWithKeywords(callable, args, NULL);
#else
    return PyEval_CallObject(callable, args);
#endif
}


PyObject*
DeadPyCFunction_Call(PyObject *callable, PyObject *args, PyObject *kwargs)
{
#if PY_VERSION_HEX >= 0x030D00A1
    return PyObject_Call(callable, args, NULL);
#else
    return PyCFunction_Call(callable, args, kwargs);
#endif
}

PyObject *
dead_build_args_tuple(const char *format, va_list vargs)
{
    PyObject *args = Py_VaBuildValue(format, vargs);
    if (args == NULL) {
        return NULL;
    }
    if (!PyTuple_Check(args)) {
        Py_DECREF(args);
        PyErr_SetString(PyExc_TypeError,
                        "argument list must be a tuple");
        return NULL;
    }
    return args;
}


PyObject *
DeadPyEval_CallFunction(PyObject *callable, const char *format, ...)
{
    va_list vargs;
    va_start(vargs, format);
    PyObject *args = dead_build_args_tuple(format, vargs);
    va_end(vargs);
    if (args == NULL) {
        return NULL;
    }

    PyObject *res = PyObject_Call(callable, args, NULL);
    Py_DECREF(args);
    return res;
}


PyObject *
DeadPyEval_CallMethod(PyObject *obj, const char *name, const char *format, ...)
{
    if (obj == NULL || name == NULL) {
        PyErr_BadArgument();
        return NULL;
    }

    va_list vargs;
    va_start(vargs, format);
    PyObject *args = dead_build_args_tuple(format, vargs);
    va_end(vargs);
    if (args == NULL) {
        return NULL;
    }

    PyObject *callable = PyObject_GetAttrString(obj, name);
    if (callable == NULL) {
        Py_DECREF(args);
        return NULL;
    }

    PyObject *res = PyObject_Call(callable, args, NULL);
    Py_DECREF(callable);
    Py_DECREF(args);
    return res;
}


#if PY_VERSION_HEX >= 0x03060000
PyObject *
_DeadPyObject_FastCall(PyObject *func, PyObject *const *args, Py_ssize_t nargs)
{
#if PY_VERSION_HEX >= 0x030D00A1
    return PyObject_Vectorcall(func, args, (size_t)nargs, NULL);
#else
    return _PyObject_FastCall(func, (PyObject **)args, nargs);
#endif
}
#endif
