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
test_object(PyObject *Py_UNUSED(module), PyObject* Py_UNUSED(ignored))
{
    PyObject *obj = PyList_New(0);
    if (obj == _Py_NULL) {
        return _Py_NULL;
    }
    Py_ssize_t refcnt = Py_REFCNT(obj);

    // Py_NewRef()
    PyObject *ref = Py_NewRef(obj);
    assert(ref == obj);
    assert(Py_REFCNT(obj) == (refcnt + 1));
    Py_DECREF(ref);

    // Py_XNewRef()
    PyObject *xref = Py_XNewRef(obj);
    assert(xref == obj);
    assert(Py_REFCNT(obj) == (refcnt + 1));
    Py_DECREF(xref);

    // Py_XNewRef(NULL)
    assert(Py_XNewRef(_Py_NULL) == _Py_NULL);

    assert(Py_REFCNT(obj) == refcnt);
    Py_DECREF(obj);
    Py_RETURN_NONE;
}


static void
check_call_result(PyObject *res, const char *str)
{
    assert(res != NULL);
    assert(PyUnicode_Check(res));
#if PY_VERSION_HEX >= 0x03000000
    assert(PyUnicode_CompareWithASCIIString(res, str) == 0);
#else
    PyObject *obj = PyUnicode_FromString(str);
    assert(obj != NULL);
    assert(PyUnicode_Compare(res, obj) == 0);
    Py_DECREF(obj);
#endif
    assert(!PyErr_Occurred());
}


static PyObject *
test_call(PyObject *Py_UNUSED(module), PyObject *Py_UNUSED(args))
{
    PyObject *str_upper = NULL, *abc = NULL, *upper_args = NULL;
    PyObject *strip_obj = NULL, *strip_args = NULL;

    str_upper = PyObject_GetAttrString((PyObject*)&PyUnicode_Type, "upper");
    if (str_upper == NULL) {
        goto error;
    }

    abc = PyUnicode_FromString("abc");
    if (abc == NULL) {
        goto error;
    }

    upper_args = PyTuple_Pack(1, abc);
    if (upper_args == NULL) {
        goto error;
    }

    strip_obj = PyUnicode_FromString(" spaces ");
    if (strip_obj == NULL) {
        goto error;
    }

    strip_args = Py_BuildValue("(s)", " ");
    if (strip_args == NULL) {
        goto error;
    }

    // test PyEval_CallObject()
    PyObject *res = PyEval_CallObject(str_upper, upper_args);
    check_call_result(res, "ABC");
    Py_DECREF(res);

    // test PyEval_CallObjectWithKeywords()
    res = PyEval_CallObjectWithKeywords(str_upper, upper_args, NULL);
    check_call_result(res, "ABC");
    Py_DECREF(res);

    // test PyCFunction_Call()
    {
        PyObject *meth = PyObject_GetAttrString(strip_obj, "strip");
        if (meth == NULL) {
            goto error;
        }
        assert(PyCFunction_Check(meth));

        res = PyCFunction_Call(meth, strip_args, NULL);
        Py_DECREF(meth);
        check_call_result(res, "spaces");
        Py_DECREF(res);
    }

    // test PyEval_CallFunction()
    res = PyEval_CallFunction(str_upper, "(O)", abc);
    check_call_result(res, "ABC");
    Py_DECREF(res);

    // test PyEval_CallMethod()
    res = PyEval_CallMethod(strip_obj, "strip", "(s)", " ");
    check_call_result(res, "spaces");
    Py_DECREF(res);

#if PY_VERSION_HEX >= 0x03060000
    // _PyObject_FastCall()
    PyObject* fast_args[1] = {abc};
    res = _PyObject_FastCall(str_upper, fast_args, 1);
    check_call_result(res, "ABC");
    Py_DECREF(res);
#endif

    Py_DECREF(str_upper);
    Py_DECREF(abc);
    Py_DECREF(upper_args);
    Py_DECREF(strip_obj);
    Py_DECREF(strip_args);
    Py_RETURN_NONE;

error:
    Py_XDECREF(str_upper);
    Py_XDECREF(abc);
    Py_XDECREF(upper_args);
    Py_XDECREF(strip_obj);
    Py_XDECREF(strip_args);
    return NULL;
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
    PyUnicode_InternImmortal(&str);
    assert(str != NULL);
#endif

    Py_RETURN_NONE;
}


static struct PyMethodDef methods[] = {
    {"test_object", test_object, METH_NOARGS, NULL},
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
