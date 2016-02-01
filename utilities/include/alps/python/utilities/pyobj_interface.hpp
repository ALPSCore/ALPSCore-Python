/*
 * Copyright (C) 1998-2015 ALPS Collaboration. See COPYRIGHT.TXT
 * All rights reserved. Use is subject to license terms. See LICENSE.TXT
 * For use in publications, see ACKNOWLEDGE.TXT
 */

/** @file pyobj_interface.hpp
    @brief Interace with Python code (accessing Python objects).

    @details This interface is intended to be lighter/simpler than
    boost::python, and therefore less powerful; to be used when
    dependence on boost::python is undesirable.
*/

#ifndef ALPS_PYTHON_UTILITIES_PYOBJ_INTERFACE_HPP_INCLUDED
#define ALPS_PYTHON_UTILITIES_PYOBJ_INTERFACE_HPP_INCLUDED

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
        };


      
#undef ALPS_PYTHON_PARAMS_DETAILS_TR


      
    } // end namespace python
} // end namespace alps

#endif // ALPS_PYTHON_UTILITIES_PYOBJ_INTERFACE_HPP_INCLUDED
