// Copyright 2024 Marina Usova

#include <gtest/gtest.h>

#include <stdexcept>  // For exceptions
#include <iostream>  // For ostream
#include <algorithm>  // For max
#include <utility>  // For move
#include <string>  // For string

#include "../lib_set/set.h"

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

TEST(SetTest, can_do_operations_on_different_sizes) {
  Set s1(10);
  Set s2(50);
  s1.insert(5);
  s2.insert(5);
  s2.insert(45);
  Set res_plus = s1 + s2;
  const BitField& bf_plus = static_cast<const BitField&>(res_plus);

  EXPECT_EQ(bf_plus.get_bit(4), true);
  EXPECT_EQ(bf_plus.get_bit(44), true);

  Set res_mult = s1 * s2;
  const BitField& bf_mult = static_cast<const BitField&>(res_mult);

  EXPECT_EQ(bf_mult.get_bit(4), true);
  EXPECT_EQ(bf_mult.get_bit(44), false);

  Set res_minus = s2 - s1;
  const BitField& bf_minus = static_cast<const BitField&>(res_minus);

  EXPECT_EQ(bf_minus.get_bit(4), false);
  EXPECT_EQ(bf_minus.get_bit(44), true);
}

TEST(SetTest, can_do_unary_minus) {
  Set s(5);
  s.insert(1);
  s.insert(24);
  Set result = -s;
  std::stringstream ss;
  ss << result;

  EXPECT_EQ(ss.str(),
            "{2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, "
            "20, 21, 22, 23, 25, 26, 27, 28, 29, 30, 31, 32}");
}

TEST(SetTest, can_cout_normal_set) {
  Set s({2, 5, 7});
  std::stringstream ss;
  ss << s;

  EXPECT_EQ(ss.str(), "{2, 5, 7}");
}

TEST(SetTest, can_cout_empty_set) {
  Set s(10);
  std::stringstream ss;
  ss << s;

  EXPECT_EQ(ss.str(), "{}");
}

TEST(SetTest, can_cin_normal_set) {
  std::string input_str = "{ 2, 3, 4, 6 }";
  std::stringstream ss(input_str);
  Set s(0);
  ss >> s;

  EXPECT_GE(s.max_power(), 6);

  const BitField& bf = static_cast<const BitField&>(s);

  EXPECT_TRUE(bf.get_bit(1));
  EXPECT_TRUE(bf.get_bit(2));
  EXPECT_TRUE(bf.get_bit(3));
  EXPECT_TRUE(bf.get_bit(5));
  EXPECT_FALSE(bf.get_bit(0));
  EXPECT_FALSE(bf.get_bit(4));
}

TEST(SetTest, can_cin_empty_set) {
  std::string input_str = "{}";
  std::stringstream ss(input_str);
  Set s({1, 5, 10});
  ss >> s;

  EXPECT_TRUE(static_cast<bool>(ss));

  const BitField& bf = static_cast<const BitField&>(s);

  for (size_t i = 0; i < bf.max_power(); ++i) {
    EXPECT_FALSE(bf.get_bit(i));
  }
}
