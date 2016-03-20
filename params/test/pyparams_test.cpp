/*
 * Copyright (C) 1998-2016 ALPS Collaboration. See COPYRIGHT.TXT
 * All rights reserved. Use is subject to license terms. See LICENSE.TXT
 * For use in publications, see ACKNOWLEDGE.TXT
 */


#include <Python.h>

#include <alps/python/utilities/pyobj_interface.hpp>
#include <alps/python/utilities/pyobj_conversion.hpp>
#include <alps/python/params.hpp>
#include <iostream>
#include <algorithm>
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


/// Expects arguments dictionary with parameters; Returns "OK" parameters are as expected
static PyObject* test_params(PyObject* self, PyObject* args)
{
    Py_INCREF(args);
    alps::python::pyseq_wrapper pyseq(args);
    if (pyseq.size()!=1) return toPystring("Expected 1 argument");
    PyObject* dict=pyseq[0];

    try {
      alps::params p=alps::python::params::params_from_dict(dict);
    } catch (alps::python::pyexception& exc) {
      return 0;
    } catch (std::exception& exc) {
      PyErr_SetString(PyExc_RuntimeError, (std::string("Non-python C++ exception thrown: ")+exc.what()).c_str());
      return 0;
    }

    return toPystring("OK");
}
    
static PyMethodDef test_methods[]={
    {"test_params", test_params, METH_VARARGS,
     "Tests instantiating C++ alps::params from Python dictionary.\n"
     "Input: a sample dictionary.\n"
     "Output: \"OK\" or error message string."
    },
    {0, 0, 0, 0}
};

PyMODINIT_FUNC initpyparams_test_c()
{
    Py_InitModule("pyparams_test_c", test_methods);
}
