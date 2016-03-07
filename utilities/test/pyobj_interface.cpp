/*
 * Copyright (C) 1998-2016 ALPS Collaboration. See COPYRIGHT.TXT
 * All rights reserved. Use is subject to license terms. See LICENSE.TXT
 * For use in publications, see ACKNOWLEDGE.TXT
 */


#include <Python.h>

#include <alps/python/utilities/pyobj_interface.hpp>
#include <alps/python/utilities/pyobj_conversion.hpp>
#include <iostream>
#include <boost/lexical_cast.hpp>

/// Internal shortcut to boost::lexical_cast<string>()
template <typename T>
static inline std::string toString(T val) { return boost::lexical_cast<std::string>(val); }

/// Internal shortcut to convert std::string to PyObject
static inline PyObject* toPystring(const std::string& s)
{
    return PyString_FromString(s.c_str());
}

/// Internal convenience function to convert char* to PyObject
static inline PyObject* toPystring(const char* s)
{
    return PyString_FromString(s);
}

namespace apd=alps::python::detail;

/// Expects arguments (10, 20, 30). Returns "OK" if sees them, or error diagnostic otherwise
static PyObject* test_pyseq(PyObject* self, PyObject* args)
{
    Py_INCREF(args);
    alps::python::pyseq_wrapper pyseq(args);

    const long expected[]={10, 20, 30};
    if (pyseq.size()!=3) {
        return toPystring("Size: expected 3, got "+toString(pyseq.size()));
    }
    
    for (int i=0; i<3; ++i) {
        PyObject* item=pyseq[i];
        if (item==0) return toPystring("NULL item at #"+toString(i));
        long val=PyInt_AsLong(item);
        if (val==-1 && PyErr_Occurred()) return 0;
        if (expected[i]!=val) {
            return toPystring("At item #" + toString(i) +
                              ": expected value " + toString(expected[i])+
                              " got " + toString(val));
        }
    }
    return toPystring("OK");
}

/// Convenience function: check for a single argument, return the corresponding PyObject
static PyObject* single_arg(PyObject* args)
{
    Py_INCREF(args);
    alps::python::pyseq_wrapper pyseq(args);

    if (pyseq.size()!=1) {
        PyErr_SetString(PyExc_RuntimeError,"Exactly 1 argument expected");
        return 0;
    }
    return pyseq[0];
}
 

/// Check for boolean argument True; return "OK" or error message.
static PyObject* test_bool(PyObject* self, PyObject* args)
{
    PyObject* obj=single_arg(args);
    if (!obj) return 0;
    
    if (!apd::pyobj_check<bool>::apply(obj)) return toPystring("Not boolean");
    if (!apd::pyobj_cast<bool>::apply(obj)) return toPystring("True expected, got False");
    return toPystring("OK");
}

#define PYOBJ_TEST_DETAIL_DUMMY_FN(_name_)                              \
    static PyObject* _name_(PyObject*, PyObject*) {                     \
        PyErr_SetString(PyExc_NotImplementedError, #_name_ "() not implemented"); \
        return 0;                                                       \
    }

PYOBJ_TEST_DETAIL_DUMMY_FN(test_vbool)
PYOBJ_TEST_DETAIL_DUMMY_FN(test_int)
PYOBJ_TEST_DETAIL_DUMMY_FN(test_vint)
PYOBJ_TEST_DETAIL_DUMMY_FN(test_long)
PYOBJ_TEST_DETAIL_DUMMY_FN(test_vlong)
PYOBJ_TEST_DETAIL_DUMMY_FN(test_float)
PYOBJ_TEST_DETAIL_DUMMY_FN(test_vfloat)
PYOBJ_TEST_DETAIL_DUMMY_FN(test_string)
PYOBJ_TEST_DETAIL_DUMMY_FN(test_vstring)

#undef PYOBJ_TEST_DETAIL_DUMMY_FN

static PyMethodDef test_methods[]={
    {"test_pyseq", test_pyseq, METH_VARARGS,
     "Tests pyobject_seq class. Input: 3 arguments: (10, 20, 30); Output: \"OK\" or error message string."
    },
    {"test_bool", test_bool, METH_VARARGS, "Tests boolean value conversion"},
    {"test_vbool", test_vbool, METH_VARARGS, "Tests boolean vector conversion"},
    {"test_int", test_int, METH_VARARGS, "Tests integer (C long) value conversion"},
    {"test_vint", test_vint, METH_VARARGS, "Tests integer (C long) vector conversion"},
    {"test_long", test_long, METH_VARARGS, "Tests long (C long long) value conversion"},
    {"test_vlong", test_vlong, METH_VARARGS, "Tests long (C long long) vector conversion"},
    {"test_float", test_float, METH_VARARGS, "Tests float (C double) value conversion"},
    {"test_vfloat", test_vfloat, METH_VARARGS, "Tests float (C double) vector conversion"},
    {"test_string", test_string, METH_VARARGS, "Tests string value conversion"},
    {"test_vstring", test_vstring, METH_VARARGS, "Tests string vector conversion"},
    {0, 0, 0, 0}
};

PyMODINIT_FUNC initpyobjtest_c()
{
    Py_InitModule("pyobjtest_c", test_methods);
}
