#define DeadPy_NO_ALIAS
#include "deadparrot.h"
#include "private.h"


Py_hash_t DeadPy_HashPointer(const void *ptr)
{
#if PY_VERSION_HEX >= 0x030D00A3
    return Py_HashPointer(ptr);
#else
#  if PY_VERSION_HEX >= 0x030900A4 && !defined(PYPY_VERSION)
    return _Py_HashPointer(ptr);
#  else
    return _Py_HashPointer(_Py_CAST(void*, ptr));
#  endif
#endif
}
