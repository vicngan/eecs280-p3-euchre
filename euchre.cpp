#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>

#include "Player.hpp"
#include "Pack.hpp"
#include "Card.hpp"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 12){
        cout<< "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
            << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
            << "NAME4 TYPE4" << endl;
        return 1;
    }
    //validate points to win; must be between 1 and 100
    int pointsToWin = atoi(argv[3]);
    if (pointsToWin < 1 || pointsToWin > 100){
        cout<< "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
            << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
            << "NAME4 TYPE4" << endl;
        return 1;
    }
    // validate shuffle option; must be either "shuffle" or "noshuffle"
    string shuffleOption = argv[2];
    if (shuffleOption != "shuffle" && shuffleOption != "noshuffle"){
        cout<< "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
            << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
            << "NAME4 TYPE4" << endl;
        return 1;
    }
    //validate player types; must be either "human" or "simple"
    for(int i = 5; i <= 11; i+=2){
        string playerType = argv[i];
        if (playerType != "human" && playerType != "simple"){
            cout<< "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
                << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
                << "NAME4 TYPE4" << endl;
            return 1;
        }
    }
    // open the pack file and read the cards
    string packFilename = argv[1];
    ifstream fin(packFilename);
    if(!fin.is_open()){
        cout<< "Error: Could not open file " << packFilename << endl;
        return 1;
    }

    //print command line arguments for debugging
    for (int i = 0; i < argc; ++i){
        cout << argv[i] << " ";
    }
    cout << endl;

    // determine whether to shuffle the pack based on the command line argument
    bool shuffle = (shuffleOption == "shuffle");
    
    vector<string> playerNames = {argv[4], argv[6], argv[8], argv[10]};
    vector<string> playerTypes = {argv[5], argv[7], argv[9], argv[11]};
    
    // create the game object and start the game
    Game game(fin, shuffle, pointsToWin, playerNames, playerTypes);
    game.play();
    return 0; 
}

class Game {
public:
    // constructor implementation
    Game(std::ifstream& fin, bool shuffle, int pointsToWin, 
         const std::vector<std::string>& playerNames, 
         const std::vector<std::string>& playerTypes);

    //main game loop implementation
    void play();

    //destructor implementation
    ~Game();

private:
    std::vector<Player*> players; // vector of player pointers
    Pack pack; // pack of cards
    bool shuffle; // whether to shuffle the pack
    int pointsToWin; // points needed to win the game

    //game state variables
    int team0Points; // points for team 0
    int team1Points; // points for team 1
    int hand_number; // current hand number
    int dealer_index; // index of the current dealer
    Card upcard; // the upcard dealt at the start of a hand

    //helper functions
    void shufflePack(); // function to shuffle the pack
    void dealCards(); // function to deal cards to players
    void makeTrump(Suit &trump, int &order_up); // function to determine the trump suit
    void playHand(Suit trump, int order_up); // function to play a hand of euchre
    void scoreHand(int team0tricks, int team1tricks, int order_up); // function to score the hand and update points
};
// Implementation of Game class member functions


Game::Game(std::ifstream& fin, bool shuffle, int pointsToWin, 
           const std::vector<std::string>& playerNames, 
           const std::vector<std::string>& playerTypes) {
        // implementation of the constructor
}
void Game::shufflePack(){
    // implementation of shuffling the pack
    pack.shuffle();
    pack.reset();
}
void Game::play() {
    // implementation of the main game loop
}
void Game::dealCards() {
    // implementation of dealing cards to players
    int currentPlayer = (dealer_index + 1) % 4; // start dealing with the player to the left of the dealer

    //define dealing batches 
    int round1[] = {3,2,3,2};
    int round2[] = {2,3,2,3};

    //round 1 
    for (int i = 0; i < 4; ++i){
        int cardsToDeal = round1[i];
        for (int j = 0; j < cardsToDeal; ++j){
            players[currentPlayer]->add_card(pack.deal_one());
        }
        currentPlayer = (currentPlayer + 1) % 4;
    }
    //round 2
    for (int i = 0; i < 4; ++i){
        int cardsToDeal = round2[i];
        for (int j = 0; j < cardsToDeal; ++j){
            players[currentPlayer]->add_card(pack.deal_one());
        }
        currentPlayer = (currentPlayer + 1) % 4;
    }
    //deal the upcard
    upcard = pack.deal_one();

    //print 
    std::cout << upcard << " turned up\n";

}

void Game::makeTrump(Suit &trump, int &order_up) {
    // implementation of determining the trump suit
    //round 1 
    int currentPlayer = (dealer_index + 1) % 4; // start with the player to the left of the dealer

    for (int i =0; i < 4; ++i){
        bool isDealer = (currentPlayer == dealer_index);
        Suit orderedSuit;
        
        //ask each player in turn if they want to order up the trump suit. If a player orders up, set the trump suit and break out of the loop. If all players pass, move on to round 2.
        if (players[currentPlayer]->make_trump(upcard, isDealer, 1, orderedSuit)){
            trump = orderedSuit;
            order_up = currentPlayer % 2; //team 0 or team 1
            
            std::cout << players[currentPlayer]->get_name() << " orders up " << orderedSuit<< "\n";

            //if trump is made, dealer must add the upcard to their hand and discard a card
            players[dealer_index]->add_and_discard(upcard);

            std::cout << "\n";
            return;
        } else {
            std::cout << players[currentPlayer]->get_name() << " passes\n";
        }
        currentPlayer = (currentPlayer + 1) % 4;
    }
    //round 2(only if all 4 players pass in round 1)
    currentPlayer = (dealer_index + 1) % 4; // reset to the player to the left of the dealer
    for (int i =0; i < 4; ++i){
        bool isDealer = (currentPlayer == dealer_index);
        Suit orderedSuit;

        if (players[currentPlayer]->make_trump(upcard, isDealer, 2, orderedSuit)){
            trump = orderedSuit;
            order_up = currentPlayer % 2; //team 0 or team 1

            std::cout << players[currentPlayer]->get_name() << " orders up " << orderedSuit<< "\n";

            std::cout << "\n";
            return;
        } else {
            std::cout << players[currentPlayer]->get_name() << " passes\n";
        }
        currentPlayer = (currentPlayer + 1) % 4;
    }
}

void Game::playHand(Suit trump, int order_up) {
    // implementation of playing a hand of euchre
}
void Game::scoreHand(int team0tricks, int team1tricks, int order_up) {
    // implementation of scoring the hand and updating points
}

