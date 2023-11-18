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

#if PY_VERSION_HEX >= 0x030D00A1 && !defined(DeadPy_NO_ALIAS)
#  define PyEval_CallObject DeadPyEval_CallObject
#  define PyEval_CallObjectWithKeywords DeadPyEval_CallObjectWithKeywords
#endif

// --- Eval ------------------------------------------------------------------

void DeadPyEval_InitThreads(void);

#if PY_VERSION_HEX >= 0x030D00A1 && !defined(DeadPy_NO_ALIAS)
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

void DeadPyUnicode_InternImmortal(PyObject **p);
#if PY_VERSION_HEX >= 0x030D00A1 && !defined(DeadPy_NO_ALIAS)
#  define PyUnicode_InternImmortal DeadPyUnicode_InternImmortal
#endif


#ifdef __cplusplus
}
#endif
#endif  // !DEADPARROT_H
