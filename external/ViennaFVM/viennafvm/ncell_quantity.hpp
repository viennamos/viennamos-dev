#ifndef VIENNAFVM_CELL_QUAN_HPP
#define VIENNAFVM_CELL_QUAN_HPP

/* =======================================================================
   Copyright (c) 2011, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at
                             -----------------
           ViennaFVM - The Vienna Finite Volume Method Library
                             -----------------

   authors:    Karl Rupp                          rupp@iue.tuwien.ac.at
               (add your name here)

   license:    To be discussed, see file LICENSE in the ViennaFVM base directory
======================================================================= */

#include "viennafvm/forwards.h"

#include "viennamath/forwards.h"
#include "viennamath/manipulation/substitute.hpp"
#include "viennamath/expression.hpp"
#include "viennadata/api.hpp"

#include "viennagrid/forwards.hpp"
#include "viennagrid/mesh/segmentation.hpp"

/** @file  ncell_quantity.hpp
    @brief Defines ViennaMath extensions: Piecewise constants (constants on each cell) and flux evaluators on interfaces
*/

namespace viennafvm
{
  namespace detail
  {
    /** @brief The runtime interface for cell quantities.
    *
    * @param CellType    Type of the ViennaGrid cell
    * @param NumericT    Floating point type of the value to be returned (typically 'double')
    */
    template <typename CellType, typename NumericT = viennafvm::numeric_type>
    class ncell_quantity_interface
    {
      protected:
        typedef NumericT          numeric_type;

      public:
        virtual numeric_type eval(CellType const & cell, numeric_type v) const = 0;
        virtual numeric_type eval(CellType const & cell, std::vector<numeric_type> const & v) const = 0;

        virtual ncell_quantity_interface<CellType, NumericT> * clone() const = 0;
    };

    /** @brief Implementation of a function which is piecewise constant on each cell. Function values are retrieved from ViennaData.
    *
    * @param CellType    Type of the ViennaGrid cell
    * @param KeyType     The key type to be used with ViennaData
    * @param DataType    The data type to be used with ViennaData
    */
    template <typename CellType, typename AccessorType>
    class ncell_quantity_constant : public ncell_quantity_interface<CellType>
    {
        typedef ncell_quantity_constant<CellType, AccessorType>        self_type;
        typedef typename ncell_quantity_interface<CellType>::numeric_type    numeric_type;

      public:
        ncell_quantity_constant(AccessorType const & accessor_) : accessor(accessor_) {}

        numeric_type eval(CellType const & cell, numeric_type /*v*/) const
        {
          return accessor(cell);
        }

        numeric_type eval(CellType const & cell, std::vector<numeric_type> const & /*v*/) const
        {
          return accessor(cell);
        }

        ncell_quantity_interface<CellType> * clone() const { return new self_type(accessor); }

      private:
        AccessorType accessor;
    };



    /** @brief A type erasure class which enables to store cell_quan_constants and cell_quan_exprs with different template arguments in a single array.
    *
    * @param CellType    Type of the ViennaGrid cell
    * @param NumericT    Floating point type of the value to be returned (typically 'double')
    */
    template <typename CellType, typename NumericT = viennafvm::numeric_type>
    class ncell_quantity_wrapper
    {
      public:
        template <typename T>
        ncell_quantity_wrapper(T const * t) : functor_(t) {}

        ncell_quantity_wrapper() {}

        ncell_quantity_wrapper & operator=(ncell_quantity_wrapper & other)
        {
          functor_ = other.functor_;
          return *this;
        }

        NumericT eval(CellType const & cell,
                      numeric_type v) const
        {
          return functor_->eval(cell, v);
        }

        NumericT eval(CellType const & cell,
                      std::vector<numeric_type> const & v) const
        {
          return functor_->eval(cell, v);
        }

        ncell_quantity_interface<CellType> * clone() const { return functor_->clone(); }

      private:
        std::auto_ptr< const ncell_quantity_interface<CellType> > functor_;
    };

  } //namespace detail


  /** @brief The main cell quantity class for using piecewise constant or piecewise expressions (in local coordinates) with ViennaMath.
   *
    * @param CellType       Type of the ViennaGrid cell
    * @param InterfaceType  The runtime interface class of ViennaMath.
   */
  template <typename CellType, typename InterfaceType>
  class ncell_quantity : public InterfaceType
  {
      typedef ncell_quantity<CellType, InterfaceType>     self_type;
      typedef typename InterfaceType::numeric_type            numeric_type;
    public:

      explicit ncell_quantity(CellType const * cell, detail::ncell_quantity_wrapper<CellType, numeric_type> const & wrapper) : current_cell(cell), accessor(wrapper.clone()) {}

      //template <typename T>
      //explicit cell_quan(T const & t) : current_cell(NULL), accessor( new quan_accessor<CellType, T, numeric_type>() ) {}

      explicit ncell_quantity() : current_cell(NULL) {}

      //interface requirements:
      InterfaceType * clone() const { return new self_type(current_cell, accessor); }
      numeric_type eval(std::vector<numeric_type> const & v) const
      {
        return accessor.eval(*current_cell, v);
      }
      numeric_type eval(numeric_type v) const
      {
        return accessor.eval(*current_cell, v);
      }

      std::string deep_str() const
      {
        std::stringstream ss;
        ss << "cell_quan<" << CellType::tag::dim << ">(" << current_cell << ")";
        return ss.str();
      }
      numeric_type unwrap() const { throw "Cannot evaluate unknown_func!"; }

      InterfaceType * substitute(const InterfaceType * e,
                                 const InterfaceType * repl) const
      {
        if (deep_equal(e))
          return repl->clone();

        return clone();
      }

      InterfaceType * substitute(std::vector<const InterfaceType *> const &  e,
                                 std::vector<const InterfaceType *> const &  repl) const
      {
        //std::cout << "Comparing variable<" << id << "> with " << e->str() << ", result: ";
        for (std::size_t i=0; i<e.size(); ++i)
          if (deep_equal(e[i]))
            return repl[i]->clone();

        //std::cout << "FALSE" << std::endl;
        return clone();
      };

      bool deep_equal(const InterfaceType * other) const
      {
        //TODO: Include comparison of accessor
        return dynamic_cast< const self_type *>(other) != NULL;
      }

      bool shallow_equal(const InterfaceType * other) const
      {
        return dynamic_cast< const self_type *>(other) != NULL;
      }

      InterfaceType * diff(const InterfaceType * /*diff_var*/) const
      {
        throw "Cannot differentiate cell_quan!";
        return NULL;
      }


      //additional members:
      void update(CellType const & cell) const
      {
        current_cell = &cell;
      }

      template <typename StorageType, typename KeyType>
      void wrap_constant(StorageType & storage, KeyType const & k)
      {
        detail::ncell_quantity_wrapper<CellType, numeric_type> temp(
          new detail::ncell_quantity_constant<CellType, typename viennadata::result_of::accessor<StorageType, KeyType, numeric_type, CellType>::type >(
            viennadata::make_accessor<KeyType, numeric_type, CellType>(storage, k)
          )
        );
        accessor = temp;
      }

      detail::ncell_quantity_wrapper<CellType, numeric_type> const & wrapper() const { return accessor; }

    private:
      mutable const CellType * current_cell;
      detail::ncell_quantity_wrapper<CellType, numeric_type> accessor;
  };

  //TODO: Check whether cell_quan can be injected directly into existing ViennaMath overloads

  /** @brief Operator overload for the multiplication of a cell quantity with a ViennaMath variable */
  template <typename CellType, typename InterfaceType>
  viennamath::rt_expr<InterfaceType> operator*(viennamath::rt_variable<InterfaceType> const & lhs,
                                               ncell_quantity<CellType, InterfaceType> const & rhs)
  {
    return viennamath::rt_expr<InterfaceType>(new viennamath::rt_binary_expr<InterfaceType>(lhs.clone(),
                                                            new viennamath::op_binary<viennamath::op_mult<viennamath::default_numeric_type>, InterfaceType >(),
                                                            rhs.clone()));
  }


  /** @brief Operator overload for the multiplication of a cell quantity with a ViennaMath expression wrapper */
  template <typename CellType, typename InterfaceType>
  viennamath::rt_expr<InterfaceType> operator*(viennamath::rt_expr<InterfaceType> const & lhs,
                                               ncell_quantity<CellType, InterfaceType> const & rhs)
  {
    return viennamath::rt_expr<InterfaceType>(new viennamath::rt_binary_expr<InterfaceType>(lhs.get()->clone(),
                                                            new viennamath::op_binary<viennamath::op_mult<viennamath::default_numeric_type>, InterfaceType >(),
                                                            rhs.clone()));
  }

  /** @brief Operator overload for the multiplication of a cell quantity with a ViennaMath unary expression */
  template <typename CellType, typename InterfaceType>
  viennamath::rt_expr<InterfaceType> operator*(ncell_quantity<CellType, InterfaceType> const & lhs,
                                               viennamath::rt_unary_expr<InterfaceType> const & rhs
                               )
  {
    return viennamath::rt_expr<InterfaceType>(new viennamath::rt_binary_expr<InterfaceType>(lhs.clone(),
                                                            new viennamath::op_binary<viennamath::op_mult<viennamath::default_numeric_type>, InterfaceType >(),
                                                            rhs.clone()));
  }

  /** @brief Operator overload for the multiplication of a cell quantity with a ViennaMath binary expression */
  template <typename CellType, typename InterfaceType>
  viennamath::rt_expr<InterfaceType> operator*(ncell_quantity<CellType, InterfaceType> const & lhs,
                                               viennamath::rt_binary_expr<InterfaceType> const & rhs
                               )
  {
    return viennamath::rt_expr<InterfaceType>(new viennamath::rt_binary_expr<InterfaceType>(lhs.clone(),
                                                            new viennamath::op_binary<viennamath::op_mult<viennamath::default_numeric_type>, InterfaceType >(),
                                                            rhs.clone()));
  }

  /** @brief Operator overload for the multiplication of a cell quantity with a ViennaMath expression wrapper */
  template <typename CellType, typename InterfaceType>
  viennamath::rt_expr<InterfaceType> operator/(viennamath::rt_expr<InterfaceType> const & lhs,
                                               ncell_quantity<CellType, InterfaceType> const & rhs)
  {
    return viennamath::rt_expr<InterfaceType>(new viennamath::rt_binary_expr<InterfaceType>(lhs.get()->clone(),
                                                            new viennamath::op_binary<viennamath::op_div<viennamath::default_numeric_type>, InterfaceType >(),
                                                            rhs.clone()));
  }

  //
  // Convenience setter
  //

  //
  // quantity region:
  //

  namespace detail
  {
    template <typename DomainSegmentType, typename AccessorType>
    void set_quantity_region_impl(DomainSegmentType const & seg,
                                  AccessorType accessor,
                                  bool b)
    {
      typedef typename viennagrid::result_of::cell_tag<DomainSegmentType>::type CellTag;

      typedef typename viennagrid::result_of::element<DomainSegmentType, CellTag>::type               CellType;
      typedef typename viennagrid::result_of::const_element_range<DomainSegmentType, CellTag>::type  CellContainer;
      typedef typename viennagrid::result_of::iterator<CellContainer>::type                       CellIterator;

      CellContainer cells(seg);
      for (CellIterator cit  = cells.begin();
                        cit != cells.end();
                      ++cit)
      {
        accessor(*cit) = b;
      }
    }

  }

  template <typename DomainSegmentType, typename AccessorType>
  void set_quantity_region(DomainSegmentType const & domseg,
                           AccessorType accessor,
                           bool b)
  {
    detail::set_quantity_region_impl(domseg, accessor, b);
  }

  template <typename DomainSegmentType, typename StorageType, typename KeyType>
  void set_quantity_region(DomainSegmentType const & domseg,
                           StorageType & storage,
                           KeyType const & key,
                           bool b)
  {
    typedef typename viennagrid::result_of::cell<DomainSegmentType>::type CellType;
    set_quantity_region(domseg, viennadata::make_accessor<KeyType, bool, CellType>(storage, key), b);
  }


  //
  // quantity value:
  //

  namespace detail
  {
    template <typename DomSegType, typename AccessorType>
    void set_quantity_value_impl(DomSegType const & seg,
                                 AccessorType accessor,
                                 numeric_type val)
    {
      typedef typename viennagrid::result_of::cell_tag<DomSegType>::type CellTag;

      typedef typename viennagrid::result_of::element<DomSegType, CellTag>::type               CellType;
      typedef typename viennagrid::result_of::const_element_range<DomSegType, CellTag>::type  CellContainer;
      typedef typename viennagrid::result_of::iterator<CellContainer>::type                       CellIterator;

      CellContainer cells(seg);
      for (CellIterator cit  = cells.begin();
                        cit != cells.end();
                      ++cit)
      {
        accessor(*cit) = val;
      }
    }

  }

  template <typename DomainSegmentType, typename AccessorType>
  void set_quantity_value(DomainSegmentType const & domseg,
                          AccessorType accessor,
                          numeric_type val)
  {
    detail::set_quantity_value_impl(domseg, accessor, val);
  }

  template <typename DomainSegmentType, typename StorageType, typename KeyType>
  void set_quantity_value(DomainSegmentType const & domseg,
                           StorageType & storage,
                           KeyType const & key,
                           numeric_type val)
  {
    typedef typename viennagrid::result_of::cell<DomainSegmentType>::type CellType;
    set_quantity_value(domseg, viennadata::make_accessor<KeyType, numeric_type, CellType>(storage, key), val);
  }

}


// generic clone:
namespace viennamath
{

  /** @brief Any generic free functions for unifying interfaces are defined here. */
  namespace traits
  {
    template <typename InterfaceType, typename CellType>
    InterfaceType * clone(viennafvm::ncell_quantity<CellType, InterfaceType> const & c) { return c.clone(); }
  }
}


#endif
