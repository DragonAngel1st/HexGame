//
//  HexGame.cpp
//  Assignment 5 - AI for HexGame
//
//  Created by Patrick Miron on 2022-02-16.
//

#include <stdio.h>
#include "HexGame.hpp"
//#include "AIPlayer.hpp"

using namespace std;

//IMPLEMENTATION
//Default constructor
inline HexGame::HexGame(int gameSize):isActive(true),gameSize(gameSize)
{
    //Start a game
    //Implement game using a graph from assignment 3
    //All graph classes from assignment 3 have been transform to template classes.
    //_gameGraphPtr = new HexGameGraph(gameSize);

    HexGameGraph* _gameGraphPtr = new HexGameGraph(gameSize);
    
    //Create a new unordered map to hold the game squares coordinates to NodeID of size gameSize*gameSize.
    //Create all nodes in the graph and add their coordinates to the unordered map.
    //For example, for a 6x6 graph, this will create 36 nodes from node 0 to node 35.
    int index = 0;
    for (int rowIndex = 0;rowIndex<gameSize;rowIndex++)
    {
        for (int columnIndex=0;columnIndex<gameSize;columnIndex++)
        {
            _gameGraphPtr->addNode(index);
            HexNode * ptrToNode = _gameGraphPtr->getNodePtr(index);
            
            ptrToNode->state = HexNodeState::UNOCCUPIED;
            index++;
        }
    }
    //Node pointers that will be reused to create edges between them to create game board.
    HexNode* fromNodePtr = _gameGraphPtr->getNodePtr(0);
    HexNode* toNodePtr = _gameGraphPtr->getNodePtr(1);
    //Add nodes to graphs for specified gameSize
    // each square needs to add an edge to the square to right diagonally up, straigth right and right diagonally down to each other. The other side will be taken care by previous node.
    for (int rowIndex=0;rowIndex<gameSize;rowIndex++)
    {
        for (int columnIndex=0;columnIndex<gameSize;columnIndex++)
        {
            //Create top row
            if (rowIndex==0)
            {
                if (columnIndex==0)
                {
                    //Create a game square with 2 edges (top left corner)
                    //EDGE 1
                    //cout << _getNodeIDFromCoordinates(rowIndex,columnIndex) << endl;
                    fromNodePtr = _gameGraphPtr->getNodePtr(_getNodeIDFromCoordinates(rowIndex,columnIndex)) ;
                    //cout << _getNodeIDFromCoordinates(rowIndex,columnIndex+1) << endl;
                    toNodePtr = _gameGraphPtr->getNodePtr(_getNodeIDFromCoordinates(rowIndex, columnIndex+1));
                    _gameGraphPtr->addUndirectedEdge(*fromNodePtr, *toNodePtr, 1.0);
                    
                    //EDGE 2
                    //cout << _getNodeIDFromCoordinates(rowIndex+1,columnIndex) << endl;
                    toNodePtr = _gameGraphPtr->getNodePtr(_getNodeIDFromCoordinates(rowIndex+1, columnIndex)) ;
                    _gameGraphPtr->addUndirectedEdge(*fromNodePtr, *toNodePtr, 1.0);
                    
                }
                else if (columnIndex==gameSize-1)
                {
                    //create a game square with 3 edges (top right corner), 2 edge are already created by previous nodes.
                    //EDGE 1
                    fromNodePtr = _gameGraphPtr->getNodePtr(_getNodeIDFromCoordinates(rowIndex,columnIndex));
                    toNodePtr = _gameGraphPtr->getNodePtr(_getNodeIDFromCoordinates(rowIndex+1, columnIndex));
                    _gameGraphPtr->addUndirectedEdge(*fromNodePtr, *toNodePtr, 1.0);
                    
                }
                else
                {
                    //create a game square with 4 edges. (first row of squares without corners), 3 edge are already created by previous nodes
                    //EDGE 1
                    fromNodePtr = _gameGraphPtr->getNodePtr(_getNodeIDFromCoordinates(rowIndex,columnIndex));
                    toNodePtr = _gameGraphPtr->getNodePtr(_getNodeIDFromCoordinates(rowIndex, columnIndex+1));
                    _gameGraphPtr->addUndirectedEdge(*fromNodePtr, *toNodePtr, 1.0);
                    //EDGE 2
                    toNodePtr = _gameGraphPtr->getNodePtr(_getNodeIDFromCoordinates(rowIndex+1, columnIndex));
                    _gameGraphPtr->addUndirectedEdge(*fromNodePtr, *toNodePtr, 1.0);
                }
            }
            //Create bottom row
            else if (rowIndex==gameSize-1)
            {
                if (columnIndex==0)
                {
                    //Create a game square with 3 edges (bottom left corner), 1 edge already created by previous node
                    //EDGE 1
                    fromNodePtr = _gameGraphPtr->getNodePtr(_getNodeIDFromCoordinates(rowIndex,columnIndex));
                    toNodePtr = _gameGraphPtr->getNodePtr(_getNodeIDFromCoordinates(rowIndex-1, columnIndex+1));
                    _gameGraphPtr->addUndirectedEdge(*fromNodePtr, *toNodePtr, 1.0);
                    //EDGE 2
                    toNodePtr = _gameGraphPtr->getNodePtr(_getNodeIDFromCoordinates(rowIndex, columnIndex+1));
                    _gameGraphPtr->addUndirectedEdge(*fromNodePtr, *toNodePtr, 1.0);
                }
                else if (columnIndex==gameSize-1)
                {
                    //create a game square with 2 edges (bottom right corner)
                    //All edges have been created by previous nodes.
                }
                else
                {
                    //create a game square with 4 edges. (bottom row of squares without corners), 2 edges have already been created by previous nodes.
                    //EDGE 1
                    fromNodePtr = _gameGraphPtr->getNodePtr(_getNodeIDFromCoordinates(rowIndex,columnIndex));
                    toNodePtr = _gameGraphPtr->getNodePtr(_getNodeIDFromCoordinates(rowIndex-1, columnIndex+1));
                    _gameGraphPtr->addUndirectedEdge(*fromNodePtr, *toNodePtr, 1.0);
                    //EDGE 2
                    toNodePtr = _gameGraphPtr->getNodePtr(_getNodeIDFromCoordinates(rowIndex, columnIndex+1));
                    _gameGraphPtr->addUndirectedEdge(*fromNodePtr, *toNodePtr, 1.0);
                }
            }
            //Create middle rows
            else
            {
                //Create end of row game square with 4 edges,3 already have been created by previous nodes.
                if (columnIndex==gameSize-1)
                {
                    //EDGE 1
                    fromNodePtr = _gameGraphPtr->getNodePtr(_getNodeIDFromCoordinates(rowIndex,columnIndex));
                    toNodePtr = _gameGraphPtr->getNodePtr(_getNodeIDFromCoordinates(rowIndex+1, columnIndex));
                    _gameGraphPtr->addUndirectedEdge(*fromNodePtr, *toNodePtr, 1.0);
                }
                //Create all other squares that have 6 edges, 3 edges already been created by previous nodes.
                else
                {
                    //EDGE 1
                    fromNodePtr = _gameGraphPtr->getNodePtr(_getNodeIDFromCoordinates(rowIndex,columnIndex));
                    toNodePtr = _gameGraphPtr->getNodePtr(_getNodeIDFromCoordinates(rowIndex-1, columnIndex+1));
                    _gameGraphPtr->addUndirectedEdge(*fromNodePtr, *toNodePtr, 1.0);
                    //EDGE 2
                    toNodePtr = _gameGraphPtr->getNodePtr(_getNodeIDFromCoordinates(rowIndex, columnIndex+1));
                    _gameGraphPtr->addUndirectedEdge(*fromNodePtr, *toNodePtr, 1.0);
                    //EDGE 3
                    toNodePtr = _gameGraphPtr->getNodePtr(_getNodeIDFromCoordinates(rowIndex+1, columnIndex));
                    _gameGraphPtr->addUndirectedEdge(*fromNodePtr, *toNodePtr, 1.0);
                }
            }
        }
    }
    //Clean up temporary pointers and start game loop.
    fromNodePtr = nullptr;
    toNodePtr = nullptr;
    delete fromNodePtr;
    delete toNodePtr;
    _startGameLoop();
}

//Game Loop
inline void HexGame::_startGameLoop()
{
    
    bool gameOver = false;
    //PRINT HEX BOARD
    _printGameBoard();
    int numberHumanPlayers;
    cout << "Please enter \"1\" if you want to play single player or \"2\" for a hot seat two player game of Hex. :";
    cin >> numberHumanPlayers;
    //Loop while the game is not over.
    
    //AIPlayer* aiPlayer1 = new AIPlayer(HexNodeState::RED);
    
    while (!gameOver)
    {
        //FIXME: Ask player 1's color here
        
        //Ask first player move
        HexNode * nodePtr = _askPlayerForCoordinates(1);
        
        //FIXME: update the current player pointer.
        nodePtr->state = HexNodeState::RED;
        
        _printGameBoard();
        
        if (numberHumanPlayers != 0 && _checkIfPlayerHasWon(1, nodePtr))
        {
            _playerHasWon(1);
            gameOver = true;
            break;
        }
        //If playing hot seat 2 player game, ask 2nd player to enter move
        if (numberHumanPlayers == 2)
        {
            nodePtr = _askPlayerForCoordinates(2);
            nodePtr->state = HexNodeState::BLUE;
        }
        _printGameBoard();
        //Here player 2 is either human or the computer, so we only check once.
        if (_checkIfPlayerHasWon(2, nodePtr))
        {
            _playerHasWon(2);
            gameOver = true;
            break;
        }
    }
}

//Helper function to check if a player has won after each move. Since a player can only win on his last move, we don't have to check if there's a route without this last move.
inline bool HexGame::_checkIfPlayerHasWon(int playerID, HexNode * nodePtr)
{
    //Check from current move if there is a path to other side of the board.
    if (playerID == 1)
    {
        //RED PLAYER Check from nodePtr to any Northern most squares (row 0) and if there is a path check from nodePtr to any South most squares (row gameSize-1) ;
        
        //Get minimum spaning tree from last square entered and check that it has both a node NorternMost Square and a SouthernMost node;
        MST<HexNode>* mst = _gameGraphPtr->getMinimumSpanningTreePRIM(nodePtr->nodeID, nodePtr->state);
        //Find if MST includes any of NorthernMost squares
        for (int columnIndex=0; columnIndex < gameSize; columnIndex++)
        {
            //We check to see if there is a minimum spanning tree from current move to any boarder square at top and if so we then check if there is a minimum spanning tree from current move to the bottom board squares.
            if (mst->nodeExist(_getNodeIDFromCoordinates(0, columnIndex)))
            {
                //A MST exist to a top board square, now test for a MST to bottom boarder squares
                for (int columnIndex=0; columnIndex < gameSize; columnIndex++)
                {
                    //We only return true if there is also a MST from current square played to top and botom boarders.
                    if(mst->nodeExist(_getNodeIDFromCoordinates(gameSize-1, columnIndex)))
                    {
                        //We could get the shortestPath algorithm to return the wining path and highlight all the winning squares on the board here.
                        return true;
                        
                    }
                }
            }
        }
    }
    //Here the test for player 2 is the same as player 1 except that we check for an MST to East boarder and West boarder
    else
    {
        //BLUE PLAYER Check from nodePtr to any Northern most squares (row 0) and if there is a path check from nodePtr to any South most squares (row gameSize-1) ;
        //Get minimum spaning tree from last square entered and check that it has both a node NorternMost Square and a SouthernMost node;
        MST<HexNode>* mst = _gameGraphPtr->getMinimumSpanningTreePRIM(nodePtr->nodeID, nodePtr->state);
        //Find if MST includes any of NorthernMost squares
        for (int rowIndex=0; rowIndex < gameSize; rowIndex++)
        {
            //Check form current square played to East boarder squares
            if (mst->nodeExist(_getNodeIDFromCoordinates(rowIndex, 0)))
            {
                for (int rowIndex=0; rowIndex < gameSize; rowIndex++)
                {
                    //Only If we have an MST from current square move to both side of the boarders we return true.
                    if(mst->nodeExist(_getNodeIDFromCoordinates(rowIndex, gameSize-1)))
                    {
                        //We could get the shortestPath algorithm to return the wining path and highlight all the winning squares on the board here.
                        return true;
                        
                    }
                }
            }
        }
    }
    return false;
}

//Indicate to the console which player has wone.
inline void HexGame::_playerHasWon(int playerID)
{
    cout << "Congratulations, Player " << playerID << " you have won the game!" << endl;
    cout << "Good luck next time, Player "  << playerID << " !" << endl;
}

//Helper function to ask the current player the coordinates they want to move a token to.
inline HexNode * HexGame::_askPlayerForCoordinates(int playerID)
{
    //Initialize row and column to -1 because 0 in this case would be a valid choice
    int rowIndex=-1, columnIndex=-1;
    //Initialize bool variable a default values.
    bool coordinatesAreValid = false;
    bool gameSquareOccupied = false;
    //Initialize the color string variable to "None". We will never see the default value on the screen, this is just for arbitrary debugging.
    string playerColor("None");
    //player 1 is always Red and player 2 is alway Blue.
    if (playerID == 1)
    {
        playerColor = "RED";
    }
    else
    {
        playerColor = "BLUE";
    }
    cout << endl;
    //Prompt user for info.
    cout << "Player " << playerID << " : " << "(" << playerColor << ")" << endl;
    cout << " Please enter coordinates for your next move." << endl;
    cout << "row #: ";
    cin >> rowIndex;
    cout << endl;
    cout << "column #: ";
    cin >> columnIndex;
    cout << "Verifying if (" << rowIndex << ", " << columnIndex << ") is a valid location." << endl;
    //Verify if coordinates are valid and if the game square is occupied.
    coordinatesAreValid = _verifyCoordinates(rowIndex, columnIndex);
    gameSquareOccupied = _isGameSquareOccupied(rowIndex, columnIndex);
    //We enter the while loop only when the choice of game square is invalid or occupied.
    //We keep asking for valid coordinates until they are and that the square is not occupied.
    while (!coordinatesAreValid || (gameSquareOccupied))
    {
        //Prompt user for valid cordinates and help them out a bit more depending on validity
        if (!coordinatesAreValid)
        {
            cout << "You did not enter valid coordinates for you next move. Please enter a row integer then press Enter, then enter a column integer and press Enter again. No other format or characters are accepted." << endl;
        }
        else if (gameSquareOccupied)
        {
            cout << "This is an illegal move. There is already a token at that location. Please enter a row integer then press Enter, then enter a column integer and press Enter again. No other format or characters are accepted." << endl;
        }
        cout << "row #: ";
        cin >> rowIndex;
        cout << endl;
        cout << "column #: ";
        cin >> columnIndex;
        cout << "Verifying if (" << rowIndex << ", " << columnIndex << ") is a valid location." << endl;
        //Verification of valid coordinates inside the while loop.
        coordinatesAreValid = _verifyCoordinates(rowIndex, columnIndex);
        gameSquareOccupied = _isGameSquareOccupied(rowIndex, columnIndex);
    }
    //If we get to this point, we have a free and valid square, return this node.
    return _gameGraphPtr->getNodePtr(_getNodeIDFromCoordinates(rowIndex, columnIndex));
    
}

//Helper function to verify if the coordinates chosen are valid. Return true if valid.
inline bool HexGame::_verifyCoordinates(int rowIndex, int columnIndex)
{
    if (rowIndex >= 0 && rowIndex < gameSize && columnIndex >= 0 && columnIndex < gameSize)
    {
        return true;
    }
    return false;
}

//Helper function to check if the chosen game square is occupied or not.
inline bool HexGame::_isGameSquareOccupied(int rowIndex, int columnIndex)
{
    //HexNode * nodePtr = (HexNode *)(_gameGraphPtr->getNodePtr(_getNodeIDFromCoordinates(rowIndex, columnIndex)));
    if ( static_cast<HexNode*>(_gameGraphPtr->getNodePtr(_getNodeIDFromCoordinates(rowIndex, columnIndex)))->state == HexNodeState::UNOCCUPIED)
    {
        return false;
    }
    return true;
}

//Helper function to get the nodeID from square coodinates on the game board.
inline int HexGame::_getNodeIDFromCoordinates(int row,int col)
{
    //We make sure that the row and column are valid
    if (col != gameSize && row != gameSize)
    {
        //Return the row number multiplied by the game board size plus the column number.
        //THis is base on the fact that columns and rows go from 0 to gameSize-1.
        return (row*gameSize+col);
    }
    else
    {
        cout << "ERROR - ROW or COL OUTSIDE OF RANGE in _getNodePtrFromCoordinates" << endl;
    }
    return -1;
}
inline tuple<int, int> HexGame::_getCoordinatesFromNodeID(int nodeID)
{
    return make_tuple(nodeID/gameSize, nodeID%gameSize);
}

//Helper function that print's the board game to the console window.
/*
 Example 5x5 board:
 . - . - . - . - .
 \ / \ / \ / \ / \
 . - . - . - . - .
 \ / \ / \ / \ / \
 . - . - . - . - .
 \ / \ / \ / \ / \
 . - . - . - . - .
 \ / \ / \ / \ / \
 . - . - . - . - .
 */
inline void HexGame::_printGameBoard()
{
    cout << endl;
    int currentNodeID = 0;
    int realRowCount = 0;
    //loop through all the rows in the game board
    for (int rowIndex = 0; rowIndex<gameSize ; rowIndex++)
    {
        //Indent the row by the number of the row count + 1 space
        cout << right << setw(realRowCount+1);
        //Increment the row count. This could be done at the end of the row but no other code bellow uses realRowCount.
        realRowCount++;
        //Loop through all columns in the game board.
        for (int columnIndex=0 ; columnIndex<gameSize*1 ; columnIndex++)
        {
            // Assertion test to make sure we do not print more nodes then they are squares in the game.
            // This type of assertion does not crash the game
            if (currentNodeID >= gameSize*gameSize)
            {
                break;
            }
            // Determine the current node type, A node can be "." for unoccupied nodes, B for Blue occupied nodes and R for Red occupied nodes.
            switch ( (static_cast<HexNode*>(_gameGraphPtr->getNodePtr(currentNodeID)))->state ) {
                case HexNodeState::BLUE:
                    cout << "B";
                    break;
                case HexNodeState::RED:
                    cout << "R";
                    break;
                case HexNodeState::UNOCCUPIED:
                    cout << ".";
                    break;
                default:
                    cout << "ERROR";
            }
            //Space after each character.
            cout << " ";
            //If we are at a node that has a following node, print a "-".
            if (columnIndex < gameSize-1)
            {
                cout << "-";
            }
            //Space after each character.
            cout << " ";
            //If we are at the end of the node make a carriage return.
            if (currentNodeID % gameSize == gameSize-1)
            {
                cout << endl;
            }
            //Increment the currentNodeID (acts like a node count)
            currentNodeID++;
        }
        
        cout << right << setw(realRowCount+3);
        realRowCount++;
        //For every pair lines only, print the sequence of edges from above-left "\" and above-right node "/".
        //Since we are still in the same iteration for the row and we printed a whole row in the first column loop, we can go ahead and fill a whole row for edges only except if we are passed the final row of nodes
        if (rowIndex < gameSize-1){
            //Loop through all columns
            for (int columnIndex=0 ; columnIndex<gameSize*1 ; columnIndex++)
            {
                //If we are not at the end of the row print "\" and "/" with one space in between.
                if (columnIndex < gameSize-1)
                {
                    //Note, we must use a escaping \ to print the "\".
                    cout << "\\ /";
                }
                else
                {
                    //If we are at the last column, do not print the "/" character, only the "\"
                    cout << "\\";
                }
                //Space after each character.
                cout << " ";
            }
            
        }
        cout << endl;
    }
}

//Deconstructor for the HexGame object to make sure
inline HexGame::~HexGame()
{
    //Delete the object pointed by this pointer
    delete _gameGraphPtr;
    //cout << "HexGame DECONSTRUCTOR CALLED" << endl;
    //Set game to isActive to false. isActive is mostly used for debugging.
    isActive = false;
}

