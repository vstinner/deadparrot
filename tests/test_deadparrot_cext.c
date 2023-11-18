// Always enable assertions
#undef NDEBUG

#include <Python.h>
#include "deadparrot.h"
#include "private.h"

#ifdef NDEBUG
#  error "assertions must be enabled"
#endif

#ifndef Py_UNUSED
#  if defined(__GNUC__) || defined(__clang__)
#    define Py_UNUSED(name) _unused_ ## name __attribute__((unused))
#  else
#    define Py_UNUSED(name) _unused_ ## name
#  endif
#endif

// Ignore deprecation warnings of Py_DEPRECATED()
IGNORE_DEPR_WARNINGS


static PyObject *
test_call(PyObject *Py_UNUSED(module), PyObject *Py_UNUSED(args))
{
    PyObject *func = (PyObject*)&PyUnicode_Type;
    PyObject *res;

    // test PyEval_CallObject()
    res = PyEval_CallObject(func, NULL);
    assert(res != NULL);
    Py_DECREF(res);

    // test PyEval_CallObjectWithKeywords()
    res = PyEval_CallObjectWithKeywords(func, NULL, NULL);
    assert(res != NULL);
    Py_DECREF(res);

    Py_RETURN_NONE;
}


static PyObject *
test_interp(PyObject *Py_UNUSED(module), PyObject *Py_UNUSED(args))
{
#if PY_VERSION_HEX >= 0x03080000
    PyInterpreterState *interp = _PyInterpreterState_Get();
    assert(interp != NULL);
#endif

    Py_RETURN_NONE;
}


static PyObject *
test_eval(PyObject *Py_UNUSED(module), PyObject *Py_UNUSED(args))
{
    // test PyEval_InitThreads()
    PyEval_InitThreads();

    Py_RETURN_NONE;
}


static PyObject *
test_unicode(PyObject *Py_UNUSED(module), PyObject *Py_UNUSED(args))
{
    // test PyUnicode_GetMax()
    DeadPy_UNICODE unicode_max = PyUnicode_GetMax();
    assert(unicode_max == 0x10ffff || unicode_max == 0xffff);

#if PY_VERSION_HEX >= 0x03000000
    PyObject *str = PyUnicode_FromString("abc");
    if (str == NULL) {
        return NULL;
    }
    DeadPyUnicode_InternImmortal(&str);
    assert(str != NULL);
#endif

    Py_RETURN_NONE;
}


static struct PyMethodDef methods[] = {
    {"test_call", test_call, METH_NOARGS, NULL},
    {"test_eval", test_eval, METH_NOARGS, NULL},
    {"test_interp", test_interp, METH_NOARGS, NULL},
    {"test_unicode", test_unicode, METH_NOARGS, NULL},
    {NULL, NULL, 0, NULL}
};


static int
module_exec(PyObject *Py_UNUSED(module))
{
    return 0;
}


#if PY_VERSION_HEX >= 0x03050000
static PyModuleDef_Slot module_slots[] = {
    {Py_mod_exec, module_exec},
    {0, NULL}
};
#endif


#if PY_VERSION_HEX >= 0x03000000
static struct PyModuleDef module_def = {
    PyModuleDef_HEAD_INIT,
    "test_deadparrot_cext",  // m_name
    NULL,                    // m_doc
    0,                       // m_size
    methods,                 // m_methods
    module_slots,            // m_slots
    NULL,                    // m_traverse
    NULL,                    // m_clear
    NULL,                    // m_free
};


PyMODINIT_FUNC
PyInit_test_deadparrot_cext(void)
{
    return PyModuleDef_Init(&module_def);
}

#else
// Python 2

PyMODINIT_FUNC
inittest_deadparrot_cext(void)
{
    PyObject *module;
    module = Py_InitModule4("test_deadparrot_cext",
                            methods,
                            NULL,
                            NULL,
                            PYTHON_API_VERSION);
    if (module == NULL) {
        return;
    }

    if (module_exec(module) < 0) {
        return;
    }
}
#endif
