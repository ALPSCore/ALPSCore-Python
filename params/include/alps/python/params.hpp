/*
 * Copyright (C) 1998-2015 ALPS Collaboration. See COPYRIGHT.TXT
 * All rights reserved. Use is subject to license terms. See LICENSE.TXT
 * For use in publications, see ACKNOWLEDGE.TXT
 */

#ifndef ALPS_PYTHON_PARAMS_HPP_7062c0890998417685d2444c5276371d
#define ALPS_PYTHON_PARAMS_HPP_7062c0890998417685d2444c5276371d

#include <alps/params.hpp>

namespace alps {
    namespace python {
        namespace params {
            /// Generate a new alps::params instance from Python dictionary
            alps::params params_from_dict(PyObject* dict);
        }
    }
}

#endif /* ALPS_PYTHON_PARAMS_HPP_7062c0890998417685d2444c5276371d */
