// Copyright 2026 smvax

#include <stdexcept>  // For exceptions
#include <iostream>  // For ostream
#include <algorithm>  // For max
#include <utility>  // For move
#include <string>  // For string
#include <vector>  // For vector

#include "../lib_set/set.h"

Set::Set(size_t max_power) : _bitfield(max_power) {}
Set::Set(std::initializer_list<int> list) : _bitfield(list.size() == 0 ?
    0 : *std::max_element(list.begin(), list.end())) {
  for (int elem : list) {
    insert(elem);
  }
}
Set::Set(const Set& other) : _bitfield(other._bitfield) {}
Set::Set(Set&& other) noexcept : _bitfield(std::move(other._bitfield)) {}
Set::Set(const BitField& bitfield) : _bitfield(bitfield) {}
Set::Set(BitField&& bitfield) noexcept : _bitfield(std::move(bitfield)) {}

Set::operator const BitField&() const noexcept {
    return _bitfield;
}

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
  _bitfield = _bitfield & (~other._bitfield);
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
  size_t max_power = set.max_power();
  out << "{ ";
  bool first = true;
  for (size_t i = 1; i <= max_power; ++i) {
    if (set.get_element(i)) {
      if (!first) {
        out << ", ";
      }
      out << i;
      first = false;
    }
  }
  out << " }";
  return out;
}
std::istream& operator>>(std::istream& in, Set& set) {
  char ch;
  if (!(in >> ch) || ch != '{') {
    in.setstate(std::ios::failbit);
    return in;
  }
  std::vector<int> parsed_elements;
  int max_elem = 0;
  bool expect_element = false;
  in >> std::ws;
  if (in.peek() == '}') {
    in >> ch;
    Set temp(0);
    set = std::move(temp);
    return in;
  }
  while (true) {
    int elem;
    if (in >> elem) {
      parsed_elements.push_back(elem);
      if (elem > max_elem) {
        max_elem = elem;
      }
      expect_element = false;
    } else {
      in.setstate(std::ios::failbit);
      return in;
    }
    if (!(in >> ch)) {
      in.setstate(std::ios::failbit);
      return in;
    }
    if (ch == '}') {
      break;
    } else if (ch == ',') {
      expect_element = true;
      continue;
    } else {
      in.setstate(std::ios::failbit);
      return in;
    }
  }
  if (expect_element) {
    in.setstate(std::ios::failbit);
    return in;
  }
  Set temp(max_elem);
  for (int elem : parsed_elements) {
    temp.insert(elem);
  }
  set = std::move(temp);
  return in;
}