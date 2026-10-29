// Copyright 2024 Marina Usova

#include <stdexcept>  // For exceptions
#include <iostream>  // For ostream
#include <algorithm>  // For max
#include <utility>  // For move

#include "../lib_set/set.h"

Set::Set(size_t max_power) : _bitfield((max_power + 31) / 32) {}
Set::Set(std::initializer_list<int> list) :
    _bitfield(list.size() == 0 ? 0 :
        (*std::max_element(list.begin(), list.end()) + 31) / 32) {
  for (int elem : list) {
    insert(elem);
  }
}
Set::Set(const Set& other) : _bitfield(other._bitfield) {}
Set::Set(Set&& other) noexcept : _bitfield(std::move(other._bitfield)) {}
Set::Set(const BitField& bitfield) : _bitfield(bitfield) {}
Set::Set(BitField&& bitfield) noexcept : _bitfield(std::move(bitfield)) {}

Set::operator const BitField&() const noexcept { return _bitfield; }

Set& Set::operator+=(int elem) {
  this->insert(elem);
  return *this;
}
Set& Set::operator-=(int elem) {
  this->erase(elem);
  return *this;
}
Set Set::operator+(const Set& other) const {
  Set result(*this);
  result += other;
  return result;
}
Set Set::operator-(const Set& other) const {
  Set result(*this);
  result -= other;
  return result;
}
Set Set::operator*(const Set& other) const {
  Set result(*this);
  result *= other;
  return result;
}
Set& Set::operator+=(const Set& other) {
  _bitfield = _bitfield | other._bitfield;
  return *this;
}
Set& Set::operator-=(const Set& other) {
  BitField other_bf = static_cast<const BitField&>(other);
  if (other_bf.max_power() >= _bitfield.max_power()) {
    _bitfield = _bitfield & (~other_bf);
  } else {
    BitField aligned_other(_bitfield.max_power());
    for (size_t i = 0; i < other_bf.max_power(); ++i) {
      if (other_bf.get_bit(i)) {
        aligned_other.set_bit(i);
      }
    }
    _bitfield = _bitfield & (~aligned_other);
  }
  return *this;
}

Set& Set::operator*=(const Set& other) {
  _bitfield = _bitfield & other._bitfield;
  return *this;
}
Set Set::operator-() const {
  Set result(*this);
  result._bitfield = ~_bitfield;
  return result;
}
Set& Set::operator=(const Set& other) {
  if (this != &other) {
    _bitfield = other._bitfield;
  }
  return *this;
}
Set& Set::operator=(Set&& other) noexcept {
  if (this != &other) {
    _bitfield = std::move(other._bitfield);
  }
  return *this;
}

std::ostream& operator<<(std::ostream& out, const Set& set) {
  size_t size = set.max_power();
  out << "{";
  bool first = true;
  for (size_t i = 0; i < size; ++i) {
    if (set._bitfield.get_bit(i)) {
      if (!first) {
        out << ", ";
      }
      out << i + 1;
      first = false;
    }
  }
  out << "}";
  return out;
}
std::istream& operator>>(std::istream& in, Set& set) {
  char ch;
  if (!(in >> ch) || ch != '{') {
    in.setstate(std::ios::failbit);
    return in;
  }
  int max_elem = 0;
  int elements[1000];
  size_t count = 0;
  while (in >> ch) {
    if (ch == '}') {
      break;
    }
    if (ch == ',') {
      continue;
    }
    in.putback(ch);
    int elem;
    if (in >> elem) {
      if (elem > max_elem) {
        max_elem = elem;
      }
      if (count < 1000) {
        elements[count++] = elem;
      }
    } else {
      break;
    }
  }
  Set temp(max_elem);
  for (size_t i = 0; i < count; ++i) {
    temp.insert(elements[i]);
  }
  set = std::move(temp);
  return in;
}
