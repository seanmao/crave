#include <boost/test/unit_test.hpp>

#include <boost/format.hpp>

#include <set>
#include <iostream>

using boost::format;
using namespace crave;

template <typename T>
bool check_unique(rand_vec<T>& v) {
  for (unsigned i = 0; i < v.size(); i++)
    for (unsigned j = 0; j < i; j++)
      if (v[i] == v[j]) return false;
  return true;
}

BOOST_FIXTURE_TEST_SUITE(Vector_Constraint_t, Context_Fixture)

struct Item : public rand_obj {
  Item() : i_(), v(this) {
    constraint(30 <= v().size() && v().size() <= 50);
    constraint(foreach (v(), v()[i_] == v()[i_ - 1] + v()[i_ - 2]));
    constraint(foreach (v(), if_then(i_ <= 1, v()[i_] == i_)));
  }

  placeholder i_;
  rand_vec<unsigned int> v;
};

BOOST_AUTO_TEST_CASE(fibo_test) {
  Item it;
  BOOST_REQUIRE(it.next());
  BOOST_REQUIRE_LE(30, it.v.size());
  BOOST_REQUIRE_LE(it.v.size(), 50);
  BOOST_REQUIRE_EQUAL(it.v[0], 0);
  BOOST_REQUIRE_EQUAL(it.v[1], 1);
  for (unsigned i = 2; i < it.v.size(); i++) BOOST_REQUIRE_EQUAL(it.v[i], it.v[i - 1] + it.v[i - 2]);
}

struct Item1 : public rand_obj {
  Item1() : v(this) {}

  __rand_vec<unsigned int> u;
  rand_vec<unsigned int> v;
};

BOOST_AUTO_TEST_CASE(free_vector_test) {
  Item1 it;
  BOOST_REQUIRE(it.next());
  BOOST_REQUIRE(it.u.size() == 0);
  BOOST_REQUIRE(it.v.size() > 0);
  for (unsigned i = 0; i < it.v.size(); i++) std::cout << it.v[i] << " ";
  std::cout << std::endl;
}

struct Item2 : public rand_obj {
  Item2() : idx(), v(this) { constraint(foreach (v(), 100 <= v()[idx] && v()[idx] <= 200)); }

  placeholder idx;
  rand_vec<unsigned int> v;
};

BOOST_AUTO_TEST_CASE(default_size_test) {
  Item2 it;
  BOOST_REQUIRE(it.next());
  BOOST_REQUIRE(it.v.size() == default_rand_vec_size());
  for (unsigned i = 0; i < it.v.size(); i++) {
    std::cout << it.v[i] << " ";
    BOOST_REQUIRE(100 <= it.v[i] && it.v[i] <= 200);
  }
  std::cout << std::endl;
}

struct Item3 : public rand_obj {
  Item3() : i(), v(this) {
    constraint(v().size() == 100);
    constraint(foreach (v(), v()[i] < 100));
    constraint(foreach (v(), v()[i] >= 0));
    constraint(unique(v()));
  }

  placeholder i;
  rand_vec<int> v;
};

BOOST_AUTO_TEST_CASE(unique_test_1) {
  Item3 it;
  BOOST_REQUIRE(it.next());
  BOOST_REQUIRE(it.v.size() == 100);
  for (unsigned i = 0; i < it.v.size(); i++) {
    BOOST_REQUIRE(0 <= it.v[i] && it.v[i] < 100);
    for (unsigned j = 0; j < i; j++) BOOST_REQUIRE(it.v[i] != it.v[j]);
  }
}

BOOST_AUTO_TEST_CASE(unique_test_2) {
  rand_vec<unsigned int> v(NULL);
  placeholder idx;
  Generator gen;
  gen(v().size() == 7);
  gen(foreach (v(), v()[idx] < 6));

  gen("unique", unique(v()));
  BOOST_REQUIRE(!gen.next());

  gen.disableConstraint("unique");
  BOOST_REQUIRE(gen.next());
  BOOST_REQUIRE(v.size() == 7);
  for (unsigned i = 0; i < v.size(); i++) {
    std::cout << v[i] << " ";
    BOOST_REQUIRE(v[i] < 6);
  }
  std::cout << std::endl;
}

BOOST_AUTO_TEST_CASE(soft_unique_test) {
  rand_vec<unsigned int> v(NULL);
  placeholder idx;
  Generator gen;
  gen(v().size() == 7);
  gen(foreach (v(), v()[idx] < 6));
  gen.soft(unique(v()));
  BOOST_REQUIRE(gen.next());
}

struct Item4 : public rand_obj {
  Item4() : _i(), v(this) {
    constraint(v().size() == 10);
    constraint("c1", foreach (v(), v()[_i] <= 100));
    constraint("c2", foreach (v(), v()[_i] > 100));
  }

  placeholder _i;
  rand_vec<unsigned int> v;
};

BOOST_AUTO_TEST_CASE(constraint_management_test) {
  Item4 it;

  BOOST_REQUIRE(!it.next());

  it.disable_constraint("c1");
  BOOST_REQUIRE(it.next());
  for (unsigned i = 0; i < it.v.size(); i++) BOOST_REQUIRE_GT(it.v[i], 100);

  it.enable_constraint("c1");
  it.disable_constraint("c2");
  BOOST_REQUIRE(it.next());
  for (unsigned i = 0; i < it.v.size(); i++) BOOST_REQUIRE(it.v[i] <= 100);

  it.enable_constraint("c2");
  BOOST_REQUIRE(!it.next());
}

struct Item5 : public rand_obj {
  Item5() : x(), v(this) {
    constraint(v().size() == 50);
    constraint(foreach (v(), if_then(x < 25, v()[x] == x)));
    constraint(foreach (v(), if_then(x == 25, v()[x] == 0)));
    constraint(foreach (v(), if_then(x > 25, v()[x] + x == 200)));
  }

  placeholder x;
  rand_vec<unsigned int> v;
};

BOOST_AUTO_TEST_CASE(index_constraint_test) {
  Item5 it;
  it.constraint.printDotGraph(std::cout);
  BOOST_REQUIRE(it.next());
  BOOST_REQUIRE(it.v.size() == 50);
  for (unsigned i = 0; i < it.v.size(); i++) {
    if (i < 25)
      BOOST_REQUIRE(it.v[i] == i);
    else if (i > 25)
      BOOST_REQUIRE(it.v[i] + i == 200);
    else
      BOOST_REQUIRE(it.v[i] == 0);
  }
}

BOOST_AUTO_TEST_CASE(soft_vec_constraint) {
  rand_vec<unsigned int> v(NULL);
  placeholder i;

  Generator gen;
  gen(v().size() == 10);
  gen(foreach (v(), v()[i] >= v()[i - 1]));
  gen.soft(foreach (v(), v()[i] < v()[i - 1]));
  BOOST_REQUIRE(gen.next());

  Generator gen1;
  gen1(v().size() == 4);
  gen1(foreach (v(), v()[i] >= v()[i - 1]));
  gen1(foreach (v(), v()[i] <= 1000));
  gen1.soft(foreach (v(), v()[i] <= v()[i - 1]));
  gen1.soft(foreach (v(), if_then(i == 0, v()[i] % 13 == 3)));
  BOOST_REQUIRE(gen1.next());
  for (int j = 0; j < 10; j++) {
    BOOST_REQUIRE_EQUAL(v.size(), 4);
    std::cout << v[0] << " " << v[1] << " " << v[2] << " " << v[3] << std::endl;
    BOOST_REQUIRE_EQUAL(v[0], v[1]);
    BOOST_REQUIRE_EQUAL(v[1], v[2]);
    BOOST_REQUIRE_EQUAL(v[2], v[3]);
    BOOST_REQUIRE_EQUAL(v[0] % 13, 3);
    if (!gen1.next()) break;
  }
}

BOOST_AUTO_TEST_CASE(mixed_bv_width_1) {
  rand_vec<signed char> a(NULL);
  placeholder idx;
  Generator gen;
  gen(a().size() == 28);
  gen(foreach (a(), a()[idx] < (short)-100));
  gen(unique(a()));

  BOOST_REQUIRE(gen.next());
  for (unsigned i = 0; i < a.size(); i++) {
    BOOST_REQUIRE_LT(a[i], -100);
    std::cout << " " << (int)a[i];
  }
  std::cout << std::endl;
  BOOST_REQUIRE(check_unique(a));
}

BOOST_AUTO_TEST_CASE(mixed_bv_width_2) {
  rand_vec<short> a(NULL);
  placeholder p;
  Generator gen;
  gen(a().size() == 19);
  gen(foreach (a(), a()[p] < 10));
  gen(foreach (a(), a()[p] > -10));
  gen(unique(a()));

  BOOST_REQUIRE(gen.next());
  for (unsigned i = 0; i < a.size(); i++) BOOST_REQUIRE(-10 < a[i] && a[i] < 10);
  BOOST_REQUIRE(check_unique(a));
}

BOOST_AUTO_TEST_CASE(mixed_bv_width_3) {
  rand_vec<int> a(NULL);
  placeholder _i;
  Generator gen;
  gen(a().size() == 19);
  gen(foreach (a(), a()[_i] < (signed char)10));
  gen(foreach (a(), a()[_i] > (short)-10));
  gen(unique(a()));

  BOOST_REQUIRE(gen.next());
  for (unsigned i = 0; i < a.size(); i++) BOOST_REQUIRE(-10 < a[i] && a[i] < 10);
  BOOST_REQUIRE(check_unique(a));
}

BOOST_AUTO_TEST_CASE(mixed_bv_width_4) {
  rand_vec<short> a(NULL);
  placeholder _i;
  Generator gen;
  gen(a().size() == 10);
  gen(foreach (a(), -3 <= a()[_i] && a()[_i] <= 3));
  gen(foreach (a(), a()[_i] != 0));
  gen(foreach (a(), a()[_i] * a()[_i - 1] % 6 == 0));
  gen(foreach (a(), _i != 0 || a()[_i] % 2 == 0));

  BOOST_REQUIRE(gen.next());
  for (unsigned i = 0; i < a.size(); i++) {
    BOOST_REQUIRE(-3 <= a[i] && a[i] <= 3);
    BOOST_REQUIRE(a[i] != 0);
    if (i == 0)
      BOOST_REQUIRE(a[i] % 2 == 0);
    else
      BOOST_REQUIRE(a[i] * a[i - 1] % 6 == 0);
  }
}

BOOST_AUTO_TEST_CASE(bool_rand_vec) {
  rand_vec<bool> a(NULL);
  placeholder _i;
  Generator gen;
  gen(a().size() == 10);
  gen(foreach (a(), a()[_i] != a()[_i - 1]));

  BOOST_REQUIRE(gen.next());
  BOOST_REQUIRE(a.size() == 10);
  for (unsigned i = 1; i < a.size(); i++) BOOST_REQUIRE(a[i] != a[i - 1]);
}

struct Item6 : public rand_obj {
  Item6() : v(this) { constraint(v().size() == default_rand_vec_size() + 7); }
  rand_vec<unsigned int> v;
};

BOOST_AUTO_TEST_CASE(only_size_constrained_vect_test) {
  Item6 it;
  BOOST_REQUIRE(it.next());
  BOOST_REQUIRE_EQUAL(it.v.size(), default_rand_vec_size() + 7);
}

BOOST_AUTO_TEST_SUITE_END()  // Context

//  vim: ft=cpp:ts=2:sw=2:expandtab
