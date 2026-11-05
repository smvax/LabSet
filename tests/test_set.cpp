// Copyright 2026 smvax

#include <gtest/gtest.h>

#include <stdexcept>  // For exceptions
#include <iostream>  // For ostream
#include <algorithm>  // For max
#include <utility>  // For move
#include <string>  // For string
#include <vector>  // For vector

#include "../lib_set/set.h"

TEST(SetTest, can_create_with_constructor_by_max_power) {
  ASSERT_NO_THROW(Set s(100));
}

TEST(SetTest, can_create_with_normal_init_list_constructor) {
  Set s1 = {1, 2, 5};

  EXPECT_EQ(s1.get_element(1), true);
  EXPECT_EQ(s1.get_element(2), true);
  EXPECT_EQ(s1.get_element(3), false);
  EXPECT_EQ(s1.get_element(4), false);
  EXPECT_EQ(s1.get_element(5), true);
  EXPECT_EQ(s1.max_power(), 5);
}

TEST(SetTest, can_create_with_empty_init_list_constructor) {
  Set s1 = {};

  EXPECT_EQ(s1.max_power(), 0);
}

TEST(SetTest, can_create_with_duplicate_init_list_constructor) {
  Set s1 = {1, 2, 5, 5, 5};

  EXPECT_EQ(s1.get_element(1), true);
  EXPECT_EQ(s1.get_element(2), true);
  EXPECT_EQ(s1.get_element(3), false);
  EXPECT_EQ(s1.get_element(4), false);
  EXPECT_EQ(s1.get_element(5), true);
  EXPECT_EQ(s1.max_power(), 5);
}

TEST(SetTest, can_create_with_copy_constructor) {
  Set s1(50);
  s1.insert(5);
  Set s2(s1);

  EXPECT_EQ(s2.max_power(), s1.max_power());

  s1.erase(5);

  EXPECT_EQ(s2.get_element(5), true);
}

TEST(SetTest, can_create_with_move_constructor) {
  Set source(40);
  source.insert(10);
  Set destination(std::move(source));

  EXPECT_EQ(destination.get_element(10), true);
  EXPECT_EQ(source.max_power(), 0);
}

TEST(SetTest, can_get_max_power) {
  Set s(32);

  EXPECT_EQ(s.max_power(), 32);
}

TEST(SetTest, can_insert_and_erase_element) {
  Set s(50);

  ASSERT_NO_THROW(s.insert(25));

  EXPECT_EQ(s.get_element(25), true);

  ASSERT_NO_THROW(s.erase(25));

  EXPECT_EQ(s.get_element(25), false);
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

  s1.insert(16);

  EXPECT_EQ(s2.get_element(15), true);
  EXPECT_EQ(s2.get_element(16), false);
  EXPECT_EQ(s1.get_element(16), true);
}

TEST(SetTest, can_assign_set_to_itself) {
  Set s(32);
  s.insert(7);

  ASSERT_NO_THROW(s = s);
  EXPECT_EQ(s.get_element(6), false);
  EXPECT_EQ(s.get_element(7), true);
  EXPECT_EQ(s.get_element(8), false);
}

TEST(SetTest, can_do_operator_plus_assign_element) {
  Set s(32);
  s += 5;

  EXPECT_EQ(s.get_element(5), true);
}

TEST(SetTest, can_do_operator_minus_assign_element) {
  Set s(32);
  s.insert(12);
  s -= 12;

  EXPECT_EQ(s.get_element(12), false);
}

TEST(SetTest, can_do_set_union_via_plus) {
  Set s1(32);
  Set s2(32);
  s1.insert(1);
  s2.insert(2);
  Set result = s1 + s2;

  EXPECT_EQ(result.get_element(1), true);
  EXPECT_EQ(result.get_element(2), true);
  EXPECT_EQ(result.get_element(3), false);
  EXPECT_EQ(s1.get_element(1), true);
  EXPECT_EQ(s1.get_element(2), false);
  EXPECT_EQ(s2.get_element(1), false);
  EXPECT_EQ(s2.get_element(2), true);
}

TEST(SetTest, can_do_set_intersection_via_star) {
  Set s1(32);
  Set s2(32);
  s1.insert(5);
  s1.insert(10);
  s2.insert(10);
  Set result = s1 * s2;

  EXPECT_EQ(result.get_element(5), false);
  EXPECT_EQ(result.get_element(10), true);
}

TEST(SetTest, can_do_set_difference_via_minus) {
  Set s1(32);
  Set s2(32);
  s1.insert(10);
  s1.insert(20);
  s2.insert(20);
  Set result = s1 - s2;

  EXPECT_EQ(result.get_element(10), true);
  EXPECT_EQ(result.get_element(20), false);
}

TEST(SetTest, can_do_unary_minus_as_negation) {
  Set s(32);
  s.insert(1);
  Set result = -s;

  EXPECT_EQ(result.get_element(1), false);
  EXPECT_EQ(result.get_element(2), true);
  EXPECT_EQ(result.get_element(32), true);
}

TEST(SetTest, throws_when_operates_with_different_universes) {
  Set s1(10);
  Set s2(50);
  s1.insert(5);
  s2.insert(5);
  s2.insert(45);

  ASSERT_THROW(s1 + s2, std::logic_error);
  ASSERT_THROW(s1 - s2, std::logic_error);
  ASSERT_THROW(s1 * s2, std::logic_error);
  ASSERT_THROW(s1 += s2, std::logic_error);
  ASSERT_THROW(s1 -= s2, std::logic_error);
  ASSERT_THROW(s1 *= s2, std::logic_error);
}

TEST(SetTest, can_do_unary_minus) {
  Set s(31);
  s.insert(1);
  s.insert(24);
  Set result = -s;
  std::stringstream ss;
  ss << result;

  EXPECT_EQ(ss.str(),
            "{ 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, "
            "17, 18, 19, 20, 21, 22, 23, 25, 26, 27, 28, 29, 30, 31 }");
}

TEST(SetTest, can_cout_normal_set) {
  Set s({2, 5, 7});
  std::stringstream ss;
  ss << s;

  EXPECT_EQ(ss.str(), "{ 2, 5, 7 }");
}

TEST(SetTest, can_cout_empty_set) {
  Set s(10);
  std::stringstream ss;
  ss << s;

  EXPECT_EQ(ss.str(), "{  }");
}

TEST(SetTest, can_cin_normal_set) {
  std::string input_str = "{ 2, 3, 4, 6 }";
  std::stringstream ss(input_str);
  Set s(0);
  ss >> s;

  EXPECT_EQ(s.max_power(), 6);
  EXPECT_EQ(s.get_element(1), false);
  EXPECT_EQ(s.get_element(2), true);
  EXPECT_EQ(s.get_element(3), true);
  EXPECT_EQ(s.get_element(4), true);
  EXPECT_EQ(s.get_element(5), false);
  EXPECT_EQ(s.get_element(6), true);
}

TEST(SetTest, can_cin_empty_set) {
  std::string input_str = "{}";
  std::stringstream ss(input_str);
  Set s({1, 5, 10});
  ss >> s;

  EXPECT_TRUE(static_cast<bool>(ss));
  EXPECT_GE(s.max_power(), 0);
}
