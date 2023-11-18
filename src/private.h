#ifndef DEADPARROT_PRIVATE_H
#define DEADPARROT_PRIVATE_H
#ifdef __cplusplus
extern "C" {
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


#ifdef __cplusplus
}
#endif
#endif  // !DEADPARROT_PRIVATE_H
