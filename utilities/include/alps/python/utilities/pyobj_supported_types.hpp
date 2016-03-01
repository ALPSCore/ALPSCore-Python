/*
 * Copyright (C) 1998-2015 ALPS Collaboration. See COPYRIGHT.TXT
 * All rights reserved. Use is subject to license terms. See LICENSE.TXT
 * For use in publications, see ACKNOWLEDGE.TXT
 */

/** @file pyobj_supported_types.hpp
    @brief Defines scalar and vector types supported by the Python/C++ interface
*/

#ifndef ALPS_PYTHON_UTILITIES_SUPPORTED_TYPES_HPP_82dc906e611348e9a9c781d60cf261ec
#define ALPS_PYTHON_UTILITIES_SUPPORTED_TYPES_HPP_82dc906e611348e9a9c781d60cf261ec

#include <boost/preprocessor/seq/transform.hpp>

// The C++ equivalents of scalar Python types we support
#define ALPS_PYTHON_SUPPORTED_SCALAR_TYPES (bool)(long)(long long)(double)(std::string)

#define ALPS_PYTHON_DETAIL_TR(dummy_,templ_,type_) templ_<type_>
// The vectors of the supported scalar types
#define ALPS_PYTHON_SUPPORTED_VECTOR_TYPES BOOST_PP_SEQ_TRANSFORM(ALPS_PYTHON_PARAMS_DETAIL_TR, \
                                                                  std::vector, \
                                                                  ALPS_PYTHON_SUPPORTED_SCALAR_TYPES)
#undef ALPS_PYTHON_DETAILS_TR

#endif /* ALPS_PYTHON_UTILITIES_SUPPORTED_TYPES_HPP_82dc906e611348e9a9c781d60cf261ec */
