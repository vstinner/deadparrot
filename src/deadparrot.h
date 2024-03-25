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

DeadPyAPI_FUNC(int) DeadPy_IS_TYPE(PyObject *ob, PyTypeObject *type);
#if PY_VERSION_HEX < 0x030900A4 && !defined(Py_IS_TYPE) && !defined(DeadPy_NO_ALIAS)
#  define Py_IS_TYPE DeadPy_IS_TYPE
#endif


#if PY_VERSION_HEX < 0x030D00A6
#  define Py_CONSTANT_NONE 0
#  define Py_CONSTANT_FALSE 1
#  define Py_CONSTANT_TRUE 2
#  define Py_CONSTANT_ELLIPSIS 3
#  define Py_CONSTANT_NOT_IMPLEMENTED 4
#  define Py_CONSTANT_ZERO 5
#  define Py_CONSTANT_ONE 6
#  define Py_CONSTANT_EMPTY_STR 7
#  define Py_CONSTANT_EMPTY_BYTES 8
#  define Py_CONSTANT_EMPTY_TUPLE 9
#endif

DeadPyAPI_FUNC(PyObject*) DeadPy_GetConstant(unsigned int constant_id);
DeadPyAPI_FUNC(PyObject*) DeadPy_GetConstantBorrowed(unsigned int constant_id);
#if PY_VERSION_HEX < 0x030D00A6 && !defined(DeadPy_NO_ALIAS)
#  define Py_GetConstant DeadPy_GetConstant
#  define Py_GetConstantBorrowed DeadPy_GetConstantBorrowed
#endif

DeadPyAPI_FUNC(int) DeadPyObject_VisitManagedDict(PyObject *obj, visitproc visit, void *arg);
DeadPyAPI_FUNC(void) DeadPyObject_ClearManagedDict(PyObject *obj);
#if PY_VERSION_HEX < 0x030D00A1 && !defined(DeadPy_NO_ALIAS)
#  define PyObject_VisitManagedDict DeadPyObject_VisitManagedDict
#  define PyObject_ClearManagedDict DeadPyObject_ClearManagedDict
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

DeadPyAPI_FUNC(Py_ssize_t) DeadPyVectorcall_NARGS(size_t n);
#if PY_VERSION_HEX < 0x030800B1 && !defined(DeadPy_NO_ALIAS)
#  define PyVectorcall_NARGS DeadPyVectorcall_NARGS
#endif

DeadPyAPI_FUNC(PyObject*) DeadPyObject_Vectorcall(
    PyObject *callable, PyObject *const *args,
    size_t nargsf, PyObject *kwnames);
#if PY_VERSION_HEX < 0x030900A4 && !defined(DeadPy_NO_ALIAS)
#  define PyObject_Vectorcall DeadPyObject_Vectorcall
#endif


// --- Code ------------------------------------------------------------------

#ifndef PYPY_VERSION
   DeadPyAPI_FUNC(PyObject*) DeadPyCode_GetCode(PyCodeObject *code);
   DeadPyAPI_FUNC(PyObject*) DeadPyCode_GetVarnames(PyCodeObject *code);
   DeadPyAPI_FUNC(PyObject*) DeadPyCode_GetFreevars(PyCodeObject *code);
   DeadPyAPI_FUNC(PyObject*) DeadPyCode_GetCellvars(PyCodeObject *code);
#  if PY_VERSION_HEX < 0x030B0000 && !defined(DeadPy_NO_ALIAS)
#    define PyCode_GetCode DeadPyCode_GetCode
#    define PyCode_GetVarnames DeadPyCode_GetVarnames
#    define PyCode_GetFreevars DeadPyCode_GetFreevars
#    define PyCode_GetCellvars DeadPyCode_GetCellvars
#  endif
#endif

// --- Dict ------------------------------------------------------------------

DeadPyAPI_FUNC(int) DeadPyDict_GetItemRef(PyObject *mp, PyObject *key, PyObject **result);
DeadPyAPI_FUNC(int) DeadPyDict_GetItemStringRef(PyObject *mp, const char *key, PyObject **result);
#if PY_VERSION_HEX < 0x030D00A1 && !defined(DeadPy_NO_ALIAS)
#  define PyDict_GetItemRef DeadPyDict_GetItemRef
#  define PyDict_GetItemStringRef DeadPyDict_GetItemStringRef
#endif

DeadPyAPI_FUNC(int) DeadPyDict_ContainsString(PyObject *op, const char *key);
#if PY_VERSION_HEX < 0x030D00A1 && !defined(DeadPy_NO_ALIAS)
#  define PyDict_ContainsString DeadPyDict_ContainsString
#endif

DeadPyAPI_FUNC(int) DeadPyDict_Pop(PyObject *dict, PyObject *key, PyObject **result);
DeadPyAPI_FUNC(int) DeadPyDict_PopString(PyObject *dict, const char *key, PyObject **result);
#if PY_VERSION_HEX < 0x030D00A2 && !defined(DeadPy_NO_ALIAS)
#  define PyDict_Pop DeadPyDict_Pop
#  define PyDict_PopString DeadPyDict_PopString
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

#ifndef PYPY_VERSION
   DeadPyAPI_FUNC(int) DeadPyFloat_Pack2(double x, char *p, int le);
   DeadPyAPI_FUNC(double) DeadPyFloat_Unpack2(const char *p, int le);
#  if 0x030600B1 <= PY_VERSION_HEX && PY_VERSION_HEX <= 0x030B00A1 && !defined(DeadPy_NO_ALIAS)
#    define PyFloat_Pack2 DeadPyFloat_Pack2
#    define PyFloat_Unpack2 DeadPyFloat_Unpack2
#  endif

   DeadPyAPI_FUNC(int) DeadPyFloat_Pack4(double x, char *p, int le);
   DeadPyAPI_FUNC(int) DeadPyFloat_Pack8(double x, char *p, int le);
   DeadPyAPI_FUNC(double) DeadPyFloat_Unpack4(const char *p, int le);
   DeadPyAPI_FUNC(double) DeadPyFloat_Unpack8(const char *p, int le);
#  if PY_VERSION_HEX <= 0x030B00A1 && !defined(DeadPy_NO_ALIAS)
#    define PyFloat_Pack4 DeadPyFloat_Pack4
#    define PyFloat_Pack8 DeadPyFloat_Pack8
#    define PyFloat_Unpack4 DeadPyFloat_Unpack4
#    define PyFloat_Unpack8 DeadPyFloat_Unpack8
#  endif
#endif


// --- GC --------------------------------------------------------------------

#ifndef PYPY_VERSION
   DeadPyAPI_FUNC(int) DeadPyObject_GC_IsTracked(PyObject* obj);
#  if PY_VERSION_HEX < 0x030900A6 && !defined(DeadPy_NO_ALIAS)
#    define PyObject_GC_IsTracked DeadPyObject_GC_IsTracked
#  endif

#  if PY_VERSION_HEX >= 0x030400F0
     DeadPyAPI_FUNC(int) DeadPyObject_GC_IsFinalized(PyObject *obj);
#    if PY_VERSION_HEX < 0x030900A6 && !defined(DeadPy_NO_ALIAS)
#      define PyObject_GC_IsFinalized DeadPyObject_GC_IsFinalized
#    endif
#  endif

#endif


// --- Hash ------------------------------------------------------------------

#if PY_VERSION_HEX < 0x030200A4
// Python 3.2.0a4 added Py_hash_t type
typedef Py_ssize_t Py_hash_t;
#endif

DeadPyAPI_FUNC(Py_hash_t) DeadPy_HashPointer(const void *ptr);
#if PY_VERSION_HEX < 0x030D00A3 && !defined(DeadPy_NO_ALIAS)
#  define Py_HashPointer DeadPy_HashPointer
#endif


// gh-111389 added hash constants to Python 3.13.0a5. These constants were
// added first as private macros to Python 3.4.0b1 and PyPy 7.3.9.
#if (!defined(PyHASH_BITS) \
     && ((!defined(PYPY_VERSION) && PY_VERSION_HEX >= 0x030400B1) \
         || (defined(PYPY_VERSION) && PY_VERSION_HEX >= 0x03070000 \
             && PYPY_VERSION_NUM >= 0x07090000)))
#  define PyHASH_BITS _PyHASH_BITS
#  define PyHASH_MODULUS _PyHASH_MODULUS
#  define PyHASH_INF _PyHASH_INF
#  define PyHASH_IMAG _PyHASH_IMAG
#endif


// --- List ------------------------------------------------------------------

DeadPyAPI_FUNC(int) DeadPyList_Extend(PyObject *list, PyObject *iterable);
DeadPyAPI_FUNC(int) DeadPyList_Clear(PyObject *list);
#if PY_VERSION_HEX < 0x030D00A2 && !defined(DeadPy_NO_ALIAS)
#  define PyList_Extend DeadPyList_Extend
#  define PyList_Clear DeadPyList_Clear
#endif

// --- Long ------------------------------------------------------------------

DeadPyAPI_FUNC(int) DeadPyLong_AsInt(PyObject *obj);
#if PY_VERSION_HEX < 0x030D00A1 && !defined(DeadPy_NO_ALIAS)
#  define PyLong_AsInt DeadPyLong_AsInt
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

#if 0x03020000 <= PY_VERSION_HEX
   DeadPyAPI_FUNC(int) DeadPy_IsFinalizing(void);
#  if ((PY_VERSION_HEX < 0x030D00A1 || defined(PYPY_VERSION_NUM)) \
       && !defined(DeadPy_NO_ALIAS))
#    define Py_IsFinalizing DeadPy_IsFinalizing
#  endif
#endif


// --- Import ----------------------------------------------------------------

DeadPyAPI_FUNC(PyObject*) DeadPyImport_AddModuleRef(const char *name);
#if PY_VERSION_HEX < 0x030D00A0 && !defined(DeadPy_NO_ALIAS)
#  define PyImport_AddModuleRef DeadPyImport_AddModuleRef
#endif

// --- PyThreadState ---------------------------------------------------------

DeadPyAPI_FUNC(PyThreadState*) DeadPyThreadState_GetUnchecked(void);
#if (PY_VERSION_HEX >= 0x03050200 && PY_VERSION_HEX < 0x030D00A1)  && !defined(DeadPy_NO_ALIAS)
#  define PyThreadState_GetUnchecked DeadPyThreadState_GetUnchecked
#endif

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


// --- Time ------------------------------------------------------------------

#if 0x03050000 <= PY_VERSION_HEX
#  if PY_VERSION_HEX < 0x030D00A4
   typedef _PyTime_t PyTime_t;
#  endif

   DeadPyAPI_FUNC(double) DeadPyTime_AsSecondsDouble(PyTime_t t);
   DeadPyAPI_FUNC(int) DeadPyTime_Monotonic(PyTime_t *result);
   DeadPyAPI_FUNC(int) DeadPyTime_Time(PyTime_t *result);
   DeadPyAPI_FUNC(int) DeadPyTime_PerfCounter(PyTime_t *result);
#  if PY_VERSION_HEX < 0x030D00A4 && !defined(DeadPy_NO_ALIAS)
#    define PyTime_AsSecondsDouble DeadPyTime_AsSecondsDouble
#    define PyTime_Monotonic DeadPyTime_Monotonic
#    define PyTime_Time DeadPyTime_Time
#    define PyTime_PerfCounter DeadPyTime_PerfCounter
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

DeadPyAPI_FUNC(int) DeadPyUnicode_EqualToUTF8AndSize(PyObject *unicode, const char *str, Py_ssize_t str_len);
DeadPyAPI_FUNC(int) DeadPyUnicode_EqualToUTF8(PyObject *unicode, const char *str);
#if PY_VERSION_HEX < 0x030D00A1
#  define PyUnicode_EqualToUTF8AndSize DeadPyUnicode_EqualToUTF8AndSize
#  define PyUnicode_EqualToUTF8 DeadPyUnicode_EqualToUTF8
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


// --- Weakref ---------------------------------------------------------------

DeadPyAPI_FUNC(int) DeadPyWeakref_GetRef(PyObject *ref, PyObject **pobj);
#if PY_VERSION_HEX < 0x030D0000 && !defined(DeadPy_NO_ALIAS)
#  define PyWeakref_GetRef DeadPyWeakref_GetRef
#endif


#ifdef __cplusplus
}
#endif
#endif  // !DEADPARROT_H
