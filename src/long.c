#define DeadPy_NO_ALIAS
#include "deadparrot.h"
#include "private.h"

int DeadPyLong_AsInt(PyObject *obj)
{
#if PY_VERSION_HEX >= 0x030D00A1
    return PyLong_AsInt(obj);
#elif !defined(PYPY_VERSION)
    return _PyLong_AsInt(obj);
#else
    // PyPy implementation
    long value = PyLong_AsLong(obj);
    if (value == -1 && PyErr_Occurred()) {
        return -1;
    }
    if (value < (long)INT_MIN || (long)INT_MAX < value) {
        PyErr_SetString(PyExc_OverflowError,
                        "Python int too large to convert to C int");
        return -1;
    }
    return (int)value;
#endif
}
