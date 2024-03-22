#ifndef DEADPARROT_H
#define DEADPARROT_H

#include <Python.h>
#include <frameobject.h>

#ifdef __cplusplus
extern "C" {
#endif

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
#  define _DeadPy_CAST(type, expr) _Py_CAST(type, expr)
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
#  define Py_SET_SIZE(obj, size) DeadPy_SET_SIZE(_DeadPy_CAST(PyVarObject*, (obj)), (size))
#endif

DeadPyAPI_FUNC(int) DeadPy_Is(PyObject *x, PyObject *y);
#if PY_VERSION_HEX < 0x030A00B1 && !defined(Py_Is)
#  define Py_Is(x, y) DeadPy_Is((x), (y))
#endif

DeadPyAPI_FUNC(int) DeadPy_IsNone(PyObject *x);
#if PY_VERSION_HEX < 0x030A00B1 && !defined(Py_IsNone)
#  define Py_IsNone(x) DeadPy_Is((x), Py_None)
#endif

DeadPyAPI_FUNC(int) DeadPy_IsFalse(PyObject *x);
#if (PY_VERSION_HEX < 0x030A00B1 || defined(PYPY_VERSION)) && !defined(Py_IsFalse)
#  define Py_IsFalse(x) DeadPy_Is((x), Py_False)
#endif

DeadPyAPI_FUNC(int) DeadPy_IsTrue(PyObject *x);
#if (PY_VERSION_HEX < 0x030A00B1 || defined(PYPY_VERSION)) && !defined(Py_IsTrue)
#  define Py_IsTrue(x) DeadPy_Is((x), Py_True)
#endif


// --- Call ------------------------------------------------------------------

DeadPyAPI_FUNC(PyObject*) DeadPyObject_CallNoArgs(PyObject *func);
#if PY_VERSION_HEX < 0x030900A1 && !defined(DeadPy_NO_ALIAS)
#  define PyObject_CallNoArgs DeadPyObject_CallNoArgs
#endif

DeadPyAPI_FUNC(PyObject*) DeadPyObject_CallOneArg(PyObject *func, PyObject *arg);
#if !defined(PyObject_CallOneArg) && PY_VERSION_HEX < 0x030900A4
#  define PyObject_CallOneArg DeadPyObject_CallOneArg
#endif

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


DeadPyAPI_FUNC(PyObject*) _DeadPyObject_FastCall(
    PyObject *func,
    PyObject *const *args,
    Py_ssize_t nargs);
#if (!defined(DeadPy_NO_ALIAS) && !defined(_PyObject_FastCall) \
     && (PY_VERSION_HEX < 0x03060000 \
         || 0x030D0000 <= PY_VERSION_HEX \
         || defined(PYPY_VERSION) && PY_VERSION_HEX < 0x03090000))
#  define _PyObject_FastCall _DeadPyObject_FastCall
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


// --- PyInterpreterState ----------------------------------------------------

DeadPyAPI_FUNC(PyInterpreterState*) DeadPyInterpreterState_Get(void);
#if PY_VERSION_HEX < 0x030900A5 || defined(PYPY_VERSION) && !defined(DeadPy_NO_ALIAS)
#  define PyInterpreterState_Get DeadPyInterpreterState_Get
#endif

#if ((PY_VERSION_HEX < 0x03080000 || 0x030D0000 <= PY_VERSION_HEX \
      || defined(PYPY_VERSION)) \
     && !defined(_PyInterpreterState_Get))
#  define _PyInterpreterState_Get PyInterpreterState_Get
#endif


// --- PyThreadState ---------------------------------------------------------

DeadPyAPI_FUNC(PyInterpreterState*) DeadPyThreadState_GetInterpreter(PyThreadState *tstate);
#if (PY_VERSION_HEX < 0x030900A5 || defined(PYPY_VERSION)) && !defined(DeadPy_NO_ALIAS)
#  define PyThreadState_GetInterpreter DeadPyThreadState_GetInterpreter
#endif

#ifndef PYPY_VERSION
   DeadPyAPI_FUNC(PyFrameObject*) DeadPyThreadState_GetFrame(PyThreadState *tstate);
#  if (PY_VERSION_HEX < 0x030900B1 && !defined(PYPY_VERSION)) && !defined(DeadPy_NO_ALIAS)
#    define PyThreadState_GetFrame DeadPyThreadState_GetFrame
#  endif
#endif

#if 0x030700A1 <= PY_VERSION_HEX && !defined(PYPY_VERSION)
   DeadPyAPI_FUNC(uint64_t) DeadPyThreadState_GetID(PyThreadState *tstate);
#  if PY_VERSION_HEX < 0x030900A6 && !defined(DeadPy_NO_ALIAS)
#    define PyThreadState_GetID DeadPyThreadState_GetID
#  endif
#endif

#ifndef PYPY_VERSION
   DeadPyAPI_FUNC(void) DeadPyThreadState_EnterTracing(PyThreadState *tstate);
   DeadPyAPI_FUNC(void) DeadPyThreadState_LeaveTracing(PyThreadState *tstate);

#  if PY_VERSION_HEX < 0x030B00A2 && !defined(PYPY_VERSION) && !defined(DeadPy_NO_ALIAS)
#    define PyThreadState_EnterTracing DeadPyThreadState_EnterTracing
#    define PyThreadState_LeaveTracing DeadPyThreadState_LeaveTracing
#  endif
#endif


// --- Unicode ---------------------------------------------------------------

#if PY_VERSION_HEX >= 0x03030000
typedef wchar_t DeadPy_UNICODE;
#else
typedef Py_UNICODE DeadPy_UNICODE;
#endif

DeadPyAPI_FUNC(DeadPy_UNICODE) DeadPyUnicode_GetMax(void);
#if PY_VERSION_HEX >= 0x030A0000 && !defined(PyUnicode_GetMax) && !defined(DeadPy_NO_ALIAS)
#  define PyUnicode_GetMax DeadPyUnicode_GetMax
#endif

#if PY_VERSION_HEX >= 0x03000000 && !defined(PYPY_VERSION)
   DeadPyAPI_FUNC(void) DeadPyUnicode_InternImmortal(PyObject **p);
#  if (PY_VERSION_HEX >= 0x030C0000 || defined(PYPY_VERSION)) && !defined(DeadPy_NO_ALIAS)
#    define PyUnicode_InternImmortal DeadPyUnicode_InternImmortal
#  endif
#endif


// --- Frame -----------------------------------------------------------------

#ifndef PYPY_VERSION
   DeadPyAPI_FUNC(PyCodeObject*) DeadPyFrame_GetCode(PyFrameObject *frame);
#  if PY_VERSION_HEX < 0x030900B1 || defined(PYPY_VERSION) && !defined(DeadPy_NO_ALIAS)
#    define PyFrame_GetCode DeadPyFrame_GetCode
#  endif
#endif

#ifndef PYPY_VERSION
   DeadPyAPI_FUNC(PyFrameObject*) DeadPyFrame_GetBack(PyFrameObject *frame);
#  if PY_VERSION_HEX < 0x030900B1 && !defined(PYPY_VERSION) && !defined(DeadPy_NO_ALIAS)
#    define PyFrame_GetBack DeadPyFrame_GetBack
#  endif
#endif

#ifndef PYPY_VERSION
   DeadPyAPI_FUNC(PyObject*) DeadPyFrame_GetLocals(PyFrameObject *frame);
#  if PY_VERSION_HEX < 0x030B00A7 && !defined(PYPY_VERSION) && !defined(DeadPy_NO_ALIAS)
#    define PyFrame_GetLocals DeadPyFrame_GetLocals
#  endif
#endif

DeadPyAPI_FUNC(PyObject*) DeadPyFrame_GetGlobals(PyFrameObject *frame);
#if PY_VERSION_HEX < 0x030B00A7 && !defined(PYPY_VERSION) && !defined(DeadPy_NO_ALIAS)
#  define PyFrame_GetGlobals DeadPyFrame_GetGlobals
#endif

#ifndef PYPY_VERSION
   DeadPyAPI_FUNC(PyObject*) DeadPyFrame_GetBuiltins(PyFrameObject *frame);
#  if PY_VERSION_HEX < 0x030B00A7 && !defined(PYPY_VERSION) && !defined(DeadPy_NO_ALIAS)
#    define PyFrame_GetBuiltins DeadPyFrame_GetBuiltins
#  endif
#endif

#ifndef PYPY_VERSION
   DeadPyAPI_FUNC(int) DeadPyFrame_GetLasti(PyFrameObject *frame);
#  if PY_VERSION_HEX < 0x030B00B1 && !defined(PYPY_VERSION) && !defined(DeadPy_NO_ALIAS)
#    define PyFrame_GetLasti DeadPyFrame_GetLasti
#  endif
#endif

#ifndef PYPY_VERSION
   DeadPyAPI_FUNC(PyObject*) DeadPyFrame_GetVar(PyFrameObject *frame, PyObject *name);
#  if PY_VERSION_HEX < 0x030C00A2 && !defined(PYPY_VERSION) && !defined(DeadPy_NO_ALIAS)
#    define PyFrame_GetVar DeadPyFrame_GetVar
#  endif

   DeadPyAPI_FUNC(PyObject*) DeadPyFrame_GetVarString(PyFrameObject *frame, const char *name);
#  if PY_VERSION_HEX < 0x030C00A2 && !defined(PYPY_VERSION) && !defined(DeadPy_NO_ALIAS)
#    define PyFrame_GetVarString DeadPyFrame_GetVarString
#  endif
#endif


// --- Module ----------------------------------------------------------------

DeadPyAPI_FUNC(int) DeadPyModule_AddObjectRef(PyObject *module, const char *name, PyObject *value);
#if PY_VERSION_HEX < 0x030A00A3 && !defined(DeadPy_NO_ALIAS)
#  define PyModule_AddObjectRef DeadPyModule_AddObjectRef
#endif

DeadPyAPI_FUNC(int) DeadPyModule_AddType(PyObject *module, PyTypeObject *type);
#if PY_VERSION_HEX < 0x030900A5 && !defined(DeadPy_NO_ALIAS)
#  define PyModule_AddType DeadPyModule_AddType
#endif

DeadPyAPI_FUNC(int) DeadPyModule_Add(PyObject *mod, const char *name, PyObject *value);
#if PY_VERSION_HEX < 0x030D00A1 && !defined(DeadPy_NO_ALIAS)
#  define PyModule_Add DeadPyModule_Add
#endif


#ifdef __cplusplus
}
#endif
#endif  // !DEADPARROT_H
