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
#include <vector>

#include "alps/python/utilities/pyobj_interface.hpp"


// conversion traits

namespace alps {
    namespace python {
        namespace detail {
            // Python "functions" are in fact macros, so we wrap them into callable classes
            // ...and store return type information too, while we are at it.
#define MAKE_CHECK_CLASS_DETAIL(_name_, _checkfn_)                      \
            struct _name_ { static bool apply(PyObject* po) { return _checkfn_(po); } };
#define MAKE_CVT_CLASS_DETAIL(_name_, _cvtfn_, _type_)                  \
            struct _name_ {                                             \
                typedef _type_ value_type;                              \
                static value_type apply(PyObject* po) { return _cvtfn_(po); } \
            };

            MAKE_CHECK_CLASS_DETAIL(pyint_check, PyInt_Check)
            MAKE_CHECK_CLASS_DETAIL(pylong_check, PyLong_Check)
            MAKE_CHECK_CLASS_DETAIL(pyfloat_check, PyFloat_Check)
            MAKE_CHECK_CLASS_DETAIL(pybool_check, PyBool_Check)
            MAKE_CHECK_CLASS_DETAIL(pystring_check, PyString_Check)

            MAKE_CVT_CLASS_DETAIL(pyint2long, PyInt_AsLong, long)
            MAKE_CVT_CLASS_DETAIL(pylong2llong, PyLong_AsLongLong, long long)
            MAKE_CVT_CLASS_DETAIL(pyfloat2double, PyFloat_AsDouble, double)

#undef MAKE_CHECK_CLASS_DETAIL
#undef MAKE_CVT_CLASS_DETAIL


            // Apply check to a Python object, returning a boolean
            template <typename F>
            struct pyobj_check_base {
                static bool apply(PyObject* po) { return F::apply(po); }
            };

            // Apply a conversion function to a Python object
            template <typename F>
            struct pyobj_cast_base {
                typedef typename F::value_type value_type;
                static value_type apply(PyObject* po)
                {
                    value_type val=F::apply(po);
                    if (val==value_type(-1)) pyexception::raise_if_error("Python type conversion error"); // FIXME: not very informative
                    return val;
                }
            };
        
            // Traits to verify that a python objects contains a given scalar type: generic
            template <typename> struct pyobj_check {};

            // Traits to provide a conversion from a (checked) python object to a scalar type: generic
            template <typename> struct pyobj_cast {};

            template <> struct pyobj_check<long> : public pyobj_check_base<detail::pyint_check> {};
            template <> struct pyobj_cast<long> : public pyobj_cast_base<detail::pyint2long> {};

            // maps PyLong -> long long
            template <> struct pyobj_check<long long> : public pyobj_check_base<detail::pylong_check> {};
            template <> struct pyobj_cast<long long> : public pyobj_cast_base<detail::pylong2llong> {};

            // maps PyFloat -> double
            template <> struct pyobj_check<double> : public pyobj_check_base<detail::pyfloat_check> {};
            template <> struct pyobj_cast<double> : public pyobj_cast_base<detail::pyfloat2double> {};

            // maps PyBool -> bool
            template <> struct pyobj_check<bool> : public pyobj_check_base<detail::pybool_check> {};
            template <> struct pyobj_cast<bool> {
                static bool apply(PyObject* po)
                {
                    if (po==Py_False) return false;
                    if (po==Py_True) return true;
                    // if we forgot to check the PyObject's type...
                    throw std::logic_error("Boolean PyObject is neither true nor false");
                }
            };
       
            // maps PyString -> std::string
            template <> struct pyobj_check<std::string> : public pyobj_check_base<detail::pystring_check> {};
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
            
        } // detail::
    } // python::
} // alps::

#endif //ALPS_PYTHON_UTILITIES_PYOBJ_TRAITS_HPP_d293de47516944468bff515a28e5fb62
