#define DeadPy_NO_ALIAS

#include "deadparrot.h"
#include "private.h"

// Ignore deprecation warnings
IGNORE_DEPR_WARNINGS


DeadPy_UNICODE
DeadPyUnicode_GetMax(void)
{
#if PY_VERSION_HEX >= 0x030A0000
    return 0x10ffff;
#else
    return PyUnicode_GetMax();
#endif
}
