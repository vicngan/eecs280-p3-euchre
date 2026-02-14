#include "Pack.hpp"

Pack::Pack(){
    int index = 0; 

    //outer loop lowest -> highest suit (spade -> diamonds)
    for (int s = SPADES; s <= DIAMONDS; ++s){
        //inner loop lowest -> highest rank (2-> Ace)
        for (int r = TWO; r <= ACE; ++r){
            Suit current_suit = static_cast<Suit>(s); 
    }
}


