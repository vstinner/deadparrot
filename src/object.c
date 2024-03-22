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


int DeadPy_Is(PyObject *x, PyObject *y)
{
    return (x == y);
}


int DeadPy_IsNone(PyObject *x)
{
    return (x == Py_None);
}


int DeadPy_IsFalse(PyObject *x)
{
    return (x == Py_False);
}


int DeadPy_IsTrue(PyObject *x)
{
    return (x == Py_True);
}


int DeadPy_IS_TYPE(PyObject *ob, PyTypeObject *type) {
#if PY_VERSION_HEX >= 0x030900A4
    return Py_IS_TYPE(ob, type);
#else
    return Py_TYPE(ob) == type;
#endif
}
