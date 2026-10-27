// Copyright 2024 Marina Usova

#include "../lib_set/set.h"
#include <gtest/gtest.h>

#include <stdexcept>  // For exceptions
#include <iostream>  // For ostream
#include <algorithm>  // For max
#include <utility>  // For move

TEST(SetTest, can_create_with_constructor_by_max_power) {
  ASSERT_NO_THROW(Set s(100));
}

TEST(SetTest, can_create_with_normal_init_list_constructor) {
  Set s1 = {1, 2, 3};
  const BitField& bf1 = static_cast<const BitField&>(s1);

  EXPECT_EQ(bf1.get_bit(0), true);
  EXPECT_EQ(bf1.get_bit(1), true);
  EXPECT_EQ(bf1.get_bit(2), true);
  EXPECT_EQ(bf1.get_bit(3), false);
  EXPECT_EQ(bf1.max_power(), 32);

  Set s2 = {1, 2, 60};
  const BitField& bf2 = static_cast<const BitField&>(s2);

  EXPECT_EQ(bf2.get_bit(0), true);
  EXPECT_EQ(bf2.get_bit(1), true);
  EXPECT_EQ(bf2.get_bit(2), false);
  EXPECT_EQ(bf2.get_bit(3), false);
  EXPECT_EQ(bf2.get_bit(59), true);
  EXPECT_EQ(bf2.max_power(), 64);
}

TEST(SetTest, can_create_with_empty_init_list_constructor) {
  Set s1 = {};
  const BitField& bf1 = static_cast<const BitField&>(s1);

  EXPECT_EQ(bf1.max_power(), 0);
}

TEST(SetTest, can_create_with_duplicate_init_list_constructor) {
  Set s1 = {1, 2, 3, 3, 3};
  const BitField& bf1 = static_cast<const BitField&>(s1);

  EXPECT_EQ(bf1.get_bit(0), true);
  EXPECT_EQ(bf1.get_bit(1), true);
  EXPECT_EQ(bf1.get_bit(2), true);
  EXPECT_EQ(bf1.get_bit(3), false);
  EXPECT_EQ(bf1.max_power(), 32);

  Set s2 = {1, 1, 2, 60};
  const BitField& bf2 = static_cast<const BitField&>(s2);

  EXPECT_EQ(bf2.get_bit(0), true);
  EXPECT_EQ(bf2.get_bit(1), true);
  EXPECT_EQ(bf2.get_bit(2), false);
  EXPECT_EQ(bf2.get_bit(3), false);
  EXPECT_EQ(bf2.get_bit(59), true);
  EXPECT_EQ(bf2.max_power(), 64);
}

TEST(SetTest, can_create_with_copy_constructor) {
  Set s1(50);
  s1.insert(5);
  Set s2(s1);

  EXPECT_EQ(s2.max_power(), s1.max_power());

  s1.erase(5);

  const BitField& bf2 = static_cast<const BitField&>(s2);

  EXPECT_EQ(bf2.get_bit(4), true);
}

TEST(SetTest, can_create_with_move_constructor) {
  Set source(40);
  source.insert(10);
  Set destination(std::move(source));
  const BitField& bf_dest = static_cast<const BitField&>(destination);

  EXPECT_EQ(bf_dest.get_bit(9), true);  // 10 - 1 = 9
}

TEST(SetTest, can_get_max_power) {
  Set s(32);

  EXPECT_EQ(s.max_power(), 32);
}

TEST(SetTest, can_insert_and_erase_element) {
  Set s(50);

  ASSERT_NO_THROW(s.insert(25));

  const BitField& bf1 = static_cast<const BitField&>(s);

  EXPECT_EQ(bf1.get_bit(24), true);

  ASSERT_NO_THROW(s.erase(25));

  const BitField& bf2 = static_cast<const BitField&>(s);

  EXPECT_EQ(bf2.get_bit(24), false);
}

TEST(SetTest, throws_when_insert_or_erase_non_natural_number) {
  Set s(50);

  EXPECT_THROW(s.insert(0), std::invalid_argument);
  EXPECT_THROW(s.insert(-5), std::invalid_argument);
  EXPECT_THROW(s.erase(0), std::invalid_argument);
  EXPECT_THROW(s.erase(-10), std::invalid_argument);
}

TEST(SetTest, can_assign_sets) {
  Set s1(32);
  s1.insert(15);
  Set s2(32);

  ASSERT_NO_THROW(s2 = s1);

  const BitField& bf2 = static_cast<const BitField&>(s2);

  EXPECT_EQ(bf2.get_bit(14), true);
}

TEST(SetTest, can_assign_set_to_itself) {
  Set s(32);
  s.insert(7);

  ASSERT_NO_THROW(s = s);

  const BitField& bf = static_cast<const BitField&>(s);

  EXPECT_EQ(bf.get_bit(6), true);
}

TEST(SetTest, can_do_operator_plus_assign_element) {
  Set s(32);
  s += 5;
  const BitField& bf = static_cast<const BitField&>(s);

  EXPECT_EQ(bf.get_bit(4), true);
}

TEST(SetTest, can_do_operator_minus_assign_element) {
  Set s(32);
  s.insert(12);
  s -= 12;
  const BitField& bf = static_cast<const BitField&>(s);

  EXPECT_EQ(bf.get_bit(11), false);
}

TEST(SetTest, can_do_set_union_via_plus) {
  Set s1(32);
  Set s2(32);
  s1.insert(1);
  s2.insert(2);
  Set result = s1 + s2;
  const BitField& bf_res = static_cast<const BitField&>(result);

  EXPECT_EQ(bf_res.get_bit(0), true);
  EXPECT_EQ(bf_res.get_bit(1), true);
}

TEST(SetTest, can_do_set_intersection_via_star) {
  Set s1(32);
  Set s2(32);
  s1.insert(5);
  s1.insert(10);
  s2.insert(10);
  Set result = s1 * s2;
  const BitField& bf_res = static_cast<const BitField&>(result);

  EXPECT_EQ(bf_res.get_bit(4), false);
  EXPECT_EQ(bf_res.get_bit(9), true);
}

TEST(SetTest, can_do_set_difference_via_minus) {
  Set s1(32);
  Set s2(32);
  s1.insert(10);
  s1.insert(20);
  s2.insert(20);
  Set result = s1 - s2;
  const BitField& bf_res = static_cast<const BitField&>(result);

  EXPECT_EQ(bf_res.get_bit(9), true);
  EXPECT_EQ(bf_res.get_bit(19), false);
}

TEST(SetTest, can_do_unary_minus_as_negation) {
  Set s(32);
  s.insert(1);  // Bit 0 = 1, rest = 0
  Set result = -s;
  const BitField& bf_res = static_cast<const BitField&>(result);

  EXPECT_EQ(bf_res.get_bit(0), false);
  EXPECT_EQ(bf_res.get_bit(1), true);
  EXPECT_EQ(bf_res.get_bit(31), true);
}

TEST(SetTest, throws_when_operations_on_different_sizes) {
  Set s1(32);
  Set s2(64);

  EXPECT_THROW(s1 + s2, std::logic_error);
  EXPECT_THROW(s1 * s2, std::logic_error);
  EXPECT_THROW(s1 - s2, std::logic_error);
}

TEST(SetTest, can_output_normal_set_via_ostream_operator) {
  Set s({2, 5, 7});
  std::stringstream ss;
  ss << s;

  EXPECT_EQ(ss.str(), "{2, 5, 7}");
}

TEST(SetTest, can_output_empty_set_via_ostream_operator) {
  Set s(10);
  std::stringstream ss;
  ss << s;

  EXPECT_EQ(ss.str(), "{}");
}