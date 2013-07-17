
#ifndef VIENNAUTILS_INTERPOLATE_ORTHOGRID_INTERPOLATE_HPP
#define	VIENNAUTILS_INTERPOLATE_ORTHOGRID_INTERPOLATE_HPP

#include <cstdlib>
#include <vector>

#include "viennautils/interpolate/exception.hpp"

namespace viennautils
{
    namespace interpolate
    {
        typedef double value_type; //TODO

        struct linear_tag{};
        struct logarithmic_tag{};      //mathematically problematic: favors low values in current realization
                                       //if zero crossing of 3rd coordinate in orthogrid cell -> linear interpolation is used for this point
                                       //to avoid problems
        template < typename T>
        struct error_indicator{};

        namespace detail
        {

            typedef std::size_t index_type; //TODO

            template < typename VectorType >
            index_type find_max_idx(VectorType const & vector)
            {
                index_type max_idx = 0;
                for (index_type i = 1; i < vector.size(); i++)
                {
                    if (vector.at(i) > vector.at(max_idx))
                    {
                        max_idx = i;
                    }
                }
                return max_idx;
            }

            template < typename VectorType >
            index_type find_min_idx(VectorType const & vector)
            {
                index_type min_idx = 0;
                for (index_type i = 1; i < vector.size(); i++)
                {
                    if (vector.at(i) < vector.at(min_idx))
                    {
                        min_idx = i;
                    }
                }
                return min_idx;
            }

            template < typename VectorType >
            index_type find_upper_bound_idx(value_type value, VectorType const & vector)
            {
                index_type upper_bound_idx = find_max_idx(vector);
                if (value > vector.at(upper_bound_idx))
                {
                    throw viennautils::interpolate::value_out_of_interpolation_bounds_exception("value is larger than upper bound",
                            value, vector.at(upper_bound_idx));
                }

                for (index_type i = 0; i < vector.size(); i++)
                    if (vector.at(i) == value)
                    {
                        return (i);
                    }
                    else if (vector.at(i) > value)
                    {
                        if (fabs(vector.at(i) - value) < fabs(vector.at(upper_bound_idx) - value))
                        {
                            upper_bound_idx = i;
                        }
                    }
                return upper_bound_idx;
            }

            template < typename VectorType >
            index_type find_lower_bound_idx(value_type value, VectorType const & vector)
            {
                index_type lower_bound_idx = find_min_idx(vector);
                if (value < vector.at(lower_bound_idx))
                {
                    throw viennautils::interpolate::value_out_of_interpolation_bounds_exception("value is smaller than lower bound",
                            value, vector.at(lower_bound_idx));
                }

                for (index_type i = 0; i < vector.size(); i++)
                {
                    if (vector.at(i) == value)
                    {
                        return (i);
                    }
                    else if (vector.at(i) < value)
                    {
                        if (fabs(vector.at(i) - value) < fabs(vector.at(lower_bound_idx) - value))
                        {
                            lower_bound_idx = i;
                        }
                    }
                }
                return lower_bound_idx;
            }

            double exponential_mapping (value_type value1, value_type value2, double weight)
            {
                if (weight < 0 || weight > 1)
                {
                    throw viennautils::interpolate::invalid_parameter_exception("in exponential_mapping: weight not in range [0.0 : 1.0]", weight);
                }
                //take section of exponential function for mapping
                double alpha_l = 0.0;
                double alpha_u = 1.0;
                //
                //bool falling = false;
                //to symmetrically favor small values flip if function is falling
                if (value1 > value2)
                {
                  //  falling = true;
                    double buffer = value1;
                    value1 = value2;
                    value2 = buffer;
                    weight = 1- weight;
                }
                return (exp(weight*(alpha_u - alpha_l)+ alpha_l) - exp(alpha_l) ) / (exp(alpha_u) - exp(alpha_l) );
            }
            
            template < typename InterpolationTypeTag >
            double get_weight(value_type value, value_type entry1, value_type entry2, InterpolationTypeTag tag) {
                typedef typename error_indicator<InterpolationTypeTag>::ERROR_NO_SUCH_INTERPOLATION_TAG key;
                return 0;
            }
             

            double get_weight(value_type value, value_type entry1, value_type entry2, linear_tag tag)
            {
                double weight = (value - entry1) / (entry2 - entry1);
                if (weight < 0 || weight > 1)
                {
                    throw viennautils::interpolate::invalid_parameter_exception("weight not in range [0.0 : 1.0]", weight);
                }
                else
                {
                    return weight;
                }
            }

            double get_weight(value_type value, value_type entry1, value_type entry2, logarithmic_tag tag)
            {
                double lin_weight = get_weight(value, entry1, entry2, viennautils::interpolate::linear_tag() );
                
                double weight = -1.0;
                if (entry1 < entry2)
                {
                    weight = exponential_mapping(1.0, 0.0, lin_weight); // to activate falling criteria in exponential_mapping
                }
                else
                {
                    weight = exponential_mapping(0.0, 1.0, lin_weight);
                }

                if (weight < 0 || weight > 1)
                {
                    throw viennautils::interpolate::invalid_parameter_exception("weight not in range [0.0 : 1.0]", weight);
                }
                return weight;
            }


            template < typename InterpolationTypeTag >
            value_type calc_1D_intpol_value(value_type lower_bound_val, value_type upper_bound_val, double weight, InterpolationTypeTag tag)
            {
                typedef typename error_indicator<InterpolationTypeTag>::ERROR_NO_SUCH_INTERPOLATION_TAG key;
                return 0;
            }

            value_type calc_1D_intpol_value(value_type lower_bound_val, value_type upper_bound_val, double weight, linear_tag tag)
            {
                return upper_bound_val * weight + lower_bound_val * (1.0 - weight);
            }

            value_type calc_1D_intpol_value(value_type lower_bound_val, value_type upper_bound_val, double weight, logarithmic_tag tag)
            {
                //return exp(log(upper_bound_val) * weight + log(lower_bound_val)*(1.0 - weight));
                double logweight = exponential_mapping(lower_bound_val,upper_bound_val,weight);
                return (upper_bound_val) * logweight + (lower_bound_val)*(1.0 - logweight);
            }


            template < typename VectorType >
            index_type find_orthogrid_point_idx(value_type value1, VectorType const & vector1, value_type value2, VectorType const & vector2)
            {
                if (vector1.size() != vector2.size())
                {
                    throw viennautils::interpolate::not_the_same_size_exception("vectors are not the same size", vector1.size(), vector2.size());
                }
                for (index_type i = 0; i < vector1.size(); i++)
                {
                    if (vector1.at(i) == value1)
                    {
                        if (vector2.at(i) == value2)
                        {
                            return i;
                        }
                    }
                }
                std::stringstream messagestream;
                messagestream << "orthogrid-point not found, was lookin for point (" << value1 << " , " << value2 << ")";
                throw viennautils::interpolate::interpolation_error_exception(messagestream.str());
            }

            template < typename InterpolationTypeTag >
            value_type calc_2D_intpol_value(value_type P00, value_type P01, value_type P10, value_type P11, double weight1, double weight2,
                                            InterpolationTypeTag tag)
            {
                typedef typename error_indicator<InterpolationTypeTag>::ERROR_NO_SUCH_INTERPOLATION_TAG key;
                return 0;
            }

            value_type calc_2D_intpol_value(value_type P00, value_type P01, value_type P10, value_type P11, double weight1, double weight2,
                                            linear_tag tag)
            {
                return P00 * (1 - weight1)*(1 - weight2) + P10 * (weight1)*(1 - weight2) + P01 * (1 - weight1)*(weight2) + P11 * (weight1)*(weight2);
            }

            value_type calc_2D_intpol_value(value_type P00, value_type P01, value_type P10, value_type P11, double weight1, double weight2,
                                            logarithmic_tag tag)
            {
                //sign specific calculation
                if (P00 >0 && P01 >0 && P10 >0 && P11 >0) //all positive
                {
                return exp(log(P00)*(1 - weight1)*(1 - weight2) + log(P10)*(weight1)*(1 - weight2) + log(P01)*(1 - weight1)*(weight2)
                        + log(P11)*(weight1)*(weight2));
                }
                else if (P00 <0 && P01 <0 && P10 <0 && P11 <0) //all negative
                {
                return -1.0* exp(log(abs(P00))*(1 - weight1)*(1 - weight2) + log(abs(P10))*(weight1)*(1 - weight2) + log(abs(P01))*(1 - weight1)*(weight2)
                        + log(abs(P11))*(weight1)*(weight2));
                }
                else //if there is some zero crossing in this cell use linear interpolation
                {
                    return calc_2D_intpol_value(P00,P01,P10,P11,weight1,weight2,viennautils::interpolate::linear_tag() );
                }
            }

            template < typename VectorType >
            bool check_ambiguiuity(VectorType const & vector1, VectorType const & vector2, bool throws_exception = true)
            {

                if (vector1.size() != vector2.size())
                {
                    throw viennautils::interpolate::not_the_same_size_exception("vectors are not the same size", vector1.size(), vector2.size());
                }

                for (index_type i = 0; i < vector1.size() - 1; i++)
                {
                    for (index_type j = i + 1; j < vector1.size(); j++)
                    {
                        if (vector1.at(i) == vector1.at(j) && vector2.at(i) != vector2.at(j))
                        {
                            if (throws_exception == true)
                            {
                                std::stringstream messagestream;
                                messagestream << "encountered ambiguous orthogrid point, point beeing (" << vector1.at(i) << " , " << vector2.at(i) << ")";
                                throw viennautils::interpolate::interpolation_error_exception(messagestream.str());
                            }
                            return false;
                        }
                    }
                }
                return true;
            }

            template < typename VectorType >
            bool check_ambiguiuity(VectorType const & vector1, VectorType const & vector2, VectorType const & vector3,
                                   bool throws_exception = true)
            {

                if (vector1.size() != vector2.size())
                {
                    throw viennautils::interpolate::not_the_same_size_exception("vectors are not the same size", vector1.size(), vector2.size());
                }
                if (vector2.size() != vector3.size())
                {
                    throw viennautils::interpolate::not_the_same_size_exception("vectors are not the same size", vector2.size(), vector3.size());
                }
                for (index_type i = 0; i < vector1.size() - 1; i++)
                {
                    for (index_type j = i + 1; j < vector1.size(); j++)
                    {
                        if (vector1.at(i) == vector1.at(j) && vector2.at(i) == vector2.at(j) && vector3.at(i) != vector3.at(j))
                        {
                            if (throws_exception == true)
                            {
                                std::stringstream messagestream;
                                messagestream << "encountered ambiguous orthogrid point, point beeing (" << vector1.at(i) << " , " << vector2.at(i) << ")";
                                throw viennautils::interpolate::interpolation_error_exception(messagestream.str());
                            }
                            return false;
                        }
                    }
                }
                return true;
            }
        }//detail

        template < typename VectorType, typename InterpolationTypeTag_v1, typename InterpolationTypeTag_v2>
        value_type interpolate_1D(typename VectorType::value_type value, VectorType const & v1, VectorType const & v2,
                                  InterpolationTypeTag_v1 v1_intpol_type, InterpolationTypeTag_v2 v2_intpol_type)
        {
            if (v1.size() != v2.size())
            {
                throw viennautils::interpolate::not_the_same_size_exception("vectors are not the same size", v1.size(), v2.size());
            }
            detail::check_ambiguiuity(v1, v2, true);
            std::size_t lb_idx = detail::find_lower_bound_idx(value, v1);
            std::size_t ub_idx = detail::find_upper_bound_idx(value, v1);

            if (ub_idx == lb_idx)
            {
                return v2.at(ub_idx);
            }

            double weight = detail::get_weight(value, v1.at(lb_idx), v1.at(ub_idx), v1_intpol_type);

            return detail::calc_1D_intpol_value(v2.at(lb_idx), v2.at(ub_idx), weight, v2_intpol_type);
        }

        template < typename VectorType >
        struct interpolator1D
        {
            typedef typename VectorType::value_type value_type;

            interpolator1D(VectorType const & x_values, VectorType const & y_values)
            : x_values_(x_values), y_values_(y_values) { }

            template < typename InterpolationTypeTag_x, typename InterpolationTypeTag_y >
                    value_type operator()(value_type x, InterpolationTypeTag_x tag_x, InterpolationTypeTag_y tag_y) const
            {
                typedef typename error_indicator<InterpolationTypeTag_x>::ERROR_NO_SUCH_INTERPOLATION_TAG key1;
                typedef typename error_indicator<InterpolationTypeTag_y>::ERROR_NO_SUCH_INTERPOLATION_TAG key2;
                return 0;
            }

            value_type operator()(value_type x, linear_tag tag_x, linear_tag tag_y) const
            {
                return interpolate_1D(x, x_values_, y_values_, tag_x, tag_y);
            }

            value_type operator()(value_type x, linear_tag tag_x, logarithmic_tag tag_y) const
            {
                return interpolate_1D(x, x_values_, y_values_, tag_x, tag_y);
            }

            value_type operator()(value_type x, logarithmic_tag tag_x, logarithmic_tag tag_y) const
            {
                return interpolate_1D(x, x_values_, y_values_, tag_x, tag_y);
            }

            value_type operator()(value_type x, logarithmic_tag tag_x, linear_tag tag_y) const
            {
                return interpolate_1D(x, x_values_, y_values_, tag_x, tag_y);
            }

            value_type operator()(value_type x) const
            {
                return this->operator ()(x, linear_tag(), linear_tag());
            }

        private:
            VectorType const & x_values_;
            VectorType const & y_values_;
        };

        template < typename VectorType, typename InterpolationTypeTag_v1 , typename InterpolationTypeTag_v2, typename InterpolationTypeTag_v3>
        value_type interpolate_2D(typename VectorType::value_type value1, VectorType const & v1, typename VectorType::value_type value2,
                                  VectorType const & v2, VectorType const & v3, InterpolationTypeTag_v1 v1_intpol_type,
                                  InterpolationTypeTag_v2 v2_intpol_type, InterpolationTypeTag_v3 v3_intpol_type)
        {
            if (v1.size() != v2.size())
            {
                throw viennautils::interpolate::not_the_same_size_exception("vectors are not the same size", v1.size(), v2.size());
            }
            if (v2.size() != v3.size())
            {
                throw viennautils::interpolate::not_the_same_size_exception("vectors are not the same size", v2.size(), v3.size());
            }

            detail::check_ambiguiuity(v1, v2, v3, true);

            std::size_t lb1_idx = detail::find_lower_bound_idx(value1, v1);
            std::size_t ub1_idx = detail::find_upper_bound_idx(value1, v1);
            std::size_t lb2_idx = detail::find_lower_bound_idx(value2, v2);
            std::size_t ub2_idx = detail::find_upper_bound_idx(value2, v2);

            value_type lb1_val = v1.at(lb1_idx);
            value_type ub1_val = v1.at(ub1_idx);
            value_type lb2_val = v2.at(lb2_idx);
            value_type ub2_val = v2.at(ub2_idx);

            //find the 4 points on orthogrid
            //        P01 x--------------------x P11
            //            |                    |
            //            |     x              |
            //            |                    |
            //            |                    |
            //        P00 x--------------------x P10

            value_type P00 = v3.at(detail::find_orthogrid_point_idx(lb1_val, v1, lb2_val, v2));
            value_type P10 = v3.at(detail::find_orthogrid_point_idx(ub1_val, v1, lb2_val, v2));
            value_type P01 = v3.at(detail::find_orthogrid_point_idx(lb1_val, v1, ub2_val, v2));
            value_type P11 = v3.at(detail::find_orthogrid_point_idx(ub1_val, v1, ub2_val, v2));

            if (P00 == P01 && P01 == P11)
            {
                return P00;
            }

            if (P00 == P10)
            {
                double weight = detail::get_weight(value2, lb2_val, ub2_val, v2_intpol_type);
                return detail::calc_1D_intpol_value(P00, P01, weight, v3_intpol_type);
            }
            if (P00 == P01)
            {

                double weight = detail::get_weight(value1, lb1_val, ub1_val, v1_intpol_type);
                return detail::calc_1D_intpol_value(P00, P10, weight, v3_intpol_type);
            }

            double weight1 = detail::get_weight(value1, lb1_val, ub1_val, v1_intpol_type);
            double weight2 = detail::get_weight(value2, lb2_val, ub2_val, v2_intpol_type);

            return detail::calc_2D_intpol_value(P00, P01, P10, P11, weight1, weight2, v3_intpol_type);
        }

        template < typename VectorType >
        struct interpolator2D
        {
            typedef typename VectorType::value_type value_type;

            interpolator2D(VectorType const & x_values, VectorType const & y_values, VectorType const & z_values) : x_values_(x_values),
            y_values_(y_values), z_values_(z_values) { }

            template < typename InterpolationTypeTag_x, typename InterpolationTypeTag_y, typename InterpolationTypeTag_z >
                    value_type operator()(value_type x, value_type y, InterpolationTypeTag_x tag_x, InterpolationTypeTag_y tag_y,
                                          InterpolationTypeTag_z tag_z) const
            {
                typedef typename error_indicator<InterpolationTypeTag_x>::ERROR_NO_SUCH_INTERPOLATION_TAG key1;
                typedef typename error_indicator<InterpolationTypeTag_y>::ERROR_NO_SUCH_INTERPOLATION_TAG key2;
                typedef typename error_indicator<InterpolationTypeTag_z>::ERROR_NO_SUCH_INTERPOLATION_TAG key3;
                return 0;
            }

            value_type operator()(value_type x, value_type y, linear_tag tag_x, linear_tag tag_y, linear_tag tag_z) const
            {
                return interpolate_2D(x, x_values_, y, y_values_, z_values_, tag_x, tag_y, tag_z);
            }

            value_type operator()(value_type x, value_type y, linear_tag tag_x, logarithmic_tag tag_y, linear_tag tag_z) const
            {
                return interpolate_2D(x, x_values_, y, y_values_, z_values_, tag_x, tag_y, tag_z);
            }

            value_type operator()(value_type x, value_type y, linear_tag tag_x, linear_tag tag_y, logarithmic_tag tag_z) const
            {
                return interpolate_2D(x, x_values_, y, y_values_, z_values_, tag_x, tag_y, tag_z);
            }

            value_type operator()(value_type x, value_type y, linear_tag tag_x, logarithmic_tag tag_y, logarithmic_tag tag_z) const
            {
                return interpolate_2D(x, x_values_, y, y_values_, z_values_, tag_x, tag_y, tag_z);
            }

            value_type operator()(value_type x, value_type y, logarithmic_tag tag_x, linear_tag tag_y, linear_tag tag_z) const
            {
                return interpolate_2D(x, x_values_, y, y_values_, z_values_, tag_x, tag_y, tag_z);
            }

            value_type operator()(value_type x, value_type y, logarithmic_tag tag_x, logarithmic_tag tag_y, linear_tag tag_z) const
            {
                return interpolate_2D(x, x_values_, y, y_values_, z_values_, tag_x, tag_y, tag_z);
            }

            value_type operator()(value_type x, value_type y, logarithmic_tag tag_x, linear_tag tag_y, logarithmic_tag tag_z) const
            {
                return interpolate_2D(x, x_values_, y, y_values_, z_values_, tag_x, tag_y, tag_z);
            }

            value_type operator()(value_type x, value_type y, logarithmic_tag tag_x, logarithmic_tag tag_y, logarithmic_tag tag_z) const
            {
                return interpolate_2D(x, x_values_, y, y_values_, z_values_, tag_x, tag_y, tag_z);
            }

            value_type operator()(value_type x, value_type y) const
            {
                return this->operator ()(x, y, linear_tag(), linear_tag(),
                        linear_tag());
            }

        private:
            VectorType const & x_values_;
            VectorType const & y_values_;
            VectorType const & z_values_;
        };


    }//interpolate
} // viennautils
#endif	/* VIENNAUTILS_INTERPOLATE_ORTHOGRID_INTERPOLATE_HPP */

