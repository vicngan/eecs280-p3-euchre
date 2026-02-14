#include "Pack.hpp"

Pack::Pack(){
    int index = 0; 

    //outer loop lowest -> highest suit (spade -> diamonds)
    for (int s = SPADES; s <= DIAMONDS; ++s){
        //inner loop lowest -> highest rank (2-> Ace)
        for (int r = TWO; r <= ACE; ++r){
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
    int index = 0;
    for (int i = 0; i < PACK_SIZE; ++i){
        pack_input >> cards[index];
        index++; 
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
    for (int i = 0; i < 7; ++i){
        for (int j = 0; j < PACK_SIZE/2; ++j){
            
        })
    }
}




