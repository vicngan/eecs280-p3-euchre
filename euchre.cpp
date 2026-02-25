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
    int team0Points = 0;
    int team1Points = 0;
    int dealer_index = 0;
    int hand_number = 0;

    cout << players[dealer_index]->get_name() << " deals" << endl;

    while (team0Points < pointsToWin && team1Points < pointsToWin) {
        // reset and optionally shuffle the pack
        if (shuffle) {
            pack.shuffle();
        }
        pack.reset();

        // deal cards and flip upcard
        dealCards();

        // determine trump suit and which team ordered it up
        Suit trump;
        int order_up; // 0 = team 0 ordered, 1 = team 1 ordered
        makeTrump(trump, order_up);

        // play the hand and score it
        playHand(trump, order_up);

        // check for a winner
        if (team0Points >= pointsToWin) {
            cout << players[0]->get_name() << " and " << players[2]->get_name()
                 << " win!" << endl;
            return;
        }
        if (team1Points >= pointsToWin) {
            cout << players[1]->get_name() << " and " << players[3]->get_name()
                 << " win!" << endl;
            return;
        }

        // advance dealer for the next hand
        dealer_index = (dealer_index + 1) % 4;
        ++hand_number;
        cout << players[dealer_index]->get_name() << " deals" << endl;
    }
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

    int team0tricks = 0;
    int team1tricks = 0; 

    int leader_index = (dealer_index + 1) % 4; 
    for (int trick = 0; trick < 5; ++trick){
        //each play a card in turn, start with leader going clockwise
        Card ledCard = players[leader_index] -> lead_card(trump);
        std::cout << players[leader_index] -> get_name() << "leads" << ledCard << "\n";
        int winning_player = leader_index;
        Card highest_card = ledCard;

        for(int i = 0; i < 3; ++i){
            int current_player = (leader_index + i + 1) % 4; 
            Card currentCard = players[current_player] -> play_card(ledCard, trump);

            std::cout<< currentCard << " played by " << players[current_player] -> get_name() << "\n";
            //determine if current card wins the trick
            
            if (Card_less(highest_card, currentCard, ledCard, trump)){
                highest_card = currentCard;
                winning_player = current_player;
            }
        }

        std::cout << players[winning_player] -> get_name() << " wins the trick\n\n";

        if(winning_player % 2 == 0 || winning_player == 2){
            team0tricks++;
        } else {
            team1tricks++;
        }
        leader_index = winning_player; // winner of the trick leads the next trick
    }
    scoreHand(team0tricks, team1tricks, order_up);
}
void Game::scoreHand(int team0tricks, int team1tricks, int order_up) {
    // implementation of scoring the hand and updating points
    int winning_team;
    int pointsEarned;

    if (team0tricks > team1tricks){
        winning_team = 0;
        pointsEarned = team0tricks;
    } else {
        winning_team = 1;
        pointsEarned = team1tricks;
    }

    std::cout << players[winning_team]->get_name() << " and " 
              << players[winning_team + 2]->get_name() << " win the hand\n";

    if (winning_team == order_up){
        if (pointsEarned == 5){
            std::cout << "marched!\n";
            if (winning_team == 0) {
                team0Points += 2;
            } else {
                team1Points += 2;
            }
        } else {
            if (winning_team == 0) {
                team0Points += 1;
            } else {
                team1Points += 1;
            }
        }
    } else {
        if (winning_team == 0) {
            team0Points += pointsEarned;
        } else {
            team1Points += pointsEarned;
        } std::cout << "euchred!\n";
    }
    std::cout << players[0]->get_name() << " has " << team0Points << " points\n";
    std::cout << players[1]->get_name() << " has " << team1Points << " points\n";
}


