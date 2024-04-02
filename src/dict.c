#define DeadPy_NO_ALIAS
#include "deadparrot.h"
#include "private.h"


int
DeadPyDict_GetItemRef(PyObject *mp, PyObject *key, PyObject **result)
{
#if PY_VERSION_HEX >= 0x030D00A1
    return PyDict_GetItemRef(mp, key, result);
#else
#  if PY_VERSION_HEX >= 0x03000000
    PyObject *item = PyDict_GetItemWithError(mp, key);
#  else
    PyObject *item = _PyDict_GetItemWithError(mp, key);
#  endif
    if (item != NULL) {
        *result = Py_NewRef(item);
        return 1;  // found
    }
    if (!PyErr_Occurred()) {
        *result = NULL;
        return 0;  // not found
    }
    *result = NULL;
    return -1;
#endif
}


int
DeadPyDict_GetItemStringRef(PyObject *mp, const char *key, PyObject **result)
{
#if PY_VERSION_HEX >= 0x030D00A1
    return PyDict_GetItemStringRef(mp, key, result);
#else
    int res;
#  if PY_VERSION_HEX >= 0x03000000
    PyObject *key_obj = PyUnicode_FromString(key);
#  else
    PyObject *key_obj = PyString_FromString(key);
#  endif
    if (key_obj == NULL) {
        *result = NULL;
        return -1;
    }
    res = DeadPyDict_GetItemRef(mp, key_obj, result);
    Py_DECREF(key_obj);
    return res;
#endif
}


int
DeadPyDict_ContainsString(PyObject *op, const char *key)
{
#if PY_VERSION_HEX >= 0x030D00A1
    return PyDict_ContainsString(op, key);
#else
    PyObject *key_obj = PyUnicode_FromString(key);
    if (key_obj == NULL) {
        return -1;
    }
    int res = PyDict_Contains(op, key_obj);
    Py_DECREF(key_obj);
    return res;
#endif
}


int
DeadPyDict_Pop(PyObject *dict, PyObject *key, PyObject **result)
{
#if PY_VERSION_HEX >= 0x030D00A2
    return PyDict_Pop(dict, key, result);
#else
    PyObject *value;

    if (!PyDict_Check(dict)) {
        PyErr_BadInternalCall();
        if (result) {
            *result = NULL;
        }
        return -1;
    }

#if defined(PYPY_VERSION) || PY_VERSION_HEX < 0x030500b2 || PY_VERSION_HEX >= 0x030D0000
    value = PyObject_CallMethod(dict, "pop", "O", key);
#elif PY_VERSION_HEX < 0x030600b3
    value = _PyDict_Pop(_Py_CAST(PyDictObject*, dict), key, NULL);
#else
    value = _PyDict_Pop(dict, key, NULL);
#endif
    if (value == NULL) {
        if (result) {
            *result = NULL;
        }
        if (PyErr_Occurred() && !PyErr_ExceptionMatches(PyExc_KeyError)) {
            return -1;
        }
        PyErr_Clear();
        return 0;
    }
    if (result) {
        *result = value;
    }
    else {
        Py_DECREF(value);
    }
    return 1;
#endif
}


int
DeadPyDict_PopString(PyObject *dict, const char *key, PyObject **result)
{
#if PY_VERSION_HEX >= 0x030D00A2
    return PyDict_PopString(dict, key, result);
#else

    PyObject *key_obj = PyUnicode_FromString(key);
    if (key_obj == NULL) {
        if (result != NULL) {
            *result = NULL;
        }
        return -1;
    }

    int res = DeadPyDict_Pop(dict, key_obj, result);
    Py_DECREF(key_obj);
    return res;
#endif
}


int
DeadPyDict_SetDefaultRef(PyObject *d, PyObject *key, PyObject *default_value,
                         PyObject **result)
{
#if PY_VERSION_HEX >= 0x030D00A4
    return PyDict_SetDefaultRef(d, key, default_value, result);
#else
    PyObject *value;
    if (DeadPyDict_GetItemRef(d, key, &value) < 0) {
        // get error
        if (result) {
            *result = NULL;
        }
        return -1;
    }
    if (value != NULL) {
        // present
        if (result) {
            *result = value;
        }
        else {
            Py_DECREF(value);
        }
        return 1;
    }

    // missing: set the item
    if (PyDict_SetItem(d, key, default_value) < 0) {
        // set error
        if (result) {
            *result = NULL;
        }
        return -1;
    }
    if (result) {
        *result = Py_NewRef(default_value);
    }
    return 0;
#endif
}
