
#ifndef VIENNAUTILS_INTERPOLATE_CRV_FILE_DIFF_CALCULATOR_HPP
#define	VIENNAUTILS_INTERPOLATE_CRV_FILE_DIFF_CALCULATOR_HPP

#include "viennamodels/io/crvfile.hpp"
#include <cstdlib>
#include "viennautils/interpolate/orthogrid_interpolate.hpp"
#include "viennautils/interpolate/exception.hpp"

namespace viennautils
{
    namespace interpolate
    {
        struct signed_tag{};
        struct absolute_tag{};
        struct squared_tag{};
//        template < typename T>
//        struct error_indicator{};

        typedef viennamodels::io::crv_file::value_type      value_type; //TODO
        typedef viennamodels::io::crv_file::index_type      index_type; //TODO
        typedef viennamodels::io::crv_file::ValueVectorType ValueVectorType; //TODO


     template < typename DiffTypeTag >
     //TODO Viennamath
     double calculate_diff (value_type ref, value_type value , DiffTypeTag tag)
     {
         typedef typename error_indicator<DiffTypeTag>::ERROR_NO_SUCH_METHOD_FOR_DIFF_CALCULATION key;
         return 0;
     }
     double calculate_diff (value_type ref, value_type value , signed_tag tag)
     {
         return (ref-value);
     }

     double calculate_diff (value_type ref, value_type value , absolute_tag tag)
     {
         return fabs(ref-value);
     }
     double calculate_diff (value_type ref, value_type value , squared_tag tag)
     {
         return (ref-value)*(ref-value);
     }


      template < typename DiffTypeTag, typename InterpolationTypeTag_x, typename InterpolationTypeTag_y >
      double diff1D(ValueVectorType const & ref_x, ValueVectorType const & ref_y, ValueVectorType const & tocheck_x,
                    ValueVectorType const & tocheck_y , InterpolationTypeTag_x x_intpol_type, InterpolationTypeTag_y y_intpol_type,
                    DiffTypeTag difftype)
      {
          if (ref_x.size() != ref_y.size() || tocheck_x.size() != tocheck_y.size())
          {
              throw viennautils::interpolate::not_the_same_size_exception("vectors in function diff1D are not the same size!");
          }

          viennautils::interpolate::interpolator1D<std::vector<double> > intp_ref(ref_x,ref_y);
          double diff = 0.0;
          for (index_type i =0; i<tocheck_x.size(); i++)
          {
              value_type curr_value = intp_ref(tocheck_x.at(i), x_intpol_type , y_intpol_type );
              diff+= calculate_diff (tocheck_y.at(i), curr_value , difftype);
          }
          return diff;
      }

      template < typename DiffTypeTag, typename InterpolationTypeTag_x, typename InterpolationTypeTag_y, typename InterpolationTypeTag_z>
      double diff2D(ValueVectorType const & ref_x, ValueVectorType const & ref_y, ValueVectorType const & ref_z,
                    ValueVectorType const & tocheck_x , ValueVectorType const & tocheck_y,
                    ValueVectorType const & tocheck_z, InterpolationTypeTag_x x_intpol_type, InterpolationTypeTag_y y_intpol_type,
                    InterpolationTypeTag_z z_intpol_type, DiffTypeTag difftype)
      {
          if (ref_x.size() != ref_z.size()         || ref_y.size() != ref_z.size() ||
              tocheck_x.size() != tocheck_z.size() || tocheck_y.size() != tocheck_z.size() )
          {
              throw viennautils::interpolate::not_the_same_size_exception("vectors in function diff2D are not the same size!");
          }

          viennautils::interpolate::interpolator2D<std::vector<double> > intp_ref(ref_x,ref_y,ref_z);
          double diff = 0.0;
          for (index_type i =0; i<tocheck_x.size(); i++)
          {
              value_type curr_value = intp_ref(tocheck_x.at(i),tocheck_y.at(i),x_intpol_type,y_intpol_type,z_intpol_type);
              diff+= calculate_diff (tocheck_z.at(i), curr_value, difftype);
          }
          return diff;
      }

    }// interpolate
} // viennautils


#endif	/* VIENNAUTILS_INTERPOLATE_CRV_FILE_DIFF_CALCILATOR_HPP */

