#define DeadPy_NO_ALIAS
#include "deadparrot.h"
#include "private.h"
#if 0x03050000 <= PY_VERSION_HEX


double DeadPyTime_AsSecondsDouble(PyTime_t t)
{
#if PY_VERSION_HEX >= 0x030D00A4
    return PyTime_AsSecondsDouble(t);
#else
    return _PyTime_AsSecondsDouble(t);
#endif
}


int DeadPyTime_Monotonic(PyTime_t *result)
{
#if PY_VERSION_HEX >= 0x030D00A4
    return PyTime_Monotonic(result);
#else
    return _PyTime_GetMonotonicClockWithInfo(result, NULL);
#endif
}


int DeadPyTime_Time(PyTime_t *result)
{
#if PY_VERSION_HEX >= 0x030D00A4
    return PyTime_Time(result);
#else
    return _PyTime_GetSystemClockWithInfo(result, NULL);
#endif
}


int DeadPyTime_PerfCounter(PyTime_t *result)
{
#if PY_VERSION_HEX >= 0x030D00A4
    return PyTime_PerfCounter(result);
#elif PY_VERSION_HEX >= 0x03070000 && !defined(PYPY_VERSION)
    return _PyTime_GetPerfCounterWithInfo(result, NULL);
#elif PY_VERSION_HEX >= 0x03070000
    // Call time.perf_counter_ns() and convert Python int object to PyTime_t.

    // Cache time.perf_counter_ns() function for best performance.
    static PyObject *func = NULL;
    if (func == NULL) {
        PyObject *mod = PyImport_ImportModule("time");
        if (mod == NULL) {
            return -1;
        }

        func = PyObject_GetAttrString(mod, "perf_counter_ns");
        Py_DECREF(mod);
        if (func == NULL) {
            return -1;
        }
    }

    PyObject *res = PyObject_CallNoArgs(func);
    if (res == NULL) {
        return -1;
    }
    long long value = PyLong_AsLongLong(res);
    Py_DECREF(res);

    if (value == -1 && PyErr_Occurred()) {
        return -1;
    }

    Py_BUILD_ASSERT(sizeof(value) >= sizeof(PyTime_t));
    *result = (PyTime_t)value;
    return 0;
#else
    // Call time.perf_counter() and convert C double to PyTime_t.

    // Cache time.perf_counter() function for best performance.
    static PyObject *func = NULL;
    if (func == NULL) {
        PyObject *mod = PyImport_ImportModule("time");
        if (mod == NULL) {
            return -1;
        }

        func = PyObject_GetAttrString(mod, "perf_counter");
        Py_DECREF(mod);
        if (func == NULL) {
            return -1;
        }
    }

    PyObject *res = DeadPyObject_CallNoArgs(func);
    if (res == NULL) {
        return -1;
    }
    double d = PyFloat_AsDouble(res);
    Py_DECREF(res);

    if (d == -1.0 && PyErr_Occurred()) {
        return -1;
    }

    // Avoid floor() to avoid having to link to libm
    *result = (PyTime_t)(d * 1e9);
    return 0;
#endif
}


#endif  // 0x03050000 <= PY_VERSION_HEX
