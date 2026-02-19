#include "Card.hpp"
#include "unit_test_framework.hpp"
#include <iostream>
using namespace std;

TEST(test_card_ctor) {
    Card c(ACE, SPADES);
    ASSERT_EQUAL(ACE, c.get_rank());
    ASSERT_EQUAL(SPADES, c.get_suit());
}

TEST(print_card) {
    Card c(ACE, SPADES);
    ostringstream oss;
    oss << c;
    ASSERT_EQUAL("Ace of Spades", oss.str());
}

TEST(is_face_or_ace) {
    ASSERT_TRUE(Card(JACK, SPADES).is_face_or_ace());
    ASSERT_TRUE(Card(QUEEN, HEARTS).is_face_or_ace());
    ASSERT_TRUE(Card(KING, CLUBS).is_face_or_ace());
    ASSERT_TRUE(Card(ACE, DIAMONDS).is_face_or_ace());
    ASSERT_FALSE(Card(TEN, SPADES).is_face_or_ace());
    ASSERT_FALSE(Card(NINE, HEARTS).is_face_or_ace());
}

TEST(is_right_bower) {
    ASSERT_TRUE(Card(JACK, HEARTS).is_right_bower(HEARTS));
    ASSERT_FALSE(Card(JACK, DIAMONDS).is_right_bower(HEARTS));
    ASSERT_FALSE(Card(ACE, HEARTS).is_right_bower(HEARTS));
}

TEST(is_left_bower) {
    ASSERT_TRUE(Card(JACK, DIAMONDS).is_left_bower(HEARTS));
    ASSERT_FALSE(Card(JACK, HEARTS).is_left_bower(HEARTS));
    ASSERT_FALSE(Card(JACK, SPADES).is_left_bower(HEARTS));
}

TEST(is_trump) {
    ASSERT_TRUE(Card(ACE, HEARTS).is_trump(HEARTS));
    ASSERT_TRUE(Card(JACK, DIAMONDS).is_trump(HEARTS));
    ASSERT_FALSE(Card(ACE, SPADES).is_trump(HEARTS));
    ASSERT_FALSE(Card(JACK, CLUBS).is_trump(HEARTS));
}

TEST(suit_next) {
    ASSERT_EQUAL(CLUBS, Suit_next(SPADES));
    ASSERT_EQUAL(SPADES, Suit_next(CLUBS));
    ASSERT_EQUAL(DIAMONDS, Suit_next(HEARTS));
    ASSERT_EQUAL(HEARTS, Suit_next(DIAMONDS));
}

TEST(rank_comparison) {
    ASSERT_TRUE(Card(NINE, SPADES) < Card(TEN, SPADES));
    ASSERT_TRUE(Card(TEN, SPADES) < Card(JACK, SPADES));
    ASSERT_FALSE(Card(ACE, SPADES) < Card(KING, SPADES));
}

TEST(suit_comparison) {
    Card c1(ACE, SPADES);
    Card c2(ACE, HEARTS);
    ASSERT_TRUE(c1 < c2);
    ASSERT_FALSE(c2 < c1);
}

TEST(trump_vs_non_trump) {
    ASSERT_TRUE(Card_less(Card(NINE, CLUBS), Card(NINE, HEARTS), HEARTS));
    ASSERT_FALSE(Card_less(Card(NINE, HEARTS), Card(NINE, CLUBS), HEARTS));
}

TEST(right_bower_vs_left_bower) {
    ASSERT_FALSE(Card_less(Card(JACK, HEARTS), Card(JACK, DIAMONDS), HEARTS));
    ASSERT_TRUE(Card_less(Card(JACK, DIAMONDS), Card(JACK, HEARTS), HEARTS));
}

TEST(left_bower_vs_trump) {
    ASSERT_FALSE(Card_less(Card(JACK, DIAMONDS), Card(ACE, HEARTS), HEARTS));
    ASSERT_TRUE(Card_less(Card(ACE, HEARTS), Card(JACK, DIAMONDS), HEARTS));
}

TEST(led_suit_vs_trump) {
    Card led(NINE, SPADES);
    ASSERT_TRUE(Card_less(Card(ACE, SPADES), Card(NINE, HEARTS), led, HEARTS));
    ASSERT_FALSE(Card_less(Card(NINE, HEARTS), Card(ACE, SPADES), led, HEARTS));
}

// operator== tests

// Basic equality
TEST(operator_equal_same_card) {
    Card c(ACE, SPADES);
    ASSERT_TRUE(c == c);
    ASSERT_FALSE(c != c);
}

TEST(operator_equal_identical) {
    Card c1(KING, HEARTS);
    Card c2(KING, HEARTS);
    ASSERT_TRUE(c1 == c2);
    ASSERT_FALSE(c1 != c2);
}

// Same rank, different suit => NOT equal
// Catches: only checking rank, or using ||
TEST(operator_equal_same_rank_diff_suit) {
    Card c1(ACE, SPADES);
    Card c2(ACE, HEARTS);
    ASSERT_FALSE(c1 == c2);
    ASSERT_TRUE(c1 != c2);
}

// Different rank, same suit => NOT equal
// Catches: only checking suit, or using ||
TEST(operator_equal_diff_rank_same_suit) {
    Card c1(ACE, SPADES);
    Card c2(KING, SPADES);
    ASSERT_FALSE(c1 == c2);
    ASSERT_TRUE(c1 != c2);
}

// Different rank AND different suit => NOT equal
TEST(operator_equal_totally_different) {
    Card c1(NINE, CLUBS);
    Card c2(ACE, DIAMONDS);
    ASSERT_FALSE(c1 == c2);
    ASSERT_TRUE(c1 != c2);
}

// Bowers: same rank (JACK), different suit
TEST(operator_equal_jacks_different_suits) {
    Card c1(JACK, HEARTS);
    Card c2(JACK, DIAMONDS);
    ASSERT_FALSE(c1 == c2);
    ASSERT_TRUE(c1 != c2);
}

// Same rank different suit - additional pairs to catch || bug
TEST(operator_equal_or_bug_same_suit_only) {
    // These share suit but not rank - with ||, would incorrectly be equal
    Card c1(ACE, SPADES);
    Card c2(NINE, SPADES);
    ASSERT_FALSE(c1 == c2);
    ASSERT_TRUE(c1 != c2);
}

TEST(operator_equal_or_bug_same_rank_only) {
    // These share rank but not suit - with ||, would incorrectly be equal
    Card c1(ACE, SPADES);
    Card c2(ACE, CLUBS);
    ASSERT_FALSE(c1 == c2);
    ASSERT_TRUE(c1 != c2);
}

// != is consistent with ==
TEST(operator_not_equal_consistent) {
    Card c1(TEN, HEARTS);
    Card c2(TEN, HEARTS);
    Card c3(TEN, CLUBS);
    ASSERT_FALSE(c1 != c2);
    ASSERT_TRUE(c1 != c3);
}

TEST(operator_equal_symmetry) {
    Card c1(TEN, DIAMONDS);
    Card c2(TEN, DIAMONDS);
    Card c3(TEN, HEARTS);
    ASSERT_TRUE(c1 == c2);
    ASSERT_TRUE(c2 == c1);
    ASSERT_FALSE(c1 == c3);
    ASSERT_FALSE(c3 == c1);
}

TEST_MAIN()
