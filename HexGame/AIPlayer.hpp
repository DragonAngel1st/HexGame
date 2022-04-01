//
//  AIPlayer.cpp
//  Assignment 5 - AI for HexGame
//
//  Created by Patrick Miron on 2022-02-15.
//
#ifndef AIPlayer_hpp
#define AIPlayer_hpp

#include <random>
#include <set>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include "HexNodeState.hpp"
#include "HexNode.hpp"
#include "HexGameGraph.hpp"
#include "HexGameHelperFunctions.hpp"


using namespace std;

class AIPlayer
{
public:
    int gameSize; // Size of one of the dimensions of the game. Note the game board is always square -> gameSize by gameSize.
    int aiIterations; // Number of try the AI will full up game board to find winning move. Can thought of AI Strenght(smartness)
    
    AIPlayer(); // Instantiate an null player
    AIPlayer(const int gameSize, const int aiIterations): gameSize(gameSize), aiIterations(aiIterations){}; //Create an AIPlayer
    
    HexNode* getPossibleWinningMove(HexGameGraph* hexGameGraphPtr, HexNodeState playerColor); //AI test for the best next move
    
    ~AIPlayer(); //Release resources.
private:
    //AI query to test if test board is a win for itself.
    bool _isScenarioAWin(HexGameGraph* hexGameGraphPtr, HexNodeState playerColor);

};

#endif /* AIPlayer_hpp */
