#define DeadPy_NO_ALIAS
#include "deadparrot.h"
#include "private.h"


// The fallback code below is cursed. It provides an alternative when
// PyType_FromMetaclass() is not available we are furthermore *not*
// targeting the stable ABI interface. It calls PyType_FromSpec() to create
// a tentative type, copies its contents into a larger type with a
// different metaclass, then lets the original type expire.
//
// Initial code written by Wenzel Jakob for nanobind.
//
// https://github.com/python/cpython/pull/93012#issuecomment-1138876646
// https://github.com/wjakob/nanobind/blob/0b3548ad6faf4eb18aa72259c3b82ac6df284652/src/nb_type.cpp#L373
PyObject *
DeadPyType_FromMetaclass(PyTypeObject *metaclass, PyObject *module,
                         PyType_Spec *spec, PyObject *bases)
{
    // Python 3.11 doesn't support the module
    (void)module;

    PyTypeObject *temp = (PyTypeObject *)PyType_FromSpec(spec);
    if (!temp) {
        goto error;
    }
    PyHeapTypeObject *temp_ht = (PyHeapTypeObject *)temp;
    PyTypeObject *temp_tp = &temp_ht->ht_type;

    Py_INCREF(temp_ht->ht_name);
    Py_INCREF(temp_ht->ht_qualname);
    Py_INCREF(temp_tp->tp_base);
    Py_XINCREF(temp_ht->ht_slots);

    PyObject *result = PyType_GenericAlloc(metaclass, 0);
    if (!result) {
        goto error;
    }

    PyHeapTypeObject *ht = (PyHeapTypeObject *) result;
    PyTypeObject *tp = &ht->ht_type;

    memcpy(ht, temp_ht, sizeof(PyHeapTypeObject));

    tp->ob_base.ob_base.ob_type = metaclass;
    tp->ob_base.ob_base.ob_refcnt = 1;
    tp->ob_base.ob_size = 0;
    tp->tp_as_async = &ht->as_async;
    tp->tp_as_number = &ht->as_number;
    tp->tp_as_sequence = &ht->as_sequence;
    tp->tp_as_mapping = &ht->as_mapping;
    tp->tp_as_buffer = &ht->as_buffer;
    tp->tp_flags = spec->flags | Py_TPFLAGS_HEAPTYPE;

    tp->tp_dict = NULL;
    tp->tp_bases = NULL;
    tp->tp_cache = NULL;
    tp->tp_subclasses = NULL;
    tp->tp_weaklist = NULL;
    ht->ht_cached_keys = NULL;
    tp->tp_version_tag = 0;

    // Move the name and MRO from 'temp' to 'result'
    tp->tp_name = temp_ht->_ht_tpname; temp_ht->_ht_tpname = NULL;
//    tp->tp_mro = temp->tp_mro; temp->tp_mro = NULL;

    if (bases != NULL && !PyTuple_Check(bases)) {
        tp->tp_base = Py_NewRef(bases);
    }

    if (PyType_Ready(tp) < 0) {
        Py_DECREF(result);
        goto error;
    }

    Py_DECREF(temp);
    return result;

error:
    Py_XDECREF(temp);
    return NULL;
}
