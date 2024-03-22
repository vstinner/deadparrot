// Always enable assertions
#undef NDEBUG

#include <Python.h>
#include "deadparrot.h"
#include "private.h"

#ifdef NDEBUG
#  error "assertions must be enabled"
#endif

#if PY_VERSION_HEX >= 0x03000000
#  define PYTHON3 1
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


// Ignore reference count checks on PyPy
#ifndef PYPY_VERSION
#  define CHECK_REFCNT
#endif

// CPython 3.12 beta 1 implements immortal objects (PEP 683)
#if 0x030C00B1 <= PY_VERSION_HEX && !defined(PYPY_VERSION)
   // Don't check reference count of Python 3.12 immortal objects (ex: bool
   // and str types)
#  define IMMORTAL_OBJS
#endif

#ifdef CHECK_REFCNT
#  define ASSERT_REFCNT(expr) assert(expr)
#else
#  define ASSERT_REFCNT(expr)
#endif


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

    // Py_SET_REFCNT
    Py_SET_REFCNT(obj, Py_REFCNT(obj));

    // Py_SET_TYPE
    Py_SET_TYPE(obj, Py_TYPE(obj));

    // Py_SET_SIZE
    Py_SET_SIZE(obj, Py_SIZE(obj));

    assert(Py_REFCNT(obj) == refcnt);
    Py_DECREF(obj);
    Py_RETURN_NONE;
}


static PyObject *
test_py_is(PyObject *Py_UNUSED(module), PyObject* Py_UNUSED(ignored))
{
    PyObject *o_none = Py_None;
    PyObject *o_true = Py_True;
    PyObject *o_false = Py_False;
    PyObject *obj = PyList_New(0);
    if (obj == _Py_NULL) {
        return _Py_NULL;
    }

    /* test Py_Is() */
    assert(Py_Is(obj, obj));
    assert(!Py_Is(obj, o_none));

    /* test Py_IsNone() */
    assert(Py_IsNone(o_none));
    assert(!Py_IsNone(obj));

    /* test Py_IsTrue() */
    assert(Py_IsTrue(o_true));
    assert(!Py_IsTrue(o_false));
    assert(!Py_IsTrue(obj));

    /* testPy_IsFalse() */
    assert(Py_IsFalse(o_false));
    assert(!Py_IsFalse(o_true));
    assert(!Py_IsFalse(obj));

    Py_DECREF(obj);
    Py_RETURN_NONE;
}


#ifndef PYPY_VERSION
static void
test_frame_getvar(PyFrameObject *frame)
{
    // Make the assumption that test_frame_getvar() is only called by
    // _run_tests() of test_pythoncapi_compat.py and so that the "name"
    // variable exists.

    // test PyFrame_GetVar() and PyFrame_GetVarString()
    PyObject *attr = PyUnicode_FromString("name");
    assert(attr != _Py_NULL);
    PyObject *name1 = PyFrame_GetVar(frame, attr);
    Py_DECREF(attr);
    assert(name1 != _Py_NULL);
    Py_DECREF(name1);

    PyObject *name2 = PyFrame_GetVarString(frame, "name");
    assert(name2 != _Py_NULL);
    Py_DECREF(name2);

    // test PyFrame_GetVar() and PyFrame_GetVarString() NameError
    PyObject *attr3 = PyUnicode_FromString("dontexist");
    assert(attr3 != _Py_NULL);
    PyObject *name3 = PyFrame_GetVar(frame, attr3);
    Py_DECREF(attr3);
    assert(name3 == _Py_NULL);
    assert(PyErr_ExceptionMatches(PyExc_NameError));
    PyErr_Clear();

    PyObject *name4 = PyFrame_GetVarString(frame, "dontexist");
    assert(name4 == _Py_NULL);
    assert(PyErr_ExceptionMatches(PyExc_NameError));
    PyErr_Clear();
}

static PyObject *
test_frame(PyObject *Py_UNUSED(module), PyObject* Py_UNUSED(ignored))
{
    PyThreadState *tstate = PyThreadState_Get();
    (void)tstate;

    // test PyThreadState_GetFrame()
    PyFrameObject *frame = PyThreadState_GetFrame(tstate);
    if (frame == _Py_NULL) {
        PyErr_SetString(PyExc_AssertionError, "PyThreadState_GetFrame failed");
        return _Py_NULL;
    }

    // test PyFrame_GetCode()
    {
        PyCodeObject *code = PyFrame_GetCode(frame);
        assert(code != _Py_NULL);
        assert(PyCode_Check(code));
        Py_DECREF(code);
    }

    // PyFrame_GetBack()
    {
        PyFrameObject* back = PyFrame_GetBack(frame);
        if (back != _Py_NULL) {
            assert(PyFrame_Check(back));
        }
        Py_XDECREF(back);
    }

    // test PyFrame_GetLocals()
    {
        PyObject *locals = PyFrame_GetLocals(frame);
        assert(locals != _Py_NULL);
        assert(PyDict_Check(locals));

        // test PyFrame_GetGlobals()
        PyObject *globals = PyFrame_GetGlobals(frame);
        assert(globals != _Py_NULL);
        assert(PyDict_Check(globals));

        // test PyFrame_GetBuiltins()
        PyObject *builtins = PyFrame_GetBuiltins(frame);
        assert(builtins != _Py_NULL);
        assert(PyDict_Check(builtins));

        assert(locals != globals);
        assert(globals != builtins);
        assert(builtins != locals);

        Py_DECREF(locals);
        Py_DECREF(globals);
        Py_DECREF(builtins);
    }

    // test PyFrame_GetLasti()
    int lasti = PyFrame_GetLasti(frame);
    assert(lasti >= 0);

    // test PyFrame_GetVar() and PyFrame_GetVarString()
    test_frame_getvar(frame);

    // done
    Py_DECREF(frame);
    Py_RETURN_NONE;
}
#endif  // !defined(PYPY_VERSION)


static PyObject *
test_thread_state(PyObject *Py_UNUSED(module), PyObject* Py_UNUSED(ignored))
{
    PyThreadState *tstate = PyThreadState_Get();

    // test PyThreadState_GetInterpreter()
    PyInterpreterState *interp = PyThreadState_GetInterpreter(tstate);
    assert(interp != _Py_NULL);

#ifndef PYPY_VERSION
    // test PyThreadState_GetFrame()
    PyFrameObject *frame = PyThreadState_GetFrame(tstate);
    if (frame != _Py_NULL) {
        assert(PyFrame_Check(frame));
    }
    Py_XDECREF(frame);
#endif

#if 0x030700A1 <= PY_VERSION_HEX && !defined(PYPY_VERSION)
    uint64_t id = PyThreadState_GetID(tstate);
    assert(id > 0);
#endif

#ifndef PYPY_VERSION
    // PyThreadState_EnterTracing(), PyThreadState_LeaveTracing()
    PyThreadState_EnterTracing(tstate);
    PyThreadState_LeaveTracing(tstate);
#endif

    Py_RETURN_NONE;
}


static PyObject *
test_interpreter(PyObject *Py_UNUSED(module), PyObject* Py_UNUSED(ignored))
{
    // test PyInterpreterState_Get()
    PyInterpreterState *interp = PyInterpreterState_Get();
    assert(interp != _Py_NULL);
    PyThreadState *tstate = PyThreadState_Get();
    PyInterpreterState *interp2 = PyThreadState_GetInterpreter(tstate);
    assert(interp == interp2);

    // test _PyInterpreterState_Get()
    PyInterpreterState *interp3 = _PyInterpreterState_Get();
    assert(interp3 == interp);

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

    // test PyObject_CallNoArgs(): str() returns ''
    {
        PyObject *func = _DeadPy_CAST(PyObject*, &PyUnicode_Type);
        PyObject *res = PyObject_CallNoArgs(func);
        if (res == NULL) {
            return NULL;
        }
        assert(PyUnicode_Check(res));
        Py_DECREF(res);
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
#ifndef PYPY_VERSION
        assert(PyCFunction_Check(meth));
#endif

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

    // _PyObject_FastCall()
    PyObject* fast_args[1] = {abc};
    res = _PyObject_FastCall(str_upper, fast_args, 1);
    check_call_result(res, "ABC");
    Py_DECREF(res);

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

#if PY_VERSION_HEX >= 0x03000000 && !defined(PYPY_VERSION)
    PyObject *str = PyUnicode_FromString("abc");
    if (str == NULL) {
        return NULL;
    }
    PyUnicode_InternImmortal(&str);
    assert(str != NULL);
#endif

    Py_RETURN_NONE;
}


static int
check_module_attr(PyObject *module, const char *name, PyObject *expected)
{
    PyObject *attr = PyObject_GetAttrString(module, name);
    if (attr == _Py_NULL) {
        return -1;
    }
    assert(attr == expected);
    Py_DECREF(attr);

    if (PyObject_DelAttrString(module, name) < 0) {
        return -1;
    }
    return 0;
}


// test PyModule_AddType()
static int
test_module_add_type(PyObject *module)
{
    PyTypeObject *type = &PyUnicode_Type;
#ifdef PYTHON3
    const char *type_name = "str";
#else
    const char *type_name = "unicode";
#endif
#ifdef CHECK_REFCNT
    Py_ssize_t refcnt = Py_REFCNT(type);
#endif

    if (PyModule_AddType(module, type) < 0) {
        return -1;
    }
#ifndef IMMORTAL_OBJS
    ASSERT_REFCNT(Py_REFCNT(type) == refcnt + 1);
#endif

    if (check_module_attr(module, type_name, _PyObject_CAST(type)) < 0) {
        return -1;
    }
    ASSERT_REFCNT(Py_REFCNT(type) == refcnt);
    return 0;
}


// test PyModule_AddObjectRef()
static int
test_module_addobjectref(PyObject *module)
{
    const char *name = "test_module_addobjectref";
    PyObject *obj = PyUnicode_FromString(name);
    assert(obj != _Py_NULL);
#ifdef CHECK_REFCNT
    Py_ssize_t refcnt = Py_REFCNT(obj);
#endif

    if (PyModule_AddObjectRef(module, name, obj) < 0) {
        ASSERT_REFCNT(Py_REFCNT(obj) == refcnt);
        Py_DECREF(obj);
        return -1;
    }
    ASSERT_REFCNT(Py_REFCNT(obj) == refcnt + 1);

    if (check_module_attr(module, name, obj) < 0) {
        Py_DECREF(obj);
        return -1;
    }
    ASSERT_REFCNT(Py_REFCNT(obj) == refcnt);

    // PyModule_AddObjectRef() with value=NULL must not crash
    assert(!PyErr_Occurred());
    int res = PyModule_AddObjectRef(module, name, _Py_NULL);
    assert(res < 0);
    assert(PyErr_ExceptionMatches(PyExc_SystemError));
    PyErr_Clear();

    Py_DECREF(obj);
    return 0;
}


// test PyModule_Add()
static int
test_module_add(PyObject *module)
{
    const char *name = "test_module_add";
    PyObject *obj = PyUnicode_FromString(name);
    assert(obj != _Py_NULL);
#ifdef CHECK_REFCNT
    Py_ssize_t refcnt = Py_REFCNT(obj);
#endif

    if (PyModule_Add(module, name, Py_NewRef(obj)) < 0) {
        ASSERT_REFCNT(Py_REFCNT(obj) == refcnt);
        Py_DECREF(obj);
        return -1;
    }
    ASSERT_REFCNT(Py_REFCNT(obj) == refcnt + 1);

    if (check_module_attr(module, name, obj) < 0) {
        Py_DECREF(obj);
        return -1;
    }
    ASSERT_REFCNT(Py_REFCNT(obj) == refcnt);

    // PyModule_Add() with value=NULL must not crash
    assert(!PyErr_Occurred());
    int res = PyModule_Add(module, name, _Py_NULL);
    assert(res < 0);
    assert(PyErr_ExceptionMatches(PyExc_SystemError));
    PyErr_Clear();

    Py_DECREF(obj);
    return 0;
}


static PyObject *
test_module(PyObject *Py_UNUSED(module), PyObject* Py_UNUSED(ignored))
{
    PyObject *module = PyImport_ImportModule("sys");
    if (module == _Py_NULL) {
        return _Py_NULL;
    }
    assert(PyModule_Check(module));

    if (test_module_add_type(module) < 0) {
        goto error;
    }
    if (test_module_addobjectref(module) < 0) {
        goto error;
    }
    if (test_module_add(module) < 0) {
        goto error;
    }

    Py_DECREF(module);
    Py_RETURN_NONE;

error:
    Py_DECREF(module);
    return _Py_NULL;
}


#ifndef PYPY_VERSION
static PyObject *
test_gc(PyObject *Py_UNUSED(module), PyObject* Py_UNUSED(ignored))
{
    PyObject *tuple = PyTuple_New(1);
    Py_INCREF(Py_None);
    PyTuple_SET_ITEM(tuple, 0, Py_None);

    // test PyObject_GC_IsTracked()
    int tracked = PyObject_GC_IsTracked(tuple);
    assert(tracked);

#if PY_VERSION_HEX >= 0x030400F0
    // test PyObject_GC_IsFinalized()
    int finalized = PyObject_GC_IsFinalized(tuple);
    assert(!finalized);
#endif

    Py_DECREF(tuple);
    Py_RETURN_NONE;
}
#endif


static struct PyMethodDef methods[] = {
    {"test_object", test_object, METH_NOARGS, NULL},
    {"test_py_is", test_py_is, METH_NOARGS, _Py_NULL},
#ifndef PYPY_VERSION
    {"test_frame", test_frame, METH_NOARGS, _Py_NULL},
#endif
    {"test_thread_state", test_thread_state, METH_NOARGS, _Py_NULL},
    {"test_interpreter", test_interpreter, METH_NOARGS, _Py_NULL},
    {"test_call", test_call, METH_NOARGS, NULL},
    {"test_eval", test_eval, METH_NOARGS, NULL},
    {"test_unicode", test_unicode, METH_NOARGS, NULL},
    {"test_module", test_module, METH_NOARGS, _Py_NULL},
#ifndef PYPY_VERSION
    {"test_gc", test_gc, METH_NOARGS, _Py_NULL},
#endif
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
