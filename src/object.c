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
