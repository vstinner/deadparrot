#ifndef DEADPARROT_PRIVATE_H
#define DEADPARROT_PRIVATE_H
#ifdef __cplusplus
extern "C" {
#endif

#ifndef _Py_CAST
#  define _Py_CAST(type, expr) ((type)(expr))
#endif
#ifndef _PyObject_CAST
#  define _PyObject_CAST(op) _Py_CAST(PyObject*, (op))
#endif

// Ignore deprecation warnings of Py_DEPRECATED()
#ifdef _Py_COMP_DIAG_IGNORE_DEPR_DECLS
#  define IGNORE_DEPR_WARNINGS _Py_COMP_DIAG_IGNORE_DEPR_DECLS
#elif defined(__clang__)
#  define IGNORE_DEPR_WARNINGS \
    _Pragma("clang diagnostic ignored \"-Wdeprecated-declarations\"")
#elif defined(__GNUC__) \
    && ((__GNUC__ >= 5) || (__GNUC__ == 4) && (__GNUC_MINOR__ >= 6))
#  define IGNORE_DEPR_WARNINGS \
    _Pragma("GCC diagnostic ignored \"-Wdeprecated-declarations\"")
#elif defined(_MSC_VER)
#  define IGNORE_DEPR_WARNINGS __pragma(warning(disable: 4996))
#else
#  define IGNORE_DEPR_WARNINGS
#endif


// Static inline functions should use _Py_NULL rather than using directly NULL
// to prevent C++ compiler warnings. On C23 and newer and on C++11 and newer,
// _Py_NULL is defined as nullptr.
#if (defined (__STDC_VERSION__) && __STDC_VERSION__ > 201710L) \
        || (defined(__cplusplus) && __cplusplus >= 201103)
#  define _Py_NULL nullptr
#else
#  define _Py_NULL NULL
#endif


#ifdef __cplusplus
}
#endif
#endif  // !DEADPARROT_PRIVATE_H
