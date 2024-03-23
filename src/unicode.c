#define DeadPy_NO_ALIAS
#include "deadparrot.h"
#include "private.h"

IGNORE_DEPR_WARNINGS


DeadPy_UNICODE
DeadPyUnicode_GetMax(void)
{
#if PY_VERSION_HEX >= 0x030A0000
    return (DeadPy_UNICODE)0x10ffff;
#else
    return PyUnicode_GetMax();
#endif
}

#if PY_VERSION_HEX >= 0x03000000 && !defined(PYPY_VERSION)
void
DeadPyUnicode_InternImmortal(PyObject **p)
{
#if PY_VERSION_HEX >= 0x030C0000
    // The function was just removed from the API,
    // but still exists in the stable ABI.
    extern void PyUnicode_InternImmortal(PyObject **p);
#endif
    PyUnicode_InternImmortal(p);
}
#endif


int
DeadPyUnicode_EqualToUTF8AndSize(PyObject *unicode, const char *str, Py_ssize_t str_len)
{
#if PY_VERSION_HEX >= 0x030D00A1
    return PyUnicode_EqualToUTF8AndSize(unicode, str, str_len);
#else
    Py_ssize_t len;
    const void *utf8;
    PyObject *exc_type, *exc_value, *exc_tb;
    int res;

    // API cannot report errors so save/restore the exception
    PyErr_Fetch(&exc_type, &exc_value, &exc_tb);

    // Python 3.3.0a1 added PyUnicode_AsUTF8AndSize()
#if PY_VERSION_HEX >= 0x030300A1
    if (PyUnicode_IS_ASCII(unicode)) {
        utf8 = PyUnicode_DATA(unicode);
        len = PyUnicode_GET_LENGTH(unicode);
    }
    else {
        utf8 = PyUnicode_AsUTF8AndSize(unicode, &len);
        if (utf8 == NULL) {
            // Memory allocation failure. The API cannot report error,
            // so ignore the exception and return 0.
            res = 0;
            goto done;
        }
    }

    if (len != str_len) {
        res = 0;
        goto done;
    }
    res = (memcmp(utf8, str, (size_t)len) == 0);
#else
    PyObject *bytes = PyUnicode_AsUTF8String(unicode);
    if (bytes == NULL) {
        // Memory allocation failure. The API cannot report error,
        // so ignore the exception and return 0.
        res = 0;
        goto done;
    }

#if PY_VERSION_HEX >= 0x03000000
    len = PyBytes_GET_SIZE(bytes);
    utf8 = PyBytes_AS_STRING(bytes);
#else
    len = PyString_GET_SIZE(bytes);
    utf8 = PyString_AS_STRING(bytes);
#endif
    if (len != str_len) {
        Py_DECREF(bytes);
        res = 0;
        goto done;
    }

    res = (memcmp(utf8, str, (size_t)len) == 0);
    Py_DECREF(bytes);
#endif

done:
    PyErr_Restore(exc_type, exc_value, exc_tb);
    return res;
#endif
}

int
DeadPyUnicode_EqualToUTF8(PyObject *unicode, const char *str)
{
#if PY_VERSION_HEX >= 0x030D00A1
    return PyUnicode_EqualToUTF8(unicode, str);
#else
    return DeadPyUnicode_EqualToUTF8AndSize(unicode, str, (Py_ssize_t)strlen(str));
#endif
}
