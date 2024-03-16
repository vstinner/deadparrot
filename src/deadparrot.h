#ifndef DEADPARROT_H
#define DEADPARROT_H
#ifdef __cplusplus
extern "C" {
#endif

#include <Python.h>

// --- Misc ------------------------------------------------------------------

// DeadPyAPI_FUNC()
#ifndef _DeadPy__has_attribute
#  define _DeadPy__has_attribute(x) __has_attribute(x)
#else
#  define _DeadPy__has_attribute(x) 0
#endif
#ifdef MS_WINDOWS
#  define DeadPyAPI_FUNC(rtype) __declspec(dllexport) rtype
#elif ((defined(__GNUC__) && (__GNUC__ >= 4)) \
       || (defined(__clang__) && _DeadPy__has_attribute(visibility)))
#  define DeadPyAPI_FUNC(rtype) __attribute__((visibility ("default"))) rtype
#else
#  define DeadPyAPI_FUNC(rtype) rtype
#endif

// _DeadPy_CAST() and _DeadPyObject_CAST()
#ifdef _Py_CAST
#  define _DeadPy_CAST(type, expr) _Py_CAST((type), (expr))
#else
#  define _DeadPy_CAST(type, expr) ((type)(expr))
#endif
#ifdef _PyObject_CAST
#  define _DeadPyObject_CAST(expr) _PyObject_CAST(expr)
#else
#  define _DeadPyObject_CAST(expr) _DeadPy_CAST(PyObject*, (expr))
#endif


// --- Object ----------------------------------------------------------------

DeadPyAPI_FUNC(PyObject*) DeadPy_NewRef(PyObject *obj);
DeadPyAPI_FUNC(PyObject*) DeadPy_XNewRef(PyObject *obj);

#if PY_VERSION_HEX < 0x030A00A3 && !defined(Py_NewRef)
#  define Py_NewRef(obj) DeadPy_NewRef(_DeadPyObject_CAST(obj))
#  define Py_XNewRef(obj) DeadPy_XNewRef(_DeadPyObject_CAST(obj))
#endif

DeadPyAPI_FUNC(void) DeadPy_SET_REFCNT(PyObject *obj, Py_ssize_t refcnt);
#if PY_VERSION_HEX < 0x030900A4 && !defined(Py_SET_REFCNT)
#  define Py_SET_REFCNT(obj, refcnt) DeadPy_SET_REFCNT(_DeadPyObject_CAST(obj), (refcnt))
#endif

DeadPyAPI_FUNC(void) DeadPy_SET_TYPE(PyObject *obj, PyTypeObject *type);
#if PY_VERSION_HEX < 0x030900A4 && !defined(Py_SET_TYPE)
#  define Py_SET_TYPE(obj, type) DeadPy_SET_TYPE(_DeadPyObject_CAST(obj), (type))
#endif

DeadPyAPI_FUNC(void) DeadPy_SET_SIZE(PyVarObject *obj, Py_ssize_t size);
#if PY_VERSION_HEX < 0x030900A4 && !defined(Py_SET_SIZE)
#define Py_SET_SIZE(obj, size) DeadPy_SET_SIZE(_DeadPy_CAST(PyVarObject*, (obj)), (size))
#endif


// --- Call ------------------------------------------------------------------

DeadPyAPI_FUNC(PyObject*) DeadPyEval_CallObject(
    PyObject *callable,
    PyObject *args);

DeadPyAPI_FUNC(PyObject*) DeadPyEval_CallObjectWithKeywords(
    PyObject *callable,
    PyObject *args,
    PyObject *kwargs);

DeadPyAPI_FUNC(PyObject*) DeadPyCFunction_Call(
    PyObject *callable,
    PyObject *args,
    PyObject *kwargs);

DeadPyAPI_FUNC(PyObject*) DeadPyEval_CallFunction(
    PyObject *callable,
    const char *format,
    ...);

DeadPyAPI_FUNC(PyObject*) DeadPyEval_CallMethod(
    PyObject *obj,
    const char *name,
    const char *format,
    ...);

#if PY_VERSION_HEX >= 0x030D0000 && !defined(DeadPy_NO_ALIAS)
#  define PyEval_CallObject DeadPyEval_CallObject
#  define PyEval_CallObjectWithKeywords DeadPyEval_CallObjectWithKeywords
#  define PyCFunction_Call DeadPyCFunction_Call
#  define PyEval_CallFunction DeadPyEval_CallFunction
#  define PyEval_CallMethod DeadPyEval_CallMethod
#endif


#if PY_VERSION_HEX >= 0x03060000
DeadPyAPI_FUNC(PyObject*) _DeadPyObject_FastCall(
    PyObject *func,
    PyObject *const *args,
    Py_ssize_t nargs);
#if PY_VERSION_HEX >= 0x030D0000 && !defined(DeadPy_NO_ALIAS)
#  define _PyObject_FastCall _DeadPyObject_FastCall
#endif
#endif


// --- Eval ------------------------------------------------------------------

DeadPyAPI_FUNC(void) DeadPyEval_InitThreads(void);

#if PY_VERSION_HEX >= 0x030D0000 && !defined(DeadPy_NO_ALIAS)
#  define PyEval_InitThreads DeadPyEval_InitThreads
#endif


// --- Float -----------------------------------------------------------------

// Removed in Python 3.9 (bpo-38835)
#ifndef PyFPE_START_PROTECT
#  define PyFPE_START_PROTECT(err_string, leave_stmt)
#  define PyFPE_END_PROTECT(v)
#endif


// --- Interpreter -----------------------------------------------------------

#if PY_VERSION_HEX >= 0x030D0000 && !defined(_PyInterpreterState_Get)
#  define _PyInterpreterState_Get PyInterpreterState_Get
#endif


// --- Unicode ---------------------------------------------------------------

#if PY_VERSION_HEX >= 0x03030000
typedef wchar_t DeadPy_UNICODE;
#else
typedef Py_UNICODE DeadPy_UNICODE;
#endif

DeadPyAPI_FUNC(DeadPy_UNICODE) DeadPyUnicode_GetMax(void);
#if PY_VERSION_HEX >= 0x030A0000 && !defined(DeadPy_NO_ALIAS)
#  define PyUnicode_GetMax DeadPyUnicode_GetMax
#endif

#if PY_VERSION_HEX >= 0x03000000
DeadPyAPI_FUNC(void) DeadPyUnicode_InternImmortal(PyObject **p);
#if PY_VERSION_HEX >= 0x030C0000 && !defined(DeadPy_NO_ALIAS)
#  define PyUnicode_InternImmortal DeadPyUnicode_InternImmortal
#endif
#endif


#ifdef __cplusplus
}
#endif
#endif  // !DEADPARROT_H
