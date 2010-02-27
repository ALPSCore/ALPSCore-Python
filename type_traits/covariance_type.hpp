/*****************************************************************************
*
* ALPS Project: Algorithms and Libraries for Physics Simulations
*
* ALPS Libraries
*
* Copyright (C) 2010 by Matthias Troyer <troyer@comp-phys.org>,
*
* This software is part of the ALPS libraries, published under the ALPS
* Library License; you can use, redistribute it and/or modify it under
* the terms of the license, either version 1 or (at your option) any later
* version.
* 
* You should have received a copy of the ALPS Library License along with
* the ALPS Libraries; see the file LICENSE.txt. If not, the license is also
* available from http://alps.comp-phys.org/.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
* FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT 
* SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE 
* FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE, 
* ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
* DEALINGS IN THE SOFTWARE.
*
*****************************************************************************/

/* $Id: obsvalue.h 3435 2009-11-28 14:45:38Z troyer $ */

#ifndef ALPS_TYPE_TRAITS_COVARIANCE_TYPE_H
#define ALPS_TYPE_TRAITS_COVARIANCE_TYPE_H

#include <alps/type_traits/average_type.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <valarray>
#include <vector>


namespace alps {

template <class T>
struct covariance_type : average_type<T> {};

template <class T>
struct covariance_type<std::valarray<T> > 
{
  typedef typename boost::numeric::ublas::matrix<typename average_type<T>::type> type;  
};

template <class T, class A>
struct covariance_type<std::vector<T,A> > 
{
  typedef typename boost::numeric::ublas::matrix<typename average_type<T>::type> type;  
};



} // end namespace alps

#endif // ALPS_TYPE_TRAITS_COVARIANCE_TYPE_H