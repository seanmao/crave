#include <boost/test/unit_test.hpp>

#include <crave/ir/UserConstraint.hpp>
#include <crave/ir/UserExpression.hpp>
#include <crave/utils/Evaluator.hpp>
#include <crave/ir/visitor/ComplexityEstimationVisitor.hpp>

// using namespace std;
using namespace crave;

BOOST_FIXTURE_TEST_SUITE(UserConstraint, Context_Fixture)

BOOST_AUTO_TEST_CASE(constraint_partitioning) {
  crv_variable<unsigned> a, b, c, d, e;
  ConstraintPartitioner cp;
  ConstraintManager cm1, cm2;
  Context ctx(variable_container());
  cm1.makeConstraint(a() > b(), &ctx);
  cm1.makeConstraint(c() > d(), &ctx);
  cm1.makeConstraint(e() > 1, &ctx);
  cm2.makeConstraint(a() != e(), &ctx);

  cp.reset();
  cp.mergeConstraints(cm1);
  cp.partition();
  BOOST_REQUIRE_EQUAL(cp.getPartitions().size(), 3);

  cp.reset();
  cp.mergeConstraints(cm1);
  cp.mergeConstraints(cm2);
  cp.partition();
  BOOST_REQUIRE_EQUAL(cp.getPartitions().size(), 2);
}

BOOST_AUTO_TEST_CASE(constraint_expression_mixing) {
  crv_variable<unsigned> x, y, z, t;
  expression e1 = make_expression(x() + y());
  expression e2 = make_expression(z() + t() > 10);

  Evaluator evaluator;
  evaluator.assign(x(), 8);
  evaluator.assign(y(), 1);
  evaluator.assign(z(), 8);
  evaluator.assign(t(), 3);

  BOOST_REQUIRE(evaluator.evaluate((x() > z()) && (e1 <= 10) && e2));
  BOOST_REQUIRE(!evaluator.result<bool>());

  evaluator.assign(x(), 9);

  BOOST_REQUIRE(evaluator.evaluate((x() > z()) && (e1 + 1 <= 11) && e2));
  BOOST_REQUIRE(evaluator.result<bool>());

  BOOST_REQUIRE(evaluator.evaluate(e1 * e1));
  BOOST_REQUIRE_EQUAL(evaluator.result<unsigned>(), 100);
}

BOOST_AUTO_TEST_CASE(single_variable_constraint) {
  crv_variable<unsigned> a, b, c, d;
  ConstraintPartitioner cp;
  ConstraintManager cm;
  Context ctx(variable_container());
  cm.makeConstraint(a() > 1, &ctx);
  cm.makeConstraint(a() < 2, &ctx);
  cm.makeConstraint((1 < b()) || (b() < 10), &ctx);
  cm.makeConstraint(c() + 5 > c() + 2 * c(), &ctx);
  cm.makeConstraint(a() + b() + c() > 0, &ctx);
  cm.makeConstraint(c() < d(), &ctx);
  cp.reset();
  cp.mergeConstraints(cm);
  cp.partition();
  BOOST_REQUIRE_EQUAL(cp.getPartitions().size(), 1);
  ConstraintPartition const& part = cp.getPartitions().at(0);
  BOOST_REQUIRE_EQUAL(part.singleVariableConstraintMap().size(), 3);
  BOOST_REQUIRE_EQUAL(part.singleVariableConstraintMap().at(a().id()).size(), 2);
  BOOST_REQUIRE_EQUAL(part.singleVariableConstraintMap().at(b().id()).size(), 1);
  BOOST_REQUIRE_EQUAL(part.singleVariableConstraintMap().at(c().id()).size(), 1);
}

BOOST_AUTO_TEST_SUITE_END()  // Syntax

//  vim: ft=cpp:ts=2:sw=2:expandtab
