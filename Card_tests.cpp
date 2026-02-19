#include "Card.hpp"
#include "unit_test_framework.hpp"
#include <iostream>

using namespace std;


TEST(test_card_ctor) {
    Card c(ACE, HEARTS);
    ASSERT_EQUAL(ACE, c.get_rank());
    ASSERT_EQUAL(HEARTS, c.get_suit());
}
TEST(print_card){
    Card c(ACE, HEARTS);
    ostringstream oss;
    oss << c;
    ASSERT_EQUAL(oss.str(), "Ace of Hearts");
}  

TEST(rank_comparison){
    Card c1(ACE, HEARTS);
    Card c2(KING, HEARTS);
    Card c3(QUEEN, HEARTS);
    Card c4(JACK, HEARTS);
    Card c5(TEN, HEARTS);
    Card c6(NINE, HEARTS);

    ASSERT_TRUE(c1.get_rank() > c2.get_rank());
    ASSERT_TRUE(c2.get_rank() > c3.get_rank());
    ASSERT_TRUE(c3.get_rank() > c4.get_rank());
    ASSERT_TRUE(c4.get_rank() > c5.get_rank());
    ASSERT_TRUE(c5.get_rank() > c6.get_rank());
}

TEST(suit_comparison){
    Card c1(ACE, SPADES);
    Card c2(ACE, HEARTS);
    Card c3(ACE, DIAMONDS);
    Card c4(ACE, CLUBS);

    ASSERT_EQUAL(c1.get_suit(), SPADES);
    ASSERT_EQUAL(c2.get_suit(), HEARTS);
    ASSERT_EQUAL(c3.get_suit(), DIAMONDS);
    ASSERT_EQUAL(c4.get_suit(), CLUBS);
}
TEST(is_face_or_ace){
    Card c1(ACE, HEARTS);
    Card c2(JACK, HEARTS);
    Card c3(TEN, HEARTS);

    ASSERT_TRUE(c1.is_face_or_ace());
    ASSERT_TRUE(c2.is_face_or_ace());
    ASSERT_FALSE(c3.is_face_or_ace());
}

TEST(suit_next){
    ASSERT_EQUAL(Suit_next(SPADES), CLUBS);
    ASSERT_EQUAL(Suit_next(HEARTS), DIAMONDS);
    ASSERT_EQUAL(Suit_next(CLUBS), SPADES);
    ASSERT_EQUAL(Suit_next(DIAMONDS), HEARTS);
}

TEST(is_left_bower){
    Card c1(JACK, DIAMONDS);
    Card c2(JACK, SPADES);
    Card c3(JACK, HEARTS);

    ASSERT_TRUE(c1.is_left_bower(HEARTS));
    ASSERT_FALSE(c2.is_left_bower(HEARTS));
    ASSERT_FALSE(c3.is_left_bower(HEARTS));
}

TEST(is_right_bower){
    Card c1(JACK, HEARTS);
    Card c2(JACK, SPADES);
    Card c3(JACK, DIAMONDS);

    ASSERT_TRUE(c1.is_right_bower(HEARTS));
    ASSERT_FALSE(c2.is_right_bower(HEARTS));
    ASSERT_FALSE(c3.is_right_bower(HEARTS));
}

TEST(is_trump){
    Card c1(JACK, HEARTS);
    Card c2(JACK, DIAMONDS);
    Card c3(JACK, SPADES);
    
    ASSERT_TRUE(c1.is_trump(HEARTS));
    ASSERT_TRUE(c2.is_trump(HEARTS));
    ASSERT_FALSE(c3.is_trump(HEARTS));

    ASSERT_TRUE(c1.is_trump(DIAMONDS));
    ASSERT_TRUE(c2.is_trump(DIAMONDS));
    ASSERT_FALSE(c3.is_trump(DIAMONDS));
}

TEST(trump_vs_non_trump){
    Suit trump = SPADES;
    Card trump_card (NINE, SPADES);
    Card non_trump_card (ACE, HEARTS);

    ASSERT_TRUE(Card_less(non_trump_card, trump_card, trump));  
    ASSERT_FALSE(Card_less(trump_card, non_trump_card, trump));
}

TEST(right_bower_vs_left_bower){
    Suit trump = DIAMONDS;
    Card right_bower (JACK, DIAMONDS);
    Card left_bower (JACK, SPADES); 

    ASSERT_TRUE(Card_less(left_bower, right_bower, trump));
    ASSERT_FALSE(Card_less(right_bower, left_bower, trump));
}

TEST(left_bower_vs_trump){
    Suit trump = CLUBS;
    Card left_bower (JACK, SPADES); 
    Card trump_card (ACE, CLUBS);

    ASSERT_TRUE(Card_less(trump_card, left_bower, trump));
    ASSERT_FALSE(Card_less(left_bower, trump_card, trump));
}

TEST(led_suit_vs_trump){
    Suit trump = HEARTS;
    Card led_card (NINE, CLUBS);

    Card follower(KING, CLUBS);
    Card trump_card (ACE, SPADES);

    ASSERT_TRUE(Card_less(trump_card, follower, led_card, trump));
}

TEST_MAIN()
