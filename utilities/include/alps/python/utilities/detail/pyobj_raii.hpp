/*
 * Copyright (C) 1998-2015 ALPS Collaboration. See COPYRIGHT.TXT
 * All rights reserved. Use is subject to license terms. See LICENSE.TXT
 * For use in publications, see ACKNOWLEDGE.TXT
 */

/** @file pyobj_raii.hpp
    @brief Defines a RAII-style wrapper around a `PyObject*`
*/


#ifndef ALPS_PYTHON_UTILITIES_DETAIL_PYOBJ_RAII_HPP_INCLUDED
#define ALPS_PYTHON_UTILITIES_DETAIL_PYOBJ_RAII_HPP_INCLUDED

namespace alps { namespace python { namespace detail {

            // Because i want to avoid using boost::python (policy decision)
            // and i am too lazy to write a proper reference-counting wrapper,
            // here is a simple RAII-style Python object wrapper to decrement refcount on destruction.

            /// Constructed with a **new** Python object, decrements refcount on destruction
            class pyobject_raii_type : boost::noncopyable {
                PyObject* pyobj_;
                public:
                pyobject_raii_type(PyObject* po): pyobj_(po) {}
                virtual ~pyobject_raii_type() { if pyobj_ Py_DECREF(pyobj_); }
                PyObject* operator()() { return pyobj_; }
            };

        }
    }
}
      
#endif // ALPS_PYTHON_UTILITIES_DETAIL_PYOBJ_RAII_HPP_INCLUDED



