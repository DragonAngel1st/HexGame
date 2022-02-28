//
//  AIPlayer.cpp
//  Assignment 5 - AI for HexGame
//
//  Created by Patrick Miron on 2022-02-16.
//

#include <stdio.h>
#include "AIPlayer.hpp"
#include <map>

AIPlayer::AIPlayer()
{
    //Default constructor (EMPTY AIPlayer)
};

HexNode* AIPlayer::getPossibleWinningMove(HexGameGraph* originalHexGameGraphPtr, HexNodeState playerColor)
{
    //Store nodes in
    map<int,int> mapOfNextMoveWinningNodes;
    
    //Start Timer
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();
    
    //Test out a number of times (aiIterations) game board with random moves
    for (int epoch = 0 ; epoch < aiIterations ; epoch++ )
    {
        //Here we create vectors of integer, the integer being the nodeID
        //The reason we use simple vectors it's much faster to work with just the nodeID without the extra space and copy time of a Node or Node pointer and we do not need anything else to assign a certain square color to a space (Node).
        vector<int> vectorOfUnoccupiedNodes;
        vector<int> vectorOfOwnedSquares;
        vector<int> vectorOfOpponentSquares;
        
        //For each node in the current game graph at this turn, we seperate each node in it's proper vector of integers.
        // all unoccupied node IDs are store in vectorOfUnoccupiedNodes, all nodes occupied by AIPlayer goes into vectorOfOwnedSquares and all oponent square node IDs goes into vectorOfOpponentSquares.
        for (auto node : originalHexGameGraphPtr->getNodes())
        {
            
            if (node->state == HexNodeState::UNOCCUPIED)
            {
                vectorOfUnoccupiedNodes.push_back(node->nodeID);
            }
            else if (node->state == playerColor)
            {
                vectorOfOwnedSquares.push_back(node->nodeID);
            }
            else
            {
                assert(node->state != playerColor || node->state != HexNodeState::UNOCCUPIED);
                vectorOfOpponentSquares.push_back(node->nodeID);
            }
        }
        //Create a random device using the RANDOM type from the standard library
        random_device os_seed;
        //Create a seed.
        const int seed = os_seed();
        //Instantiate the generator with the seed. Using mt19937 generator
        mt19937 generator( seed );
        //Sanity check
        assert(vectorOfUnoccupiedNodes.size() != 0);
        //Shuflle the vector with the unoccupied node IDs.
        shuffle(vectorOfUnoccupiedNodes.begin(), ( vectorOfUnoccupiedNodes.end()), generator);
        //Sanity check
        assert(vectorOfUnoccupiedNodes.size() != 0);
        //Node count is maintained to split the vector of node IDs in 2 to assign half of the nodes to blue player and half to the red player.
        int nodeCount = 0;
        HexNode* currentNodePtr;
        //save starting move as the possible next move for this epoch.
        int possibleNextMoveNodeID = *vectorOfUnoccupiedNodes.begin();
        //Iterate through all nodes in vectorOfUnoccupiedNodes
        for (auto node : vectorOfUnoccupiedNodes)
        {
            //Get current node pointer
            currentNodePtr = originalHexGameGraphPtr->getNodePtr(node);
            if (nodeCount <= vectorOfUnoccupiedNodes.size()/2)
            {
                // Assign this node's state to the current player color.
                currentNodePtr->state = playerColor;
            }
            else
            {
                // Assign the other half of vectorOfUnoccupiedNodes to opposite player.
                if (playerColor == HexNodeState::BLUE)
                {
                    currentNodePtr->state = HexNodeState::RED;
                }
                else
                {
                    currentNodePtr->state = HexNodeState::BLUE;
                }
            }
            //Set the current nodes isAITest to true so we can reset all of these node for the original game board.
            currentNodePtr->isAITest = true;
            nodeCount++;
        }
        //Verify is this scenario (full board) is a Win scenario for the current player. Add the start node as a winning node in a map (mapOfNextMoveWinningNodes) with the node ID as the index.
        if (_isScenarioAWin(originalHexGameGraphPtr, playerColor))
        {
            //Add this node to map of possible wining moves nodes
            mapOfNextMoveWinningNodes[possibleNextMoveNodeID]++;
            assert(mapOfNextMoveWinningNodes.size() != 0);
        }
        //Reset all the tested nodes to unoccupied nodes
        for (auto node : vectorOfUnoccupiedNodes)
        {
            currentNodePtr = originalHexGameGraphPtr->getNodePtr(node);
            if (currentNodePtr->isAITest)
            {
                currentNodePtr->state = HexNodeState::UNOCCUPIED;
                currentNodePtr->isAITest = false;
            }
        }
        //Sanity check, set current node pointer to nullptr.
        currentNodePtr = nullptr;
        //cout << "Epoch: " << epoch << endl;
    }
    //Create a multimap so we can reverse the indexes of the mapOfNextMoveWinningNodes to enable sorting using the second parameter of the map (sort by value instead of key)
    multimap<int, int> tempMultiMap;
    //Sanity check
    assert(mapOfNextMoveWinningNodes.size() != 0);
    for (auto& iteratorPtr : mapOfNextMoveWinningNodes)
    {
        tempMultiMap.insert({iteratorPtr.second, iteratorPtr.first});
    }
    //End the timer to gauge the AI's performance.
    end = std::chrono::system_clock::now();
    //Calculate the duration and count the seconds in this duration.
    std::chrono::duration<double> elapsedSeconds = end - start;
    cout << "Time AI to choose a coordinate: " << elapsedSeconds.count() << "sec." << endl;
    //Because we sorted the multimap by value, the node that scored a win the most time will be stored in the first node of the tree used by multimap. In other words return the second parameter of the item at the begining of the multimap which is now an interger the Node ID.
    return originalHexGameGraphPtr->getNodePtr(tempMultiMap.rbegin()->second);
}

bool AIPlayer::_isScenarioAWin(HexGameGraph* originalHexGameGraphPtr, HexNodeState playerColor)
{
    //Check from current move if there is a path to other side of the board.
    int columnIndex = 0;
    int rowIndex = 0;
    int opositeRowIndex = gameSize-1;
    int opositeColumnIndex = gameSize-1;
    //RED PLAYER Check from nodePtr to any Northern most squares (row 0) and if there is a path check from nodePtr to any South most squares (row gameSize-1), BLUE PLAYER Check from nodePtr to any Eastern most square (column 0) to Western most square (column gameSize-1) ;
    //Get minimum spanning tree from game square (0,0). Here it does not matter if player is RED or BLUE, each share that starting point.
    MST<HexNode>* mst = nullptr; //= originalHexGameGraphPtr->getMinimumSpanningTreePRIM(0, playerColor);
    
    //Find if MST includes any of first boarder side squares
    for (int index1=0; index1 < gameSize; index1++)
    {
        // If the current player is RED, then set the rowIndex to increment or set the columIndex to increment if its the BLUE player.
        playerColor == HexNodeState::RED ? (columnIndex = index1) : (rowIndex = index1);
        
        //Update the minimum spaning tree with new cordinates to test. NorternMost Square and a SouthernMost node for BLUE PLAYER and EastMost and WestMost for RED PLAYER;
        mst = originalHexGameGraphPtr->getMinimumSpanningTreePRIM(GetNodeIDFromCoordinates(rowIndex, columnIndex, gameSize), playerColor);
        
        //Check sqaures to East or North boarders if the square exist in the MST if so check that the tree contains also a node to the oposite end.
        if (mst != nullptr && mst->nodeExist(GetNodeIDFromCoordinates(rowIndex, columnIndex, gameSize)) && originalHexGameGraphPtr->getNodePtr(GetNodeIDFromCoordinates(rowIndex, columnIndex, gameSize))->state == playerColor)
        {
            //Find if MST also includes any of the oposite side squares by iterating through possible squares.
            for (int index2=0; index2 < gameSize; index2++)
            {
                //Set opositeRowIndex to increment if color of player is RED or set opositeColumnIndex to increment if the player color is BLUE. (Vice-versa)
                playerColor == HexNodeState::RED ? (opositeColumnIndex = index2) : (opositeRowIndex = index2);
                //Only If we have an MST from current square move to both side of the boarders we return true. Here we return early to reduce the work load since we only need to know if we connect both sides of the game board.
                if(mst != nullptr && mst->nodeExist(GetNodeIDFromCoordinates(opositeRowIndex, opositeColumnIndex, gameSize)) && originalHexGameGraphPtr->getNodePtr(GetNodeIDFromCoordinates(opositeRowIndex, opositeColumnIndex, gameSize))->state == playerColor)
                {
                    //Delete the minimum spanning tree since we no longer need it and reclaim it's resources.
                    delete mst;
                    //Return true to caller
                    return true;
                    
                }
            }
        }

    }
    //If we get to this point, the minimum spanning tree does not include a square (of the current player color) at both the opposite ends of the board.
    if (mst != nullptr)
    {
        //Return MST resources to the heap.
        delete mst;
    }
    return false;
}

AIPlayer::~AIPlayer()
{
    //Default deconstructor
    //No heap resources to return to the heap.
}
