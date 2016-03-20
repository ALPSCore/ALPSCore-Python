/*
 * Copyright (C) 1998-2015 ALPS Collaboration. See COPYRIGHT.TXT
 * All rights reserved. Use is subject to license terms. See LICENSE.TXT
 * For use in publications, see ACKNOWLEDGE.TXT
 */

#include <Python.h>

#include <string>
#include <cassert>
#include <boost/preprocessor/seq/transform.hpp>

#include "alps/python/params.hpp"

#include "alps/python/utilities/pyobj_supported_types.hpp"
#include "alps/python/utilities/pyobj_conversion.hpp"

namespace alps {
    namespace python {
        namespace params {
            alps::params params_from_dict(PyObject* dict)
            {
                using alps::python::detail::pyobj_check;
                using alps::python::detail::pyobj_cast;
                using alps::python::pyseq_wrapper;
                
                // Check if it is a dictionary (or a map object)
                if (!PyMapping_Check(dict)) {
                    alps::python::pyexception::raise(PyExc_TypeError,
                                                     "A non-mapping object is passed to alps::params constructor");
                }
                // Get list of key-value pairs
                pyseq_wrapper items(PyMapping_Items(dict));
                Py_ssize_t nitems=items.size();
                alps::params ret_param;

                // Loop/iterate over pairs
                for (Py_ssize_t it=0; it<nitems; ++it) {
                    pyseq_wrapper kvpair(items[it]);
                    assert(kvpair.size()==2 && "Key-value pair from Python must have length!=2");
                    if (!pyobj_check<std::string>::apply(kvpair[0])) {
                        throw std::runtime_error("Dictonary key other than string is unsupported");
                    }
                    std::string key=pyobj_cast<std::string>::apply(kvpair[0]);

#define ALPS_DETAIL_TRY_TYPE(_dummy_,_data_,_typ_)                      \
                    else if (pyobj_check<_typ_>::apply(kvpair[1])) {    \
                        _typ_ val=pyobj_cast<_typ_>::apply(kvpair[1]);  \
                        ret_param[key]=val;                             \
                    }
                    if (false); // <- needed for the following macros to work!
                    BOOST_PP_SEQ_FOR_EACH(ALPS_DETAIL_TRY_TYPE,~,ALPS_PYTHON_SUPPORTED_SCALAR_TYPES)
                    BOOST_PP_SEQ_FOR_EACH(ALPS_DETAIL_TRY_TYPE,~,ALPS_PYTHON_SUPPORTED_VECTOR_TYPES)
                    else {
                        throw std::runtime_error("Unsupported value type for key "+key);
                    }
#undef ALPS_DETAIL_TRY_TYPE
                } // end loop over dict
                return ret_param;
            }

        } // params::
    } // python::
} // alps::
