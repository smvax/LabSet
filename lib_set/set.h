// Copyright 2024 Marina Usova

#ifndef LIB_SET_SET_H_
#define LIB_SET_SET_H_

#include "../lib_bitfield/bitfield.h"

class Set {
  BitField _bitfield;

 public:
  explicit Set(size_t);
  Set(const Set&);  // NOT noexcept 
  // Because internal class uses allocation
  Set(Set&&) noexcept;
  Set(const BitField&);
  Set(BitField&&) noexcept;

  inline size_t max_power() const noexcept;
  
  void insert(int);
  void erase(int);

  explicit operator const BitField&() const noexcept;
  
  Set& operator+=(int);
  Set& operator-=(int);
  Set operator+(const Set&) const;
  Set operator-(const Set&) const;
  Set operator*(const Set&) const;
  Set& operator+=(const Set&);
  Set& operator-=(const Set&);
  Set& operator*=(const Set&);
  Set operator-();  // Unary
  Set& operator=(const Set&);
  Set& operator=(Set&&) noexcept;

};

#endif  // LIB_SET_SET_H_
