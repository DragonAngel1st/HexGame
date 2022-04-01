//
//  HexGame.hpp
//  Part B - Assignment 4 - Hex Game
//
//  Created then Modified by Patrick Miron on 2022-01-25.
//

#ifndef HexGame_hpp
#define HexGame_hpp

#include <stdio.h>
#include <tuple>
#include "HexGameGraph.hpp"
#include "Player.hpp"
#include "PlayerType.hpp"
#include "AIPlayer.hpp"

using namespace std;

//Having transformed all graph, node, edge, mst, priority_queue, it was necessary to put all implementation of those classes into the .hpp "header" file since they are created during compile time and not instantiated until needed.
//HexGame was also created the same way to keep it the same as the whole project.
class HexGame
{
public:
    bool isActive;
    int gameSize;
    Player player1;
    Player player2;
    AIPlayer aiBrain;
    //Start a game
    HexGame(int gameSize = 5);
    ~HexGame();
    bool _checkIfPlayerHasWonFromLastMove(Player currentPlayer, HexNode * nodePtr);
private:
    HexGameGraph * _gameGraphPtr;
    void _startGameLoop();
    bool _verifyCoordinates(int row, int col);
    bool _isGameSquareOccupied(int row, int col);
    HexNode * _askPlayerForCoordinates(Player currentPlayer);
    
    void _playerHasWon(Player currentPlayer);
    
    void _printGameBoard();
    void _printTitle();
    tuple<int, int> _getCoordinatesFromNodeID(int nodeID);
};



#endif /* HexGame_hpp */
