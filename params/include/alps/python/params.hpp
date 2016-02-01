/*
 * Copyright (C) 1998-2015 ALPS Collaboration. See COPYRIGHT.TXT
 * All rights reserved. Use is subject to license terms. See LICENSE.TXT
 * For use in publications, see ACKNOWLEDGE.TXT
 */

#ifndef ALPS_PYTHON_PARAMS_INCLUDED
#define ALPS_PYTHON_PARAMS_INCLUDED

// this must be first
#include "alps/python/utilities/boost_python.hpp"

#include <alps/params.hpp>

namespace alps {
    namespace python {
        namespace params {
            /// Generate a new alps::params instance from Python dictionary
            alps::params params_from_dict(PyObject* dict);
        }
    }
}

#endif // ALPS_PYTHON_PARAMS_INCLUDED
