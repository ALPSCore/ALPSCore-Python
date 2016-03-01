/*
 * Copyright (C) 1998-2015 ALPS Collaboration. See COPYRIGHT.TXT
 * All rights reserved. Use is subject to license terms. See LICENSE.TXT
 * For use in publications, see ACKNOWLEDGE.TXT
 */

/** @file pyobj_conversion.hpp
    @brief Defines traits helping to convert a Python object to a C++ type
*/

#ifndef ALPS_PYTHON_UTILITIES_PYOBJ_CONVERSION_HPP_d293de47516944468bff515a28e5fb62
#define ALPS_PYTHON_UTILITIES_PYOBJ_CONVERSION_HPP_d293de47516944468bff515a28e5fb62

#include <cassert>
#include <string>
#include "alps/python/utilities/pyobj_interface.hpp"

// conversion traits

namespace alps {
    namespace python {
        namespace detail {

            // Call an integer function on a Python object, interpreting it as boolean
            template <int (*FN)(PyObject*)>
            struct pyobj_check_base {
                static bool apply(PyObject* po) { return FN(po); }
            };

            // Call a conversion function on a Python object
            template <typename T, T (*CVT)(PyObject*)>
            struct pyobj_cast_base {
                static T apply(PyObject* po)
                {
                    T val=CVT(po);
                    if (val==T(-1)) pyexception::raise_if_error("Python type conversion error"); // FIXME: not very informative
                    return val;
                }
            };
        
            // Traits to verify that a python objects contains a given scalar type: generic
            template <typename> struct pyobj_check {};

            // Traits to provide a conversion from a (checked) python object to a scalar type: generic
            template <typename> struct pyobj_cast {};

            // maps PyInt -> long
            template <> struct pyobj_check<long> : public pyobj_check_base<PyInt_Check> {};
            template <> struct pyobj_cast<long> : public pyobj_cast_base<long,PyInt_AsLong> {};

            // maps PyLong -> long long
            template <> struct pyobj_check<long long> : public pyobj_check_base<PyLong_Check> {};
            template <> struct pyobj_cast<long long> : public pyobj_cast_base<long long,PyLong_AsLongLong> {};

            // maps PyFloat -> double
            template <> struct pyobj_check<double> : public pyobj_check_base<PyFloat_Check> {};
            template <> struct pyobj_cast<double> : public pyobj_cast_base<double,PyFloat_AsDouble> {};

            // maps PyBool -> bool
            template <> struct pyobj_check<bool> : public pyobj_check_base<PyBool_Check> {};
            template <> struct pyobj_cast<bool> {
                static bool apply(PyObject* po)
                {
                    if (po==PyFalse) return false;
                    if (po==PyTrue) return true;
                    // if we forgot to check the PyObject's type...
                    throw std::logic_error("Boolean PyObject is neither true nor false");
                }
            };
       
            // maps PyString -> std::string
            template <> struct pyobj_check<std::string> : public pyobj_check_base<PyString_Check> {};
            template <> struct pyobj_cast<std::string> {
                static std::string apply(PyObject* po)
                {
                    char* buf=0; 
                    Py_ssize_t len=0; 
                    if (PyString_AsStringAndSize(po, &buf, &len)==-1) {
                        if (!pyexception::raise_if_error("Cannot convert Python string to std::string")) {
                            throw std::logic_error("PyString conversion failed without raising an exception");
                        }
                    }
                    return std::string(buf,len);
                }
            };

            // checks if the Python sequence can be mapped to std::vector<T>
            template <typename T>
            struct pyobj_check< std::vector<T> > {
                static bool apply(PyObject* po)
                {
                    pyseq_wrapper pyseq(po, pyseq_wrapper::DONT_THROW);
                    if (!pyseq.ok()) return false;
                    Py_ssize_t pyseq_sz=pyseq.size();

                    for (Py_ssize_t i=0; i<pyseq_sz; ++i) {
                        PyObject* pyitem=pyseq[i];
                        if (!pyobj_check<T>::apply(pyitem)) return false;
                    }
                    return true;
                }
            };

            // map the Python sequence to std::vector<T> (assuming `pyobj_check` was performed)
            template <typename T>
            struct pyobj_cast< std::vector<T> > {
                typedef std::vector<T> value_type;
                
                static value_type apply(PyObject* po)
                {
                    pyseq_wrapper pyseq(po);
                    Py_ssize_t pyseq_sz=pyseq.size();
                    value_type ret_val(pyseq_sz);

                    for (Py_ssize_t i=0; i<pyseq_sz; ++i) {
                        ret_val[i]=pyobj_cast<T>::apply(pyseq[i]);
                    }
                    return ret_val;
                }
            };
            
        } // ns detail
    } // ns python
} // ns alps

#undef //ALPS_PYTHON_UTILITIES_PYOBJ_TRAITS_HPP_d293de47516944468bff515a28e5fb62
