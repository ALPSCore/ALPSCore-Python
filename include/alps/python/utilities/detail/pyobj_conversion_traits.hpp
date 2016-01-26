/*
 * Copyright (C) 1998-2015 ALPS Collaboration. See COPYRIGHT.TXT
 * All rights reserved. Use is subject to license terms. See LICENSE.TXT
 * For use in publications, see ACKNOWLEDGE.TXT
 */

/** @file pyobj_conversion_traits.hpp
    @brief Defines traits helping to convert a Python object to a C++ type
*/

#ifndef ALPS_PYTHON_UTILITIES_DETAIL_PYOBJ_CONVERSION_TRAITS_HPP_INCLUDED
#define ALPS_PYTHON_UTILITIES_DETAIL_PYOBJ_CONVERSION_TRAITS_HPP_INCLUDED

// conversion traits

namespace alps {
    namespace python {
        namespace detail {

            // Call an integer function on a Python object, interpreting it as boolean
            template <int (*FN)(PyObject*)>
            struct pyobj_check_predicate {
                static bool check(PyObject* po) { return FN(po); }
            };

            // Call a conversion function on a Python object
            template <typename T, T (*CVT)(PyObject*)>
            struct pyobj_conversion {
                static T cast(PyObject* po)
                {
                    T val=CVT(po);
                    if (val==T(-1)) pyexception::raise_if_error("Python type conversion error"); // FIXME: not very informative
                    return val;
                }
            };
        
            // Traits to verify that a python objects contains a given scalar type: generic
            template <typename> struct pyobj_check_trait {};

            // Traits to provide a conversion from a (checked) python object to a scalar type: generic
            template <typename> struct pyobj_cast_trait {};

            // maps PyInt -> long
            template <> struct pyobj_check_trait<long> : public pyobj_check_predicate<PyInt_Check> {};
            template <> struct pyobj_cast_trait<long> : public pyobj_conversion<long,PyInt_AsLong> {};

            // maps PyLong -> long long
            template <> struct pyobj_check_trait<long long> : public pyobj_check_predicate<PyLong_Check> {};
            template <> struct pyobj_cast_trait<long long> : public pyobj_conversion<long long,PyLong_AsLongLong> {};

            // maps PyFloat -> double
            template <> struct pyobj_check_trait<double> : public pyobj_check_predicate<PyFloat_Check> {};
            template <> struct pyobj_cast_trait<double> : public pyobj_conversion<double,PyFloat_AsDouble> {};

            // maps PyBool -> bool
            template <> struct pyobj_check_trait<bool> : public pyobj_check_predicate<PyBool_Check> {};
            template <> struct pyobj_cast_trait<bool> {
                static bool cast(PyObject* po)
                {
                    if (po==PyFalse) return false;
                    if (po==PyTrue) return true;
                    // if we forgot to check the PyObject's type...
                    throw std::logic_error("Boolean PyObject is neither true nor false");
                }
            };
       
            // maps PyString -> std::string
            template <> struct pyobj_check_trait<std::string> : public pyobj_check_predicate<PyString_Check> {};
            template <> struct pyobj_cast_trait<std::string> {
                static std::string cast(PyObject* po)
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
        

            // to convert a (scalar) Python object to a type T
            template <typename T>
            class pyobj_cast {
                public:
                static boost::optional<T> as(PyObject* pyobj) {
                    if (!pyobj_check_trait<T>::check(pyobj)) return boost::none;
                    return pyobj_cast_trait<T>::cast(pyobj);
                }
            };

            // to convert a Python sequence of same-type objects to a vector of T
            template <typename T>
            class pyobj_cast< std::vector<T> > {
                public:
                typedef std::vector<T> value_type;
                typedef T element_type;
    
                static boost::optional<value_type> as(PyObject* po) {
                    if (!PySequence_Check(po)) return boost::none;
                    Py_ssize_t sz=PySequence_Size(po);
                    assert(sz>=0 && "Unexpected failure of PySequence_Size()");

                    value_type ret_val(sz);
                    pyobject_raii_type pyseq(PySequence_Fast(po,"Error converting PyObject to a tuple/list"));
                    if (!pyseq()) {
                        if (!pyexception::raise_if_error("Error convering sequence")) {
                            throw logic_error("PySequence conversion failed without raising an exception");
                        }
                    }
                    for (int i=0; i<sz; ++i) {
                        PyObject* pyitem=PySeq_Fast_GET_ITEM(pyseq(), i);
                        boost::optional<element_type> item=pyobj_cast<element_type>::cast(pyitem);
                        if (!item) return boost::none;
                        ret_val[i]=*item;
                    }
                    return ret_val;
                }
            };

        } // ns detail
    } // ns python
} // ns alps

#undef //ALPS_PYTHON_UTILITIES_DETAIL_PYOBJ_CONVERSION_TRAITS_HPP_INCLUDED
