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
