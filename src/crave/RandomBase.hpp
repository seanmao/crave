#pragma once

#include "Constraint.hpp"
#include "Distribution.hpp"
#include "VectorConstraint.hpp"

#include <boost/preprocessor/seq/for_each.hpp>

#include <vector>
#include <set>

namespace crave {

  class rand_base {
  protected:
    rand_base() { }
    ~rand_base() { }

  public:
    virtual bool next() = 0;
    virtual void gatherValues(std::vector<long>&) = 0;
  };

  class rand_obj_base : public rand_base {
  public:
    virtual void addChild(rand_base*, bool bindNext) = 0;

  protected:
    rand_obj_base* parent;
    std::vector<rand_base*> children;
  };

  template<typename T>
  class randv_prim_base : public rand_base {
  public:
    operator T() const { return value; }
    friend ostream& operator<<(ostream& os, const randv_prim_base<T>& e) { os << e.value; return os; }
    WriteReference<T> const& operator()() const { return var; }

    virtual void gatherValues(std::vector<long>& ch) { ch.push_back(static_cast<long>(value)); }

  protected:
    randv_prim_base(rand_obj_base* parent) : var(value) { if (parent != 0) parent->addChild(this, true); }
    randv_prim_base(const randv_prim_base& other) : var(value), value(other.value) { }
    WriteReference<T> var;
    T value;
  };

  template<typename T>
  class randv : public randv_prim_base<T> {
  public:
    randv(rand_obj_base* parent) : randv_prim_base<T>(parent) { }
    randv(const randv& other) : randv_prim_base<T>(other) { }
    bool next() { return true; }
  };

#define _COMMON_INTERFACE(Typename) \
public: \
  randv(rand_obj_base* parent = 0) : randv_prim_base<Typename>(parent) { } \
  randv(const randv& other) : randv_prim_base<Typename>(other) { } \
  bool next() { static distribution<Typename> dist; value = dist.nextValue(); return true; } \
  randv<Typename>& operator=(const randv<Typename>& i) { value = i.value; return *this; } \
  randv<Typename>& operator=(Typename i) { value = i; return *this; } \

#define _INTEGER_INTERFACE(Typename) \
public: \
  randv<Typename>& operator++() { ++value;  return *this; } \
  Typename operator++(int) { Typename tmp = value; ++value; return tmp; } \
  randv<Typename>& operator--() { --value;  return *this; } \
  Typename operator--(int) { Typename tmp = value; --value; return tmp; } \
  randv<Typename>& operator+=(Typename i) { value += i;  return *this; } \
  randv<Typename>& operator-=(Typename i) { value -= i;  return *this; } \
  randv<Typename>& operator*=(Typename i) { value *= i;  return *this; } \
  randv<Typename>& operator/=(Typename i) { value /= i;  return *this; } \
  randv<Typename>& operator%=(Typename i) { value %= i;  return *this; } \
  randv<Typename>& operator&=(Typename i) { value &= i;  return *this; } \
  randv<Typename>& operator|=(Typename i) { value |= i;  return *this; } \
  randv<Typename>& operator^=(Typename i) { value ^= i;  return *this; } \
  randv<Typename>& operator<<=(Typename i) { value <<= i;  return *this; } \
  randv<Typename>& operator>>=(Typename i) { value >>= i;  return *this; } \

// bool
  template<>
  class randv<bool> : public randv_prim_base<bool> {
    _COMMON_INTERFACE(bool)
  };

// for all C/C++ built-in integer types
#define _INTEGER_TYPE(typename) \
template<> \
class randv<typename> : public randv_prim_base<typename> { \
  _COMMON_INTERFACE(typename) \
  _INTEGER_INTERFACE(typename) \
}; \

  _INTEGER_TYPE(int)
  _INTEGER_TYPE(unsigned int)
  _INTEGER_TYPE(char)
  _INTEGER_TYPE(signed char)
  _INTEGER_TYPE(unsigned char)
  _INTEGER_TYPE(short)
  _INTEGER_TYPE(unsigned short)
  _INTEGER_TYPE(long)
  _INTEGER_TYPE(unsigned long)
  _INTEGER_TYPE(long long)
  _INTEGER_TYPE(unsigned long long)

#undef _COMMON_INTERFACE
#undef _INTEGER_INTERFACE
#undef _INTEGER_TYPE

  template<typename T>
  class rand_vec : public __rand_vec<T>, public rand_base {
  public:
    rand_vec(rand_obj_base* parent) : __rand_vec<T>() { if (parent != 0) parent->addChild(this, true); }

    virtual bool next() {
      static randv<T> r(NULL);
      this->clear();
      for (uint i = 0; i < default_rand_vec_size(); i++) {
        r.next();
        this->push_back(r);
      }
      return true;
    }

    virtual void gatherValues(std::vector<long>& ch) { }
  };

} // namespace crave
