#define DeadPy_NO_ALIAS
#include "deadparrot.h"
#include "private.h"


PyFrameObject* DeadPyThreadState_GetFrame(PyThreadState *tstate)
{
#if PY_VERSION_HEX >= 0x030900B1
    return DeadPyThreadState_GetFrame(tstate);
#else
    assert(tstate != _Py_NULL);
    return _DeadPy_CAST(PyFrameObject *, Py_XNewRef(tstate->frame));
#endif
}
