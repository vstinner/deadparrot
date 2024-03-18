#define DeadPy_NO_ALIAS
#include "deadparrot.h"
#include "private.h"


PyFrameObject* DeadPyThreadState_GetFrame(PyThreadState *tstate)
{
#if PY_VERSION_HEX >= 0x030900B1
    return PyThreadState_GetFrame(tstate);
#else
    assert(tstate != _Py_NULL);
    return _DeadPy_CAST(PyFrameObject *, Py_XNewRef(tstate->frame));
#endif
}


PyInterpreterState* DeadPyThreadState_GetInterpreter(PyThreadState *tstate)
{
#if PY_VERSION_HEX >= 0x030900A5
    return PyThreadState_GetInterpreter(tstate);
#else
    assert(tstate != _Py_NULL);
    return tstate->interp;
#endif
}


#if 0x030700A1 <= PY_VERSION_HEX && !defined(PYPY_VERSION)
uint64_t DeadPyThreadState_GetID(PyThreadState *tstate)
{
#if PY_VERSION_HEX >= 0x030900A6
    return PyThreadState_GetID(tstate);
#else
    assert(tstate != _Py_NULL);
    return tstate->id;
#endif
}
#endif


#ifndef PYPY_VERSION
void DeadPyThreadState_EnterTracing(PyThreadState *tstate)
{
#if PY_VERSION_HEX >= 0x030B00A2
    PyThreadState_EnterTracing(tstate);
#else
    tstate->tracing++;
#if PY_VERSION_HEX >= 0x030A00A1
    tstate->cframe->use_tracing = 0;
#else
    tstate->use_tracing = 0;
#endif
#endif
}
#endif


#ifndef PYPY_VERSION
void DeadPyThreadState_LeaveTracing(PyThreadState *tstate)
{
#if PY_VERSION_HEX >= 0x030B00A2
    PyThreadState_LeaveTracing(tstate);
#else
    int use_tracing = (tstate->c_tracefunc != _Py_NULL
                       || tstate->c_profilefunc != _Py_NULL);
    tstate->tracing--;
#if PY_VERSION_HEX >= 0x030A00A1
    tstate->cframe->use_tracing = use_tracing;
#else
    tstate->use_tracing = use_tracing;
#endif
#endif
}
#endif
