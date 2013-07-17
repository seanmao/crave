#pragma once

#include <boost/proto/core.hpp>
#include <ostream>

using std::ostream;

namespace crave {
  namespace bproto = boost::proto;

  struct Constraint_Domain;

  struct Constraint_Grammar
  : bproto::_
  {};

  template<typename T>
  class randv;

  template<typename T>
  struct Variable;

  template<typename Expr>
  struct Constraint
  : bproto::extends<Expr, Constraint<Expr>, Constraint_Domain >
  {
    typedef bproto::extends<Expr, Constraint<Expr>, Constraint_Domain > base_type;

    Constraint(Expr const & expr = Expr())
      : base_type(expr)
    {
    }


   template<typename T, typename E2>
   typename bproto::result_of::make_expr<
          bproto::tag::assign
        , Constraint_Domain
        , Expr
        , E2
      > ::type
      operator= ( E2 e ) {
      return bproto::make_expr< bproto::tag::assign>
        ( *this, e);
      }
  };


  struct Constraint_Domain
  : bproto::domain<bproto::generator<Constraint>, Constraint_Grammar>
  {};

   template<typename Expr>
   void check (Constraint<Expr> const & ) {
     BOOST_MPL_ASSERT((bproto::matches<Expr, Constraint_Grammar>));
   }

   template<typename Expr>
   void check_not (Constraint<Expr> const & ) {
     BOOST_MPL_ASSERT_NOT((bproto::matches<Expr, Constraint_Grammar>));
   }


  template<typename value_type>
  struct var_tag { var_tag(int id_): id(id_) { }; int id; };

  template<typename value_type>
  struct vector_tag { vector_tag(int id_): id(id_) { }; int id; };

  struct placeholder_tag { placeholder_tag(int id_): id(id_) { }; int id; };

  template<typename value_type>
  struct write_ref_tag : public var_tag<value_type> {
  write_ref_tag(int id_,  value_type & ref_) : var_tag<value_type>(id_), ref(ref_) { }; value_type & ref; };

  template<typename value_type>
  struct read_ref_tag : public var_tag<value_type> {
  read_ref_tag(int id_,  value_type const & ref_) : var_tag<value_type>(id_), ref(ref_) { }; value_type const & ref; };

  struct operator_inside {};
  struct operator_dist {};
  struct operator_if_then {};
  struct operator_if_then_else : operator_if_then {};

  template <typename OUT, typename value_type>
  OUT & operator<< (OUT & out, var_tag<value_type> const & tag) {
    out << "var<" << tag.id << ">" ;
    return out;
  }

  template <typename OUT, typename value_type>
  OUT & operator<< (OUT & out, read_ref_tag<value_type> const & tag) {
    out << "read_ref<" << tag.id << ">" ;
    return out;
  }

  template <typename OUT, typename value_type>
  OUT & operator<< (OUT & out, write_ref_tag<value_type> const & tag) {
    out << "write_ref<" << tag.id << ">" ;
    return out;
  }

  template <typename OUT, typename value_type>
  OUT & operator<< (OUT & out, vector_tag<value_type> const & tag) {
    out << "vector<" << tag.id << ">" ;
    return out;
  }

  template <typename OUT>
  OUT & operator<< (OUT & out, placeholder_tag const & tag) {
    out << "placeholder<" << tag.id << ">" ;
    return out;
  }

  template <typename OUT>
  OUT & operator<< (OUT & out, operator_inside const & tag ) {
    return out << "inside"  ;
  }

  template <typename OUT>
  OUT & operator<< (OUT & out, operator_dist const & tag ) {
    return out << "dist"  ;
  }

  template <typename OUT>
  OUT & operator<< (OUT & out, operator_if_then const & tag ) {
   return out << "if_then"  ;
  }

  template <typename OUT>
  OUT & operator<< (OUT & out, operator_if_then_else const & tag ) {
   return out << static_cast<operator_if_then>(tag) << "_else"  ;
  }

  int new_var_id();

  template<typename value_type_>
  struct Variable : public Constraint< typename bproto::terminal< var_tag<value_type_> >::type >
  {
    typedef Constraint< typename bproto::terminal< var_tag<value_type_> >::type > base_type;
    Variable()
      : base_type( bproto::make_expr< bproto::tag::terminal>
        ( var_tag<value_type_>(new_var_id() ) ) )
    {}

    template<typename T>
    explicit Variable( T linkto )
    : Variable()
    { linkto.addVariable(*this); }

    typedef value_type_ value_type;
    int id() const {  return bproto::value(*this).id; };

   typename bproto::result_of::make_expr<
          bproto::tag::assign
        , Constraint_Domain
        , Variable<value_type_> const &
        , value_type_ const &
      > ::type const
      operator= ( value_type_ const & e ) const {
      return bproto::make_expr< bproto::tag::assign, Constraint_Domain>
        ( boost::cref(*this), boost::cref(e));
      }
  };

  template<typename value_type_>
  struct Vector : public Constraint< typename bproto::terminal< vector_tag<value_type_> >::type >
  {
    typedef Constraint< typename bproto::terminal< vector_tag<value_type_> >::type > base_type;
    Vector()
      : base_type( bproto::make_expr< bproto::tag::terminal>
        ( vector_tag<value_type_>(new_var_id() ) ) )
    {}

    typedef value_type_ value_type;
    int id() const {  return bproto::value(*this).id; };

    Variable<unsigned int> _size;
    const Variable<unsigned int>& size() const { return _size; }
  };

  struct placeholder : public Constraint< bproto::terminal<placeholder_tag>::type >
  {
    typedef Constraint< bproto::terminal< placeholder_tag >::type > base_type;
    placeholder()
      : base_type( bproto::make_expr<bproto::tag::terminal>(placeholder_tag(new_var_id())) )
    {}

    int id() const { return bproto::value(*this).id; }
  };

  template<typename value_type_>
  struct WriteReference : public Constraint< typename boost::proto::terminal< write_ref_tag<value_type_> >::type >
  {
    typedef Constraint< typename bproto::terminal< write_ref_tag<value_type_> >::type > base_type;
    WriteReference(value_type_ & ref)
      : base_type( bproto::make_expr< bproto::tag::terminal>
        ( write_ref_tag<value_type_>(new_var_id(), ref ) ) )
    {}
    typedef value_type_ value_type;
    int id() const {  return boost::proto::value(*this).id; };

   typename bproto::result_of::make_expr<
          bproto::tag::assign
        , Constraint_Domain
        , WriteReference<value_type_> const &
        , value_type_ const &
      > ::type const
      operator= ( value_type_ const & e ) const {
      return bproto::make_expr< bproto::tag::assign, Constraint_Domain>
        ( boost::cref(*this), boost::cref(e));
      }
  };


  template<typename value_type_>
  struct ReadReference : public Constraint< typename boost::proto::terminal< read_ref_tag<value_type_> >::type >
  {
    typedef Constraint< typename bproto::terminal< read_ref_tag<value_type_> >::type > base_type;
    ReadReference(value_type_ &  ref)
      : base_type( bproto::make_expr< bproto::tag::terminal>
        ( read_ref_tag<value_type_>(new_var_id(), ref) ) )
    {}
    typedef value_type_ value_type;
    int id() const {  return boost::proto::value(*this).id; };
  };

      template<typename T>
      typename bproto::result_of::make_expr<
          bproto::tag::terminal
        , Constraint_Domain
        , read_ref_tag<T>
      > ::type
      reference( T const &  ref )
      {
        return bproto::make_expr< bproto::tag::terminal, Constraint_Domain >( read_ref_tag<T>( new_var_id(), ref )) ;
      }



  boost::proto::terminal<operator_inside>::type const inside = {};
  boost::proto::terminal<operator_dist>::type const dist = {};
  boost::proto::terminal<operator_if_then>::type const if_then = {};
  boost::proto::terminal<operator_if_then_else>::type const if_then_else = {};
} // namespace crave

//  vim: ft=cpp:ts=2:sw=2:expandtab

