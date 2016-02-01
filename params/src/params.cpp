/*
 * Copyright (C) 1998-2015 ALPS Collaboration. See COPYRIGHT.TXT
 * All rights reserved. Use is subject to license terms. See LICENSE.TXT
 * For use in publications, see ACKNOWLEDGE.TXT
 */




#include "alps/python/params/params.hpp"

#include <string>
#include <cassert>
#include <boost/preprocessor/seq/enum.hpp>
#include <boost/preprocessor/seq/transform.hpp>
#include <boost/optional.hpp>



namespace {
    
    class py_container {
        public:
        struct empty_tag  {};
        typedef boost::optional< empty_tag,
                                 BOOST_PP_SEQ_ENUM(ALPS_PYTHON_PARAMS_SCALAR_TYPES),
                                 BOOST_PP_SEQ_ENUM(ALPS_PYTHON_PARAMS_VECTOR_TYPES) > container_optional_type;

        private:
        container_optional_type value_;

        // Visitor class to fill the vector with python list elements
        class vector_fill_visitor: public boost::static_visitor<> {
            const boost::python::list& list_;

            public:
            template <typename T>
            void operator()(std::vector<T>& val) const
            {
                assert(val.size() == boost::python::len(list_));
                for (int i=0; i<val.size(); ++i) {
                    val[i]=list_[i];
                }
            }

            vector_fill_visitor(const boost::python::list& a_list): list_(a_list) {}
        };

        template <typename T>
        bool try_set_list_type(const boost::python::list& list)
        {
            long sz=boost::python::len(list);
            if (sz==0) return false;
            boost::python::extract<T> x(list[0]);
            if (!x.check()) return false;
            value_=std::vector<T>(sz);
            return true;
        }

        template <typename T>
        bool try_assign_list(const boost::python::list& list)
        {
            if (!try_set_list_type(list)) return false;
            boost::apply_visitor(vector_fill_visitor(v), val_);
            return true;
        }
            
        template <typename T>
        bool try_assign_scalar(const boost::python::object& scalar)
        {
            boost::pyton::extract<T> x(scalar);
            if (!x.check()) return false;
            value_=x();
            return true;
        }
        
        
        public:

        template <typename T>
        std::vector<T> as_vector();

        py_vector(const boost::python::list& list)

        // Note: do not care about "safe bool" --- this is an internal class
        operator bool() { return val_.which()!=0; }
    };

    // Note: If conversion is unsuccessful, the object will be "false"
    py_vector::py_vector(const boost::python::object& pyobj)
    {

        // Try a scalar for each type: does the list contain this type?
        
#define ALPS_PYTHON_PARAMS_DETAILS_TRY_EXTRACT(dummy_,fn_,type_)  fn_<type_>(list) ||

        bool ok =
            BOOST_PP_SEQ_FOR_EACH(ALPS_PYTHON_PARAMS_DETAILS_TRY_EXTRACT, try_assign_scalar, ALPS_PYTHON_PARAMS_SCALAR_TYPES)
            BOOST_PP_SEQ_FOR_EACH(ALPS_PYTHON_PARAMS_DETAILS_TRY_EXTRACT, try_assign_list, ALPS_PYTHON_PARAMS_SCALAR_TYPES)
            false;

#undef ALPS_PYTHON_PARAMS_DETAILS_EXTRACT_TYPE

        // Try a scalar for each type: does the list contain this type?

        if (!*this) return;
        boost::apply_visitor(vector_fill_visitor(v), val_);
    }
        
    

} // end anonymous namespace



namespace alps {
    namespace python {
        namespace params {
            alps::params params_from_dict(PyObject* dict)
            {
              // Check if it is a dictionary (or a map object)
              // Get list of keys (or iterator over keys)
              // Loop/iterate over keys, extracting corresponding values
              // Try for each supported scalar type: int, long, float, string
              // If it is a sequence type (list, tuple), iterate over sequence;
              //   complication: it must be a sequence of the *same* types.
            }
        }
    }
}
