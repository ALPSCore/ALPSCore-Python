/*
 * Copyright (C) 1998-2015 ALPS Collaboration. See COPYRIGHT.TXT
 * All rights reserved. Use is subject to license terms. See LICENSE.TXT
 * For use in publications, see ACKNOWLEDGE.TXT
 */

/** @file pyobj_interface.hpp
    @brief Defines a thin C++ interface to Python C API.
*/


#ifndef ALPS_PYTHON_UTILITIES_PYOBJ_INTERFACE_HPP_be3a2b9eb6ef487d8af28d8db786643a
#define ALPS_PYTHON_UTILITIES_PYOBJ_INTERFACE_HPP_be3a2b9eb6ef487d8af28d8db786643a

#include <cassert>
#include <stdexcept>

namespace alps {
    namespace python { 

        /// general Python exception
        class pyexception : public std::runtime_error {
            PyObject* pyerr_; ///< Borrowed reference to Python exception

            pyexception(const std::string& msg, PyObject* perr): std::runtime_error(msg), pyerr_(perr) {}
            public:

            /// Getter method: @returns borrowed reference to Python exception
            PyObject* operator()() const { return pyerr_; } 

            /// Throws the exception if a Python error has occurred, returns false otherwise
            static bool raise_if_error(const std::string& msg)
            {
                PyObject* perr=PyErr_Occurred();
                if (perr)
                    throw pyexception(msg, perr);
                else
                    return false;
            }

            /// Throws the exception wrapping the specified Python exception
            // FIXME: specialize so that it can raise only valid Python exceptions?
            static void raise(PyObject* exc, const std::string& msg)
            {
                PyErr_SetString(exc, msg.c_str());
                raise_if_error(msg);
                throw std::logic_error("Can't happen: setting Python exception hasn't set exception condition!");
            }
        };

        // Because we want to avoid using boost::python (by policy)
        // here is a simple RAII-style Python object wrapper
        // to maintain correct refcount.

        /// Constructed with a **new** Python object, takes the ownership and maintains correct refcount
        class pyobject_wrapper {
            PyObject* pyobj_;
            public:
            /// Default constructor: empty (NULL) PyObject
            pyobject_wrapper(): pyobj_(0) {}
            
            /// Constructor: from now on, the PyObject is "stolen" (maintained by the wrapper)
            explicit pyobject_wrapper(PyObject* po): pyobj_(po) { }
            
            /// Destructor: disowns the wrapped PyObject
            virtual ~pyobject_wrapper() { Py_XDECREF(pyobj_); }

            /// Accessor: returns the wrapped PyObject (to borrow)
            PyObject* operator()() { return pyobj_; }

            /// Copy-constructor: takes the ownership of the copy
            pyobject_wrapper(const pyobject_wrapper& rhs): pyobj_(rhs.pyobj_) {
                Py_XINCREF(pyobj_);
            }

            /// Assignment: takes the ownership of the copy, disowns the current wrapped PyObject
            pyobject_wrapper& operator=(const pyobject_wrapper& rhs) {
                Py_XINCREF(rhs.pyobj_); // increment refcount first, in case of self-assignment
                Py_XDECREF(this->pyobj_);
                pyobj_=rhs.pyobj_;
                return *this;
            }
        };


        /// Thin class wrapping a Python sequence, to access elements and size
        class pyseq_wrapper {
            pyobject_wrapper wrapped_seq_;

          public:
            /// Flag whether to throw if constructed from non-sequence
            enum throw_flag_enum { MAY_THROW=0, DONT_THROW=1 };

            /// Constructor: converts to tuple/list for fast access; may throw if asked
            pyseq_wrapper(PyObject* po, throw_flag_enum throw_flag=MAY_THROW): wrapped_seq_()
            {
                if (!PySequence_Check(po)) {
                    if (throw_flag==MAY_THROW) {
                        throw std::logic_error("Sequence wrapper is called on non-sequence");
                    }
                    return; // leaves with default-initialized wrapped sequence
                }
                wrapped_seq_=pyobject_wrapper(PySequence_Fast(po,"Error converting PyObject to a tuple/list"));
                if (!wrapped_seq_()) {
                    if (!pyexception::raise_if_error("Error converting sequence")) {
                        throw std::logic_error("PySequence conversion failed without raising an exception");
                    }
                }
            }

            /// Validity predicate
            bool ok() { return !(wrapped_seq_()==0); }
            
            /// Sequence size (0 if not initialized)
            Py_ssize_t size()
            {
                if (!ok()) return 0;
                Py_ssize_t sz=PySequence_Size(wrapped_seq_());
                assert(sz>=0 && "PySequence_Size() failed unexpectedly");
                return sz;
            }

            /// Borrowed reference to a sequence element (undefined if out of range)
            PyObject* operator[](Py_ssize_t idx)
            {
                assert(wrapped_seq_() && "operator[] called on unitialized pyseq_wrapper");
                PyObject* pyitem=PySequence_Fast_GET_ITEM(wrapped_seq_(), idx);
                return pyitem;
            }

            /// Borrowed reference to the wrapped sequence
            PyObject* operator()()
            {
                return wrapped_seq_();
            }
        };
                
    } // python::
} // alps::
      
#endif /* ALPS_PYTHON_UTILITIES_PYOBJ_INTERFACE_HPP_be3a2b9eb6ef487d8af28d8db786643a */
