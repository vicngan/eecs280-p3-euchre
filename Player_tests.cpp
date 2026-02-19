#include "Player.hpp"
#include "unit_test_framework.hpp"

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

TEST(test_player_get_name) {
    Player * alice = Player_factory("Alice", "Simple");
    ASSERT_EQUAL("Alice", alice->get_name());

    delete alice;
}

TEST(test_simple_make_trump_round1) {
    Player * p = Player_factory("P", "Simple");
    p->add_card(Card(ACE, HEARTS));
    p->add_card(Card(JACK, DIAMONDS)); // left bower if Hearts are trump
    p->add_card(Card(NINE, SPADES));
    p->add_card(Card(TEN, SPADES));
    p->add_card(Card(QUEEN, CLUBS));

    Suit order_up = SPADES;
    bool ordered = p->make_trump(Card(NINE, HEARTS), false, 1, order_up);

    ASSERT_TRUE(ordered);
    ASSERT_EQUAL(order_up, HEARTS);
    delete p;
}

TEST(test_simple_make_trump_round1_pass_without_two_face_aces) {
    Player * p = Player_factory("P", "Simple");
    p->add_card(Card(NINE, HEARTS));
    p->add_card(Card(TEN, HEARTS));
    p->add_card(Card(ACE, SPADES));
    p->add_card(Card(KING, CLUBS));
    p->add_card(Card(QUEEN, DIAMONDS));

    Suit order_up = CLUBS;
    bool ordered = p->make_trump(Card(NINE, HEARTS), false, 1, order_up);

    ASSERT_FALSE(ordered);
    ASSERT_EQUAL(order_up, CLUBS);
    delete p;
}

TEST(test_simple_make_trump_round2) {
    Player * p = Player_factory("P", "Simple");
    p->add_card(Card(ACE, SPADES)); // next suit to Clubs
    p->add_card(Card(NINE, HEARTS));
    p->add_card(Card(TEN, HEARTS));
    p->add_card(Card(QUEEN, DIAMONDS));
    p->add_card(Card(KING, DIAMONDS));

    Suit order_up = HEARTS;
    bool ordered = p->make_trump(Card(NINE, CLUBS), false, 2, order_up);

    ASSERT_TRUE(ordered);
    ASSERT_EQUAL(order_up, SPADES);
    delete p;
}

TEST(test_simple_make_trump_round2_pass_without_next_suit_face_ace) {
    Player * p = Player_factory("P", "Simple");
    p->add_card(Card(ACE, HEARTS));
    p->add_card(Card(KING, HEARTS));
    p->add_card(Card(QUEEN, DIAMONDS));
    p->add_card(Card(NINE, DIAMONDS));
    p->add_card(Card(TEN, DIAMONDS));

    Suit order_up = DIAMONDS;
    bool ordered = p->make_trump(Card(NINE, CLUBS), false, 2, order_up);

    ASSERT_FALSE(ordered);
    ASSERT_EQUAL(order_up, DIAMONDS);
    delete p;
}

TEST(test_simple_make_trump_round2_screw_dealer) {
    Player * p = Player_factory("P", "Simple");
    p->add_card(Card(ACE, HEARTS));
    p->add_card(Card(KING, HEARTS));
    p->add_card(Card(QUEEN, DIAMONDS));
    p->add_card(Card(NINE, DIAMONDS));
    p->add_card(Card(TEN, DIAMONDS));

    Suit order_up = DIAMONDS;
    bool ordered = p->make_trump(Card(NINE, CLUBS), true, 2, order_up);

    ASSERT_TRUE(ordered);
    ASSERT_EQUAL(order_up, SPADES);
    delete p;
}

TEST(test_simple_add_and_discard_discards_lowest) {
    Player * p = Player_factory("P", "Simple");
    p->add_card(Card(ACE, HEARTS));
    p->add_card(Card(KING, HEARTS));
    p->add_card(Card(QUEEN, SPADES));
    p->add_card(Card(TEN, SPADES));
    p->add_card(Card(NINE, DIAMONDS));

    p->add_and_discard(Card(JACK, HEARTS)); // trump is Hearts

    vector<Card> actual;
    Card led(NINE, CLUBS); // no clubs in hand, so always play lowest
    for (int i = 0; i < Player::MAX_HAND_SIZE; ++i) {
        actual.push_back(p->play_card(led, HEARTS));
    }

    vector<Card> expected = {
        Card(ACE, HEARTS),
        Card(KING, HEARTS),
        Card(JACK, HEARTS),
        Card(QUEEN, SPADES),
        Card(TEN, SPADES),
    };

    sort(actual.begin(), actual.end());
    sort(expected.begin(), expected.end());
    ASSERT_SEQUENCE_EQUAL(actual, expected);
    delete p;
}

TEST(test_simple_add_and_discard_keeps_left_bower_and_trump) {
    Player * p = Player_factory("P", "Simple");
    p->add_card(Card(JACK, DIAMONDS)); // left bower for Hearts
    p->add_card(Card(ACE, CLUBS));
    p->add_card(Card(KING, CLUBS));
    p->add_card(Card(QUEEN, CLUBS));
    p->add_card(Card(ACE, SPADES));

    p->add_and_discard(Card(NINE, HEARTS)); // trump is Hearts

    vector<Card> actual;
    Card led(NINE, DIAMONDS); // no diamonds in hand (left bower counts as trump)
    for (int i = 0; i < Player::MAX_HAND_SIZE; ++i) {
        actual.push_back(p->play_card(led, HEARTS));
    }

    vector<Card> expected = {
        Card(JACK, DIAMONDS),
        Card(ACE, CLUBS),
        Card(KING, CLUBS),
        Card(ACE, SPADES),
        Card(NINE, HEARTS),
    };

    sort(actual.begin(), actual.end());
    sort(expected.begin(), expected.end());
    ASSERT_SEQUENCE_EQUAL(actual, expected);
    delete p;
}

TEST(test_simple_lead_card_highest_non_trump) {
    Player * p = Player_factory("P", "Simple");
    p->add_card(Card(ACE, SPADES));
    p->add_card(Card(TEN, SPADES));
    p->add_card(Card(QUEEN, CLUBS));
    p->add_card(Card(KING, HEARTS));
    p->add_card(Card(NINE, HEARTS));

    Card led = p->lead_card(HEARTS);
    ASSERT_EQUAL(led, Card(ACE, SPADES));
    delete p;
}

TEST(test_simple_lead_card_all_trump) {
    Player * p = Player_factory("P", "Simple");
    p->add_card(Card(JACK, HEARTS));   // right bower
    p->add_card(Card(JACK, DIAMONDS)); // left bower
    p->add_card(Card(ACE, HEARTS));
    p->add_card(Card(KING, HEARTS));
    p->add_card(Card(NINE, HEARTS));

    Card led = p->lead_card(HEARTS);
    ASSERT_EQUAL(led, Card(JACK, HEARTS));
    delete p;
}

TEST(test_simple_play_card_follow_suit_highest) {
    Player * p = Player_factory("P", "Simple");
    p->add_card(Card(ACE, SPADES));
    p->add_card(Card(TEN, SPADES));
    p->add_card(Card(KING, CLUBS));
    p->add_card(Card(JACK, HEARTS));
    p->add_card(Card(NINE, DIAMONDS));

    Card played = p->play_card(Card(NINE, SPADES), HEARTS);
    ASSERT_EQUAL(played, Card(ACE, SPADES));
    delete p;
}

TEST(test_simple_play_card_left_bower_not_follow) {
    Player * p = Player_factory("P", "Simple");
    p->add_card(Card(JACK, DIAMONDS)); // left bower for Hearts
    p->add_card(Card(ACE, SPADES));
    p->add_card(Card(KING, CLUBS));
    p->add_card(Card(NINE, HEARTS));
    p->add_card(Card(TEN, SPADES));

    Card played = p->play_card(Card(NINE, DIAMONDS), HEARTS);
    ASSERT_EQUAL(played, Card(TEN, SPADES));
    delete p;
}

TEST_MAIN()
