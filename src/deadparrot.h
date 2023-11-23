#ifndef DEADPARROT_H
#define DEADPARROT_H
#ifdef __cplusplus
extern "C" {
#endif

#include <Python.h>

// --- Call ------------------------------------------------------------------

PyObject* DeadPyEval_CallObject(
    PyObject *callable,
    PyObject *args);

PyObject* DeadPyEval_CallObjectWithKeywords(
    PyObject *callable,
    PyObject *args,
    PyObject *kwargs);

PyObject* DeadPyCFunction_Call(
    PyObject *callable,
    PyObject *args,
    PyObject *kwargs);

PyObject* DeadPyEval_CallFunction(
    PyObject *callable,
    const char *format,
    ...);

PyObject* DeadPyEval_CallMethod(
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

// --- Eval ------------------------------------------------------------------

void DeadPyEval_InitThreads(void);

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

DeadPy_UNICODE DeadPyUnicode_GetMax(void);
#if PY_VERSION_HEX >= 0x030A0000 && !defined(DeadPy_NO_ALIAS)
#  define PyUnicode_GetMax DeadPyUnicode_GetMax
#endif

#if PY_VERSION_HEX >= 0x03000000
void DeadPyUnicode_InternImmortal(PyObject **p);
#if PY_VERSION_HEX >= 0x030C0000 && !defined(DeadPy_NO_ALIAS)
#  define PyUnicode_InternImmortal DeadPyUnicode_InternImmortal
#endif
#endif


#ifdef __cplusplus
}
#endif
#endif  // !DEADPARROT_H
