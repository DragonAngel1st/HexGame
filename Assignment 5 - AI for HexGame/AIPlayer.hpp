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


using namespace std;

class AIPlayer
{
public:
    HexNodeState playerColor;
    AIPlayer(const HexNodeState playerColor): playerColor(playerColor){};
    HexNode* getBestNextMoveEvaluation(HexGameGraph* originalHexGameGraphPtr);
private:
    bool _isScenarioAWin(PriorityQueue<HexNode>* pq);
    int _getRandomNodeID(int gameSize);
};

//PriorityQueue<HexNode> AIPlayer::_createRandomWinningBoardFrom(HexGameGraph* _hexGraphPtr)
//{
////    PriorityQueue<HexNode> pq;
////
//    return NULL;
//}
#endif /* AIPlayer_hpp */
