#pragma once

#include "ConstrainedRandom.hpp"
#include "expression/Node.hpp"
#include "expression/ReplaceVisitor.hpp"

#include <boost/intrusive_ptr.hpp>
#include <boost/foreach.hpp>
#include <boost/fusion/adapted/std_pair.hpp>

#include <ostream>
#include <string>
#include <vector>

namespace crave {

struct UserConstraint {

  typedef boost::intrusive_ptr<Node> expression;
  typedef std::string string;

  UserConstraint(expression const expr, string const& name)
  : expr_(expr), name_(name), soft_(false), enabled_(true) { }
  UserConstraint(expression const expr, string const& name, bool const soft)
  : expr_(expr), name_(name), soft_(soft), enabled_(true) { }
  UserConstraint(expression const expr, string const& name, bool const soft, bool const enabled)
  : expr_(expr), name_(name), soft_(soft), enabled_(enabled) { }
  UserConstraint(UserConstraint const& other)
  : expr_(other.get_expression()), name_(other.get_name()), soft_(other.is_soft()),
    enabled_(other.is_enabled()) { }

  inline expression const & get_expression() const {
    return expr_;
  }

  inline string get_name() const {
    return name_;
  }

  inline bool is_soft() const {
    return soft_;
  }

  inline bool is_enabled() const {
    return enabled_;
  }
  inline void enable() {
    enabled_ = true;
  }
  inline void disable() {
    enabled_ = false;
  }

 protected:
  expression expr_;
  string name_;
  bool soft_;
  bool enabled_;
};

struct VectorConstraint : UserConstraint {

  typedef UserConstraint::expression expression;
  typedef UserConstraint::string string;

  typedef std::vector<expression> ExpressionsVector;
  typedef ExpressionsVector::const_iterator const_iterator;

  typedef boost::intrusive_ptr<VariableExpr> VariablePtr;
  typedef std::vector<VariablePtr> VectorElements;

  VectorConstraint(expression const expr, string const& name)
  : UserConstraint(expr, name), vec_expressions_() { }
  VectorConstraint(expression const expr, string const& name, bool const soft)
  : UserConstraint(expr, name, soft), vec_expressions_() { }
  VectorConstraint(expression const expr, string const& name, bool const soft,
                   bool const enabled)
  : UserConstraint(expr, name, soft, enabled), vec_expressions_() { }
  VectorConstraint(VectorConstraint const& o)
  : UserConstraint(o), vec_expressions_(o.vec_expressions_) { }

  ExpressionsVector& get_exprs() const {
    return vec_expressions_;
  }

private:
  // auxiliary variables
  mutable ExpressionsVector vec_expressions_;
};

template<typename ConstraintType>
struct ConstraintSet {

  typedef std::string string;
  typedef std::vector<ConstraintType> ConstraintsVector;
  typedef typename ConstraintsVector::iterator iterator;
  typedef typename ConstraintsVector::const_iterator const_iterator;
  typedef typename ConstraintsVector::reference reference;
  typedef typename ConstraintsVector::const_reference const_reference;
  typedef typename ConstraintsVector::size_type size_type;
  typedef typename ConstraintsVector::value_type value_type;

  typedef boost::intrusive_ptr<VariableExpr> VariablePtr;
  typedef std::vector<VariablePtr> VectorElements;

  ConstraintSet()
  : constraints_(), changed_(false), unique_(false), vec_elements_() { }

  reference operator[](size_type n) {
    changed_ = true;
    return constraints_[n];
  }
  const_reference operator[](size_type n) const {
    return constraints_[n];
  }
  reference at(size_type n) {
    changed_ = true;
    return constraints_.at(n);
  }
  const_reference at(size_type n) const {
    return constraints_.at(n);
  }

  iterator begin() {
    changed_ = true;
    return constraints_.begin();
  }
  const_iterator begin() const {
    return constraints_.begin();
  }
  iterator end() {
    changed_ = true;
    return constraints_.end();
  }
  const_iterator end() const {
    return constraints_.end();
  }

  void push_back(value_type const& value) {
    changed_ = true;
    constraints_.push_back(value);
  }
  void pop_back() {
    changed_ = true;
    constraints_.pop_back();
  }

  iterator insert(iterator position, value_type const& value) {
    changed_ = true;
    return constraints_.insert(position, value);
  }
  template<typename InputIterator>
  void insert(iterator position, InputIterator first, InputIterator last) {
    changed_ = true;
    constraints_.insert(position, first, last);
  }

  iterator erase(iterator position) {
    changed_ = true;
    return constraints_.erase(position);
  }
  iterator erase(iterator first, iterator last) {
    changed_ = true;
    return constraints_.erase(first, last);
  }

  void clear() {
    changed_ = true;
    constraints_.clear();
  }

  bool enable_constraint(string const& key) {
    BOOST_FOREACH (ConstraintType& c, constraints_) {
      if (0 == c.get_name().compare(key)) {
        c.enable();
        changed_ = true;
        return true;
      }
    }
    return false;
  }
  bool disable_constraint(string const& key) {
    BOOST_FOREACH (ConstraintType& c, constraints_) {
      if (0 == c.get_name().compare(key)) {
        c.disable();
        changed_ = true;
        return true;
      }
    }
    return false;
  }
  bool is_constaint_enabled(string const& key) {
    BOOST_FOREACH (ConstraintType const& c, constraints_)
      if (0 == c.get_name().compare(key))
        return c.is_enabled();

    return false;
  }

  bool is_changed() const {
    return changed_;
  }
  void set_synced() {
    changed_ = false;
  }

  bool is_unique() const {
    return unique_;
  }
  void set_unique(bool const val) {
    changed_ = true;
    unique_ = val;
  }

  VectorElements& get_vec_vars() {
    return vec_elements_;
  }

private:
  ConstraintsVector constraints_;
  bool changed_;
  bool unique_;

  mutable VectorElements vec_elements_;
};

} // end namespace crave
