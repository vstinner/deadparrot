#define DeadPy_NO_ALIAS
#include "deadparrot.h"
#include "private.h"


int DeadPyList_Extend(PyObject *list, PyObject *iterable)
{
#if PY_VERSION_HEX >= 0x030D00A2
    return PyList_Extend(list, iterable);
#else
    return PyList_SetSlice(list, PY_SSIZE_T_MAX, PY_SSIZE_T_MAX, iterable);
#endif
}


int DeadPyList_Clear(PyObject *list)
{
#if PY_VERSION_HEX >= 0x030D00A2
    return PyList_Clear(list);
#else
    return PyList_SetSlice(list, 0, PY_SSIZE_T_MAX, NULL);
#endif
}
