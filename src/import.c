#define DeadPy_NO_ALIAS
#include "deadparrot.h"
#include "private.h"


PyObject* DeadPyImport_AddModuleRef(const char *name)
{
#if PY_VERSION_HEX >= 0x030D00A0
    return PyImport_AddModuleRef(name);
#else
    return Py_XNewRef(PyImport_AddModule(name));
#endif
}
