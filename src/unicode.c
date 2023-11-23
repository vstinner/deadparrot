#define DeadPy_NO_ALIAS
#include "deadparrot.h"
#include "private.h"

IGNORE_DEPR_WARNINGS


DeadPy_UNICODE
DeadPyUnicode_GetMax(void)
{
#if PY_VERSION_HEX >= 0x030A0000
    return (DeadPy_UNICODE)0x10ffff;
#else
    return PyUnicode_GetMax();
#endif
}


#if PY_VERSION_HEX >= 0x03000000
void
DeadPyUnicode_InternImmortal(PyObject **p)
{
#if PY_VERSION_HEX >= 0x030C0000
    // The function was just removed from the API,
    // but still exists in the stable ABI.
    extern void PyUnicode_InternImmortal(PyObject **p);
#endif
    PyUnicode_InternImmortal(p);
}
#endif
