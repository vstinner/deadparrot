#define DeadPy_NO_ALIAS
#include "deadparrot.h"
#include "private.h"

IGNORE_DEPR_WARNINGS

void DeadPyEval_InitThreads(void)
{
#if PY_VERSION_HEX >= 0x030D00A1
    // do nothing
#else
    PyEval_InitThreads();
#endif
}
