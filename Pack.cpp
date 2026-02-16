#include "Pack.hpp"
#include <algorithm>
#include <array>

Pack::Pack(){
    int index = 0; 
    next = 0; //initialize next index to 0

    //outer loop lowest -> highest suit (spade -> diamonds)
    for (int s = SPADES; s <= DIAMONDS; ++s){
        //inner loop lowest -> highest rank (9-> Ace)
        for (int r = NINE; r <= ACE; ++r){
            //cast int to enum types 
            Suit current_suit = static_cast<Suit>(s); 
            Rank current_rank = static_cast<Rank>(r);
            //create card and add to pack 
            cards[index] = Card(current_rank, current_suit);
            index++; 
        }
    }
}

Pack::Pack(std::istream& pack_input){
    next = 0;
    for (int i = 0; i < PACK_SIZE; ++i){
        pack_input >> cards[i];
    }
}

Card Pack::deal_one(){
    Card next_card = cards[next];
    next++;
    return next_card;
}

void Pack::reset(){
    next= 0;
}

void Pack::shuffle(){ 
    const int PACK_SIZE = 24;
    
    for (int i = 0; i < 7; ++i){ //run outer loop 7 times 
        std::array<Card, PACK_SIZE> shuffled_cards;
        int T = 0; //index top half
        int B = PACK_SIZE/2; //index bottom half
        for (int j = 0; j < PACK_SIZE; ++j){ //inner loop swap cards in top half [:size/2] with bottom [size/2:size]
            if (j%2 == 0){ //even index, take card from bottom half
                shuffled_cards[j] = cards[B++];
            } else { //odd index, take card from top half
                shuffled_cards[j] = cards[T++];
            }
        }
        cards = shuffled_cards;
    }
    next = 0; //reset next index to 0 after shuffle
}

bool Pack::empty() const{
    if (next >= PACK_SIZE){ //if next index is bigger/equal to size, then pack is empty 
        return true;
    }
    else {
        return false;
    }
}





