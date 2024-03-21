#define DeadPy_NO_ALIAS
#include "deadparrot.h"
#include "private.h"


int DeadPyModule_AddObjectRef(PyObject *module, const char *name, PyObject *value)
{
#if PY_VERSION_HEX >= 0x030A00A3
    return PyModule_AddObjectRef(module, name, value);
#else
    int res;

    if (!value && !PyErr_Occurred()) {
        // PyModule_AddObject() raises TypeError in this case
        PyErr_SetString(PyExc_SystemError,
                        "PyModule_AddObjectRef() must be called "
                        "with an exception raised if value is NULL");
        return -1;
    }

    Py_XINCREF(value);
    res = PyModule_AddObject(module, name, value);
    if (res < 0) {
        Py_XDECREF(value);
    }
    return res;
#endif
}


int DeadPyModule_AddType(PyObject *module, PyTypeObject *type)
{
#if PY_VERSION_HEX >= 0x030900A5
    return DeadPyModule_AddType(module, type);
#else
    const char *name, *dot;

    if (PyType_Ready(type) < 0) {
        return -1;
    }

    // inline _PyType_Name()
    name = type->tp_name;
    assert(name != _Py_NULL);
    dot = strrchr(name, '.');
    if (dot != _Py_NULL) {
        name = dot + 1;
    }

    return PyModule_AddObjectRef(module, name, _PyObject_CAST(type));
#endif
}


int
DeadPyModule_Add(PyObject *mod, const char *name, PyObject *value)
{
#if PY_VERSION_HEX >= 0x030D00A1
    return PyModule_Add(mod, name, value);
#else
    int res = PyModule_AddObjectRef(mod, name, value);
    Py_XDECREF(value);
    return res;
#endif
}
