#define DeadPy_NO_ALIAS
#include "deadparrot.h"
#include "private.h"

#ifndef PYPY_VERSION


int DeadPyObject_GC_IsTracked(PyObject* obj)
{
#if PY_VERSION_HEX >= 0x030900A6
    return PyObject_GC_IsTracked(obj);
#else
    return (PyObject_IS_GC(obj) && _PyObject_GC_IS_TRACKED(obj));
#endif
}


#if PY_VERSION_HEX >= 0x030400F0
int DeadPyObject_GC_IsFinalized(PyObject *obj)
{
#if PY_VERSION_HEX >= 0x030900A6
    return PyObject_GC_IsFinalized(obj);
#else
    PyGC_Head *gc = _Py_CAST(PyGC_Head*, obj) - 1;
    return (PyObject_IS_GC(obj) && _PyGCHead_FINALIZED(gc));
#endif
}
#endif


#endif  // !PYPY_VERSION
