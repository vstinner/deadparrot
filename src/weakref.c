#define DeadPy_NO_ALIAS
#include "deadparrot.h"
#include "private.h"


int DeadPyWeakref_GetRef(PyObject *ref, PyObject **pobj)
{
#if PY_VERSION_HEX >= 0x030D0000
    return PyWeakref_GetRef(ref, pobj);
#else
    PyObject *obj;
    if (ref != NULL && !PyWeakref_Check(ref)) {
        *pobj = NULL;
        PyErr_SetString(PyExc_TypeError, "expected a weakref");
        return -1;
    }
    obj = PyWeakref_GetObject(ref);
    if (obj == NULL) {
        // SystemError if ref is NULL
        *pobj = NULL;
        return -1;
    }
    if (obj == Py_None) {
        *pobj = NULL;
        return 0;
    }
    *pobj = Py_NewRef(obj);
    return (*pobj != NULL);
#endif
}
