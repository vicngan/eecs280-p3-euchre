#include "Player.hpp"
#include <vector>
#include <algorithm>
#include <iostream>
#include <cassert>

using namespace std;

class Simple : public Player {
private: 
    std::string name;
    std::vector<Card> hand;

public:
    Simple(const std::string &name_in) : name(name_in){}

    //return name of player 
    const std::string & get_name() const override {
        return name; 
    }
    //add card to hand and sort 
    void add_card(const Card &c) override{
        hand.push_back(c);
        std::sort(hand.begin(), hand.end());
    }
    //make trump if have two or more trump cards, otherwise pass
    bool make_trump(const Card &upcard, bool is_dealer, int round, Suit &order_up_suit) const override{
        int trump_count = 0;
        for (const Card &c : hand){
            if (c.is_trump(upcard.get_suit())){
                trump_count++;
            }
        }
        //round 1
        if (trump_count >= 2){
            order_up_suit = upcard.get_suit();
            return true;
        }

        //round 2
        if (round == 2 && trump_count >= 1){
            order_up_suit = Suit_next(upcard.get_suit());
            return true;
        }

        //screw the dealer (round 2 only)
        if (is_dealer && round == 2){
            order_up_suit = Suit_next(upcard.get_suit());
            return true;
        }
        return false;
    }
    //add upcard to hand and discard lowest card
    void add_and_discard(const Card &upcard) override {
        add_card(upcard);
        hand.pop_back();
    }
    //lead card with highest suit (trump > non-trump) and rank
    Card lead_card(Suit trump) override {
        std::sort(hand.begin(), hand.end(), [&](const Card &a, const Card &b){
            return Card_less(a,b , trump);
        });
        for (const Card &c :hand){
            if(c.is_trump(trump)){
                //play highest trump card
                Card lead = c; 
                hand.erase(std::remove(hand.begin(), hand.end(), c), hand.end());
                return lead;
            } else {
                //play highest non-trump card
                Card lead = hand.back();
                hand.pop_back();
                return lead;
            }
        }
    }
    //play card when someone already led, otherwise play lowest
    Card play_card (const Card &led_card, Suit trump) override {
        //find all cards that follow suit 
        std::vector<Card> follow_suit_cards;
        for (const Card &c : hand){
            if (c.get_suit(trump) == led_card.get_suit(trump)){
                follow_suit_cards.push_back(c);
            }
        } 
        if (!follow_suit_cards.empty()){
            //play highest card that follows suit
            std::sort(follow_suit_cards.begin(), follow_suit_cards.end(), [&](const Card &a, const Card &b){
                return Card_less(a,b , trump);
            });
            Card play = follow_suit_cards.back();
            hand.erase(std::remove(hand.begin(), hand.end(), play), hand.end());
            return play;
        }
        //play lowest card if can't follow suit 
        Card play = hand.front();
        hand.erase(hand.begin());
        return play;
    }
};

class Human : public Player{
private:
    std::string name;
    std::vector<Card> hand;

    void print_hand() const {
        for (size_t i = 0; i < hand.size(); ++i){
            cout << "Human" << name << "hand:" << "["<<i<<"]" << hand[i] << "\n";
        }
    }
    
public: 
    Human(const std::string &name_in) : name(name_in){}

    const std::string & get_name() const override{
        return name;
    }
    
    void add_card(const Card &c) override{
        hand.push_back(c); 
        std::sort(hand.begin(), hand.end());
    }

    bool make_trump(const Card &upcard, bool is_dealer, int round, Suit &order_up_suit) const override{
        print_hand(); 
        cout << "Player" << name << "enter suit to order up " << upcard.get_suit() << " or pass: \n";
        string choice;
        cin >> choice;
        if (choice == "pass"){
            return false;
        } else {
            order_up_suit = string_to_suit(choice);
            return true;
        }
    }

    void add_and_discard(const Card &upcard) override{
        print_hand();
        cout << "Player" << name << "enter index of card to discard: \n";
        int index; 
        cin>> index;
        add_card(upcard);
        hand.erase(hand.begin() + index);
    }

    Card lead_card(Suit trump) override{
        print_hand();
        cout << "Player" << name << "enter index of card to lead: \n";
        int index; 
        cin >> index; 
        Card lead = hand[index];
        hand.erase(hand.begin() + index);
        return lead;
    }

    Card play_card(const Card &led_card, Suit trump) override{
        print_hand();
        cout << "Player" << name << "enter index of card to play: \n";
        int index; 
        cin >> index; 
        Card play = hand[index];
        hand.erase(hand.begin() + index);
        return play;
    }
};

Player * Player_factory(const std::string &name, 
                        const std::string &strategy) {
  // We need to check the value of strategy and return 
  // the corresponding player type.
  if (strategy == "Simple") {
    // The "new" keyword dynamically allocates an object.
    return new Simple(name);
  }
  if (strategy == "Human") {
    return new Human(name);
  }
  // Invalid strategy if we get here
  assert(false);
  return nullptr;
}
std::ostream & operator<<(std::ostream &os, const Player &p) {
  os << p.get_name();
  return os;
}