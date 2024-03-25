#define DeadPy_NO_ALIAS
#include "deadparrot.h"
#include "private.h"

int
DeadPyMapping_GetOptionalItem(PyObject *obj, PyObject *key, PyObject **result)
{
#if PY_VERSION_HEX >= 0x030D00A1
    return PyMapping_GetOptionalItem(obj, key, result);
#else
    *result = PyObject_GetItem(obj, key);
    if (*result) {
        return 1;
    }
    if (!PyErr_ExceptionMatches(PyExc_KeyError)) {
        return -1;
    }
    PyErr_Clear();
    return 0;
#endif
}


int
DeadPyMapping_GetOptionalItemString(PyObject *obj, const char *key, PyObject **result)
{
#if PY_VERSION_HEX >= 0x030D00A1
    return PyMapping_GetOptionalItemString(obj, key, result);
#else
    PyObject *key_obj;
    int rc;
#  if PY_VERSION_HEX >= 0x03000000
    key_obj = PyUnicode_FromString(key);
#  else
    key_obj = PyString_FromString(key);
#  endif
    if (key_obj == NULL) {
        *result = NULL;
        return -1;
    }
    rc = DeadPyMapping_GetOptionalItem(obj, key_obj, result);
    Py_DECREF(key_obj);
    return rc;
#endif
}


int
DeadPyMapping_HasKeyWithError(PyObject *obj, PyObject *key)
{
#if PY_VERSION_HEX >= 0x030D00A1
    return PyMapping_HasKeyWithError(obj, key);
#else
    PyObject *res;
    int rc = DeadPyMapping_GetOptionalItem(obj, key, &res);
    Py_XDECREF(res);
    return rc;
#endif
}


int
DeadPyMapping_HasKeyStringWithError(PyObject *obj, const char *key)
{
#if PY_VERSION_HEX >= 0x030D00A1
    return PyMapping_HasKeyStringWithError(obj, key);
#else
    PyObject *res;
    int rc = DeadPyMapping_GetOptionalItemString(obj, key, &res);
    Py_XDECREF(res);
    return rc;
#endif
}


int
DeadPyObject_GetOptionalAttr(PyObject *obj, PyObject *attr_name, PyObject **result)
{
#if PY_VERSION_HEX >= 0x030D00A1
    return PyObject_GetOptionalAttr(obj, attr_name, result);
#else
    // bpo-32571 added _PyObject_LookupAttr() to Python 3.7.0b1
#  if PY_VERSION_HEX >= 0x030700B1 && !defined(PYPY_VERSION)
    return _PyObject_LookupAttr(obj, attr_name, result);
#  else
    *result = PyObject_GetAttr(obj, attr_name);
    if (*result != NULL) {
        return 1;
    }
    if (!PyErr_Occurred()) {
        return 0;
    }
    if (PyErr_ExceptionMatches(PyExc_AttributeError)) {
        PyErr_Clear();
        return 0;
    }
    return -1;
#  endif
#endif
}


int
DeadPyObject_GetOptionalAttrString(PyObject *obj, const char *attr_name, PyObject **result)
{
#if PY_VERSION_HEX >= 0x030D00A1
    return PyObject_GetOptionalAttrString(obj, attr_name, result);
#else
    PyObject *name_obj;
    int rc;
#  if PY_VERSION_HEX >= 0x03000000
    name_obj = PyUnicode_FromString(attr_name);
#  else
    name_obj = PyString_FromString(attr_name);
#  endif
    if (name_obj == NULL) {
        *result = NULL;
        return -1;
    }
    rc = DeadPyObject_GetOptionalAttr(obj, name_obj, result);
    Py_DECREF(name_obj);
    return rc;
#endif
}


int
DeadPyObject_HasAttrWithError(PyObject *obj, PyObject *attr)
{
#if PY_VERSION_HEX >= 0x030D00A1
    return PyObject_HasAttrWithError(obj, attr);
#else
    PyObject *res;
    int rc = DeadPyObject_GetOptionalAttr(obj, attr, &res);
    Py_XDECREF(res);
    return rc;
#endif
}


int
DeadPyObject_HasAttrStringWithError(PyObject *obj, const char *attr)
{
#if PY_VERSION_HEX >= 0x030D00A1
    return PyObject_HasAttrStringWithError(obj, attr);
#else
    PyObject *res;
    int rc = DeadPyObject_GetOptionalAttrString(obj, attr, &res);
    Py_XDECREF(res);
    return rc;
#endif
}
