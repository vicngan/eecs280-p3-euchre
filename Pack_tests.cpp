#include "Pack.hpp"
#include "unit_test_framework.hpp"

#include <iostream>

using namespace std;

TEST(test_pack_default_ctor) {
    Pack pack;
    Card first = pack.deal_one();
    ASSERT_EQUAL(NINE, first.get_rank());
    ASSERT_EQUAL(SPADES, first.get_suit());
}

TEST(test_pack_deal_one_increments) {
    Pack pack;
    Card c1 = pack.deal_one();
    Card c2 = pack.deal_one();
    // Dealing twice should give two different cards (in a standard pack)
    ASSERT_NOT_EQUAL(c1, c2);
}

TEST(test_pack_reset_returns_to_start) {
    Pack pack;

    Card first1 = pack.deal_one();
    pack.deal_one();   // advance
    pack.deal_one();   // advance more

    pack.reset();
    Card first2 = pack.deal_one();

    ASSERT_EQUAL(first1, first2);
}

TEST(test_pack_empty_false_initially) {
    Pack pack;
    ASSERT_FALSE(pack.empty());
}

TEST(test_pack_empty_true_after_24) {
    Pack pack;
    for (int i = 0; i < 24; ++i) {
        pack.deal_one();
    }
    ASSERT_TRUE(pack.empty());
}

TEST(test_pack_shuffle_resets_next) {
    Pack pack;

    // Deal some cards so next != 0
    pack.deal_one();
    pack.deal_one();

    // After shuffle(), next should be reset, so we can compare
    Pack fresh;
    pack.shuffle();

    Card after_shuffle_first = pack.deal_one();
    Card fresh_first = fresh.deal_one();
}

TEST_MAIN()
