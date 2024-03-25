#define DeadPy_NO_ALIAS
#include "deadparrot.h"
#include "private.h"

IGNORE_DEPR_WARNINGS


PyObject* DeadPyObject_CallNoArgs(PyObject *func)
{
#if PY_VERSION_HEX >= 0x030900A1
    return PyObject_CallNoArgs(func);
#else
    return PyObject_CallFunctionObjArgs(func, NULL);
#endif
}


PyObject* DeadPyObject_CallOneArg(PyObject *func, PyObject *arg)
{
#if PY_VERSION_HEX >= 0x030900A4
    return PyObject_CallOneArg(func, arg);
#else
    return PyObject_CallFunctionObjArgs(func, arg, NULL);
#endif
}


PyObject*
DeadPyEval_CallObjectWithKeywords(PyObject *callable,
                                  PyObject *args, PyObject *kwargs)
{
#if PY_VERSION_HEX < 0x030D00A1
    return PyEval_CallObjectWithKeywords(callable, args, kwargs);
#else
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
#endif
}


PyObject*
DeadPyEval_CallObject(PyObject *callable, PyObject *args)
{
#if PY_VERSION_HEX < 0x030D00A1
    return PyEval_CallObject(callable, args);
#else
    return DeadPyEval_CallObjectWithKeywords(callable, args, NULL);
#endif
}


PyObject*
DeadPyCFunction_Call(PyObject *callable, PyObject *args, PyObject *kwargs)
{
#if PY_VERSION_HEX < 0x030D00A1
    return PyCFunction_Call(callable, args, kwargs);
#else
    return PyObject_Call(callable, args, NULL);
#endif
}

PyObject *
dead_build_vargs_tuple(const char *format, va_list vargs)
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
    PyObject *args = dead_build_vargs_tuple(format, vargs);
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
    PyObject *args = dead_build_vargs_tuple(format, vargs);
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


PyObject *
_DeadPyObject_FastCall(PyObject *func, PyObject *const *args, Py_ssize_t nargs)
{
#if PY_VERSION_HEX >= 0x030D00A1
    return PyObject_Vectorcall(func, args, (size_t)nargs, NULL);
#elif ((PY_VERSION_HEX >= 0x03060000 && !defined(PYPY_VERSION)) \
       || (PY_VERSION_HEX >= 0x03090000 && defined(PYPY_VERSION)))
    return _PyObject_FastCall(func, (PyObject **)args, nargs);
#else
    // PyPy for Python 3.8 and older
    PyObject *tuple = PyTuple_New(nargs);
    if (tuple == NULL) {
        return NULL;
    }
    for (Py_ssize_t i=0; i < nargs; i++) {
        PyTuple_SET_ITEM(tuple, i, Py_NewRef(args[i]));
    }

    PyObject *res = PyObject_Call(func, tuple, NULL);
    Py_DECREF(tuple);
    return res;
#endif
}


Py_ssize_t DeadPyVectorcall_NARGS(size_t n)
{
#if PY_VERSION_HEX >= 0x030800B1
    return PyVectorcall_NARGS(n);
#else
    return n & ~PY_VECTORCALL_ARGUMENTS_OFFSET;
#endif
}


PyObject*
DeadPyObject_Vectorcall(PyObject *callable, PyObject *const *args,
                        size_t nargsf, PyObject *kwnames)
{
#if PY_VERSION_HEX >= 0x030900A4
    return PyObject_Vectorcall(callable, args, nargsf, kwnames);
#else
#  if PY_VERSION_HEX >= 0x030800B1 && !defined(PYPY_VERSION)
    // bpo-36974 added _PyObject_Vectorcall() to Python 3.8.0b1
    return _PyObject_Vectorcall(callable, args, nargsf, kwnames);
#  else
    PyObject *posargs = NULL, *kwargs = NULL;
    PyObject *res;
    Py_ssize_t nposargs, nkwargs, i;

    if (nargsf != 0 && args == NULL) {
        PyErr_BadInternalCall();
        goto error;
    }
    if (kwnames != NULL && !PyTuple_Check(kwnames)) {
        PyErr_BadInternalCall();
        goto error;
    }

    nposargs = (Py_ssize_t)DeadPyVectorcall_NARGS(nargsf);
    if (kwnames) {
        nkwargs = PyTuple_GET_SIZE(kwnames);
    }
    else {
        nkwargs = 0;
    }

    posargs = PyTuple_New(nposargs);
    if (posargs == NULL) {
        goto error;
    }
    if (nposargs) {
        for (i=0; i < nposargs; i++) {
            PyTuple_SET_ITEM(posargs, i, Py_NewRef(*args));
            args++;
        }
    }

    if (nkwargs) {
        kwargs = PyDict_New();
        if (kwargs == NULL) {
            goto error;
        }

        for (i = 0; i < nkwargs; i++) {
            PyObject *key = PyTuple_GET_ITEM(kwnames, i);
            PyObject *value = *args;
            args++;
            if (PyDict_SetItem(kwargs, key, value) < 0) {
                goto error;
            }
        }
    }
    else {
        kwargs = NULL;
    }

    res = PyObject_Call(callable, posargs, kwargs);
    Py_DECREF(posargs);
    Py_XDECREF(kwargs);
    return res;

error:
    Py_DECREF(posargs);
    Py_XDECREF(kwargs);
    return NULL;
#  endif
#endif
}
