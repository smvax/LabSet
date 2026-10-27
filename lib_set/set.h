// Copyright 2024 Marina Usova

#ifndef LIB_SET_SET_H_
#define LIB_SET_SET_H_

#include "../lib_bitfield/bitfield.h"

class Set {
  BitField _bitfield;

 public:
  explicit Set(size_t);
  Set(std::initializer_list<int>);
  Set(const Set&);  // NOT noexcept
  // Because internal class uses allocation
  Set(Set&&) noexcept;
  explicit Set(const BitField&);
  explicit Set(BitField&&) noexcept;

  inline size_t max_power() const noexcept;

  inline void insert(int);
  inline void erase(int);

  explicit operator const BitField&() const noexcept;

  Set& operator+=(int);
  Set& operator-=(int);
  Set operator+(const Set&) const;
  Set operator-(const Set&) const;
  Set operator*(const Set&) const;
  Set& operator+=(const Set&);
  Set& operator-=(const Set&);
  Set& operator*=(const Set&);
  Set operator-() const;  // Unary
  Set& operator=(const Set&);
  Set& operator=(Set&&) noexcept;

  friend std::ostream& operator<<(std::ostream&, const Set&);
};

inline size_t Set::max_power() const noexcept { return _bitfield.max_power(); }

inline void Set::insert(int elem) {
  if (elem < 1) {
    throw std::invalid_argument("ERROR: Element must be a natural number!");
  }
  // If element > max_power, BitField throws
  _bitfield.set_bit(elem - 1);
}
inline void Set::erase(int elem) {
  if (elem < 1) {
    throw std::invalid_argument("ERROR: Element must be a natural number!");
  }
  // If element > max_power, BitField throws
  _bitfield.reset_bit(elem - 1);
}

std::ostream& operator<<(std::ostream&, const Set&);

#endif  // LIB_SET_SET_H_
