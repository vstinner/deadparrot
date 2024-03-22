#define DeadPy_NO_ALIAS
#include "deadparrot.h"
#include "private.h"

#ifndef PYPY_VERSION

#if 0x030600B1 <= PY_VERSION_HEX
int DeadPyFloat_Pack2(double x, char *p, int le)
{
#if PY_VERSION_HEX >= 0x030B00A1
    return PyFloat_Pack2(x, p, le);
#else
    return _PyFloat_Pack2(x, (unsigned char*)p, le);
#endif
}

double DeadPyFloat_Unpack2(const char *p, int le)
{
#if PY_VERSION_HEX >= 0x030B00A1
    return PyFloat_Unpack2(p, le);
#else
    return _PyFloat_Unpack2((const unsigned char *)p, le);
#endif
}
#endif  // 0x030600B1 <= PY_VERSION_HEX


int DeadPyFloat_Pack4(double x, char *p, int le)
{
#if PY_VERSION_HEX >= 0x030B00A1
    return PyFloat_Pack4(x, p, le);
#else
    return _PyFloat_Pack4(x, (unsigned char*)p, le);
#endif
}

int DeadPyFloat_Pack8(double x, char *p, int le)
{
#if PY_VERSION_HEX >= 0x030B00A1
    return PyFloat_Pack8(x, p, le);
#else
    return _PyFloat_Pack8(x, (unsigned char*)p, le);
#endif
}

double DeadPyFloat_Unpack4(const char *p, int le)
{
#if PY_VERSION_HEX >= 0x030B00A1
    return PyFloat_Unpack4(p, le);
#else
    return _PyFloat_Unpack4((const unsigned char *)p, le);
#endif
}

double DeadPyFloat_Unpack8(const char *p, int le)
{
#if PY_VERSION_HEX >= 0x030B00A1
    return PyFloat_Unpack8(p, le);
#else
    return _PyFloat_Unpack8((const unsigned char *)p, le);
#endif
}


#endif  // !PYPY_VERSION
