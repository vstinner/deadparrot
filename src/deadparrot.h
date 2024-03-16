#ifndef DEADPARROT_H
#define DEADPARROT_H
#ifdef __cplusplus
extern "C" {
#endif

#include <Python.h>

// --- Misc ------------------------------------------------------------------

#define DeadPyAPI_FUNC(rtype) rtype

#ifdef _PyObject_CAST
#  define _DeadPyObject_CAST(op) _PyObject_CAST(op)
#else
#  define _DeadPyObject_CAST(op) ((PyObject*)(op))
#endif

// --- Object ----------------------------------------------------------------

DeadPyAPI_FUNC(PyObject*) DeadPy_NewRef(PyObject *obj);
DeadPyAPI_FUNC(PyObject*) DeadPy_XNewRef(PyObject *obj);

#if PY_VERSION_HEX < 0x030A00A3 && !defined(Py_NewRef)
#define Py_NewRef(obj) DeadPy_NewRef(_DeadPyObject_CAST(obj))
#define Py_XNewRef(obj) DeadPy_XNewRef(_DeadPyObject_CAST(obj))
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
