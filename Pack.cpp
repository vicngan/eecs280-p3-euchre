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


