//
//  AIPlayer.cpp
//  Assignment 5 - AI for HexGame
//
//  Created by Patrick Miron on 2022-02-16.
//

#include <stdio.h>
#include "AIPlayer.hpp"

HexNode* AIPlayer::getBestNextMoveEvaluation(HexGameGraph* originalHexGameGraphPtr)
{
    HexNode* nodePtr = nullptr;
    //PriorityQueue<HexNode>* pqPtr = new PriorityQueue<HexNode>();
    HexGameGraph* ptrToTempGraph = new HexGameGraph(5);
    vector<int> vectorOfUnoccupiedNodes;
    vector<int> vectorOfOwnedSquares;
    vector<int> vectorOfOpponentSquares;
    
    for (auto node : originalHexGameGraphPtr->getNodes())
    {
        
        if (node->state == HexNodeState::UNOCCUPIED)
        {
            vectorOfUnoccupiedNodes.push_back(node->nodeID);
        }
        else if (node->state == this->playerColor)
        {
            vectorOfOwnedSquares.push_back(node->nodeID);
        }
        else
        {
            assert(node->state != playerColor || node->state != HexNodeState::UNOCCUPIED);
            vectorOfOpponentSquares.push_back(node->nodeID);
        }
    }
    random_device os_seed;
    const int seed = os_seed();
    
    mt19937 generator( seed );
    
    shuffle(vectorOfUnoccupiedNodes.begin(), ( vectorOfUnoccupiedNodes.end()), generator);
    int nodeCount = 0;
    for (auto node : vectorOfUnoccupiedNodes)
    {
        if (nodeCount < vectorOfUnoccupiedNodes.size()/2)
        {
            ptrToTempGraph->getNodePtr(node)->state = playerColor;
        }
        else
        {
            if (playerColor == HexNodeState::BLUE)
            {
                ptrToTempGraph->getNodePtr(node)->state = HexNodeState::RED;
            }
            else
            {
                ptrToTempGraph->getNodePtr(node)->state = HexNodeState::BLUE;
            }
        }
    }
    ptrToTempGraph->print();
    
    return nullptr;
}

bool AIPlayer::_isScenarioAWin(PriorityQueue<HexNode>* pq)
{
    return true;
}

int AIPlayer::_getRandomNodeID(int gameSize)
{
    random_device os_seed;
    const int seed = os_seed();
    
    mt19937 generator( seed );
    uniform_int_distribution<int> distribute( 0, gameSize*gameSize );
    int nodeID = distribute(generator);
    
    return nodeID;
}
