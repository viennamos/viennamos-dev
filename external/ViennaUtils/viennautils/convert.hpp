/* =============================================================================
   Copyright (c) 2010, 2011, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at
                             -----------------
                ViennaUtils - The Vienna Utilities Library
                             -----------------

   authors:    Josef Weinbub                      weinbub@iue.tuwien.ac.at

   license:    see file LICENSE in the base directory
============================================================================= */

#ifndef VIENNAUTILS_CONVERTER_HPP
#define VIENNAUTILS_CONVERTER_HPP

#include <sstream>
#include <string>

#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/lexical_cast.hpp>

namespace viennautils {

template<typename Target>
struct convert
{
  typedef Target result_type; // model the std::result_of concept

  template<typename Source>
  result_type operator()(Source const& source, typename boost::enable_if<boost::is_same<Source,result_type> >::type* dummy = 0)
  {
    return source;
  }

  template<typename Source>
  result_type operator()(Source const& source, typename boost::disable_if<boost::is_same<Source,result_type> >::type* dummy = 0)
  {
    return boost::lexical_cast<result_type>(source);
  }
};

template<>
struct convert<float>
{
  typedef float result_type; // model the std::result_of concept
   
  template<typename Source>
  result_type operator()(Source const& source, typename boost::enable_if<boost::is_same<Source,result_type> >::type* dummy = 0)
  {
    return source;
  }

  template<typename Source>
  result_type operator()(Source const& source, typename boost::disable_if<boost::is_same<Source,result_type> >::type* dummy = 0)
  {
    sstr << source;
    sstr >> target;
    return target;
  }
private:
  result_type target;
  std::stringstream sstr;
};

template<>
struct convert<double>
{
  typedef double result_type; // model the std::result_of concept
   
  template<typename Source>
  result_type operator()(Source const& source, typename boost::enable_if<boost::is_same<Source,result_type> >::type* dummy = 0)
  {
    return source;
  }

  template<typename Source>
  result_type operator()(Source const& source, typename boost::disable_if<boost::is_same<Source,result_type> >::type* dummy = 0)
  {
    sstr << source;
    sstr >> target;
    return target;
  }
private:
  result_type target;
  std::stringstream sstr;
};


template<>
struct convert<std::string>
{
  typedef std::string result_type; // model the std::result_of concept
   
  template<typename Source>
  result_type operator()(Source const& source, typename boost::enable_if<boost::is_same<Source,result_type> >::type* dummy = 0)
  {
    return source;
  }

  template<typename Source>
  result_type operator()(Source const& source, typename boost::disable_if<boost::is_same<Source,result_type> >::type* dummy = 0)
  {
    sstr << source;
    sstr >> target;
    return target;
  }
private:
  result_type target;
  std::stringstream sstr;
};

} // end namespace viennautils

#endif




