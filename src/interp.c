#define DeadPy_NO_ALIAS
#include "deadparrot.h"
#include "private.h"

PyInterpreterState* DeadPyInterpreterState_Get(void)
{
#if PY_VERSION_HEX >= 0x030900A5 && !defined(PYPY_VERSION)
    return PyInterpreterState_Get();
#else
    PyThreadState *tstate;
    PyInterpreterState *interp;

    tstate = PyThreadState_GET();
    if (tstate == _Py_NULL) {
        Py_FatalError("GIL released (tstate is NULL)");
    }
    interp = tstate->interp;
    if (interp == _Py_NULL) {
        Py_FatalError("no current interpreter");
    }
    return interp;
#endif
}


#if 0x03020000 <= PY_VERSION_HEX
int DeadPy_IsFinalizing(void)
{
#if PY_VERSION_HEX >= 0x030D00A1
    return Py_IsFinalizing();
#elif PY_VERSION_HEX >= 0x030700A1
    // _Py_IsFinalizing() was added to Python 3.7.0a1.
    return _Py_IsFinalizing();
#else
    return (_Py_Finalizing != NULL);
#endif
}
#endif
