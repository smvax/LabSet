// Copyright 2024 Marina Usova

#include "../lib_set/set.h"

Set::Set(size_t max_power) : _bitfield((max_power + 31) / 32) {}
Set::Set(std::initializer_list<int> list) :
    _bitfield(list.size()==0 ? 0 : (std::max(list) + 31) / 32) {
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
