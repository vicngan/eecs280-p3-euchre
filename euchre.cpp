#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include "game.h"

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

    //helper functions
    void shufflePack(); // function to shuffle the pack
    void dealCards(); // function to deal cards to players
    void makeTrump(Suit &trump, int &order_up); // function to determine the trump suit
    void playHand(Suit trump, int order_up); // function to play a hand of euchre
    void scoreHand(int team0tricks, int team1tricks, int order_up); // function to score the hand and update points
};



