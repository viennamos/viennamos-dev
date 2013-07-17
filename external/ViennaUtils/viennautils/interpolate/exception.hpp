#ifndef VIENNAUTILS_INTERPOLATE_EXCEPTION_HPP
#define	VIENNAUTILS_INTERPOLATE_EXCEPTION_HPP


#include <exception>

namespace viennautils
{
    namespace interpolate
    {
        class value_out_of_interpolation_bounds_exception : public std::runtime_error
        {
        public:

          value_out_of_interpolation_bounds_exception(std::string const & str) : std::runtime_error(str) { }

          virtual const char* what() const throw ()
          {
            std::stringstream ss;
            ss << "* ViennaUtils: Value out of interpolation bounds! '" << msg_ << "' Invalid value being: '" << value_ << "'"
               << "bound beeing: '" << bound_  <<"'";
            return ss.str().c_str();
          }

         value_out_of_interpolation_bounds_exception(std::string msg, double value, double bound) : std::runtime_error(msg),
                                                                                                    msg_(msg), value_(value), bound_(bound) { }

          virtual ~value_out_of_interpolation_bounds_exception() throw () { }

        private:
          std::string msg_;
          double value_;
          double bound_;
        };


//        class invalid_tag_exception : public std::runtime_error
//        {
//        public:
//
//          invalid_tag_exception(std::string const & str) : std::runtime_error(str) { }
//
//          virtual const char* what() const throw ()
//          {
//            std::stringstream ss;
//            ss << "* ViennaUtils: Invalid tag! '" << msg_ << "' Invalid tag being: '" << tag_ << "'";
//            return ss.str().c_str();
//          }
//
//          invalid_tag_exception(std::string msg, std::string tag) : std::runtime_error(msg), msg_(msg), tag_(tag) { }
//
//          virtual ~invalid_tag_exception() throw () { }
//
//        private:
//          std::string msg_;
//          std::string tag_;
//        };


        class invalid_parameter_exception : public std::runtime_error
        {
        public:

          invalid_parameter_exception(std::string const & str) : std::runtime_error(str) { }

          virtual const char* what() const throw ()
          {
            std::stringstream ss;
            ss << "* ViennaUtils: Invalid parameter! '" << msg_ << "' Invalid value being: '" << value_ << "'";
            return ss.str().c_str();
          }

          invalid_parameter_exception(std::string msg, double value) : std::runtime_error(msg), msg_(msg), value_(value) { }

          virtual ~invalid_parameter_exception() throw () { }

        private:
          std::string msg_;
          double value_;
        };


        class not_the_same_size_exception : public std::runtime_error
        {
        public:

          not_the_same_size_exception(std::string const & str) : std::runtime_error(str) { }

          virtual const char* what() const throw ()
          {
            std::stringstream ss;
            ss << "* ViennaUtils: not_the_same_size_exception! '" << msg_ << "' size1 being: '" << size1_ << "'" << "' size2 being: '" << size2_ << "'";
            return ss.str().c_str();
          }

          not_the_same_size_exception(std::string msg, long size1, long size2) : std::runtime_error(msg), msg_(msg), size1_(size1), size2_(size2) { }

          virtual ~ not_the_same_size_exception() throw () { }


        private:
          std::string msg_;
          long size1_;
          long size2_;
        };

        class index_out_of_bounds_exception : public std::runtime_error
        {
        public:

          index_out_of_bounds_exception(std::string const & str) : std::runtime_error(str) { }

          virtual const char* what() const throw ()
          {
            std::stringstream ss;
            ss << "* ViennaUtils: index out of bounds! '" << msg_ << "' index being: '" << index_ << "'" << "' bound being: '" << bound_ << "'";
            return ss.str().c_str();
          }

          index_out_of_bounds_exception(std::string msg, long index, long bound) : std::runtime_error(msg), msg_(msg), index_(index), bound_(bound) { }

          virtual ~index_out_of_bounds_exception() throw () { }


        private:
          std::string msg_;
          long index_;
          long bound_;
        };


        class no_such_file_exception : public std::runtime_error
        {
        public:

          no_such_file_exception(std::string const & str) : std::runtime_error(str) { }

          virtual const char* what() const throw ()
          {
            std::stringstream ss;
            ss << "* ViennaUtils: no such file '" << msg_ << "'" << "filename being: '" << filename_ << "'";
            return ss.str().c_str();
          }

          no_such_file_exception(std::string msg, std::string filename) : std::runtime_error(msg), msg_(msg), filename_(filename) { }

          virtual ~no_such_file_exception() throw () { }


        private:
          std::string msg_;
          std::string filename_;
        };

         class interpolation_error_exception : public std::runtime_error
        {
        public:

          interpolation_error_exception(std::string const & msg) : std::runtime_error(msg) { }
        };

    }//interpolate
} // viennautils

#endif	/* VIENNAUTILS_INTERPOLATE_EXCEPTION_HPP */

