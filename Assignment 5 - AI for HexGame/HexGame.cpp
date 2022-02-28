//
//  HexGame.cpp
//  Assignment 5 - AI for HexGame
//
//  Created by Patrick Miron on 2022-02-16.
//

#include <stdio.h>
#include "HexGame.hpp"
#include "HexGameHelperFunctions.hpp"



using namespace std;

//IMPLEMENTATION
//Default constructor
HexGame::HexGame(int gameSize):isActive(true),gameSize(gameSize)
{
    //Start a game
    //Implement game using a graph from assignment 3
    //All graph classes from assignment 3 have been transform to template classes.
    //_gameGraphPtr = new HexGameGraph(gameSize);

    _gameGraphPtr = new HexGameGraph(gameSize);
    
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
                    fromNodePtr = _gameGraphPtr->getNodePtr(GetNodeIDFromCoordinates(rowIndex,columnIndex,gameSize)) ;
                    //cout << _getNodeIDFromCoordinates(rowIndex,columnIndex+1) << endl;
                    toNodePtr = _gameGraphPtr->getNodePtr(GetNodeIDFromCoordinates(rowIndex, columnIndex+1,gameSize));
                    _gameGraphPtr->addUndirectedEdge(*fromNodePtr, *toNodePtr, 1.0);
                    
                    //EDGE 2
                    //cout << _getNodeIDFromCoordinates(rowIndex+1,columnIndex) << endl;
                    toNodePtr = _gameGraphPtr->getNodePtr(GetNodeIDFromCoordinates(rowIndex+1, columnIndex,gameSize)) ;
                    _gameGraphPtr->addUndirectedEdge(*fromNodePtr, *toNodePtr, 1.0);
                    
                }
                else if (columnIndex==gameSize-1)
                {
                    //create a game square with 3 edges (top right corner), 2 edge are already created by previous nodes.
                    //EDGE 1
                    fromNodePtr = _gameGraphPtr->getNodePtr(GetNodeIDFromCoordinates(rowIndex,columnIndex,gameSize));
                    toNodePtr = _gameGraphPtr->getNodePtr(GetNodeIDFromCoordinates(rowIndex+1, columnIndex,gameSize));
                    _gameGraphPtr->addUndirectedEdge(*fromNodePtr, *toNodePtr, 1.0);
                    
                }
                else
                {
                    //create a game square with 4 edges. (first row of squares without corners), 3 edge are already created by previous nodes
                    //EDGE 1
                    fromNodePtr = _gameGraphPtr->getNodePtr(GetNodeIDFromCoordinates(rowIndex,columnIndex,gameSize));
                    toNodePtr = _gameGraphPtr->getNodePtr(GetNodeIDFromCoordinates(rowIndex, columnIndex+1,gameSize));
                    _gameGraphPtr->addUndirectedEdge(*fromNodePtr, *toNodePtr, 1.0);
                    //EDGE 2
                    toNodePtr = _gameGraphPtr->getNodePtr(GetNodeIDFromCoordinates(rowIndex+1, columnIndex,gameSize));
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
                    fromNodePtr = _gameGraphPtr->getNodePtr(GetNodeIDFromCoordinates(rowIndex,columnIndex,gameSize));
                    toNodePtr = _gameGraphPtr->getNodePtr(GetNodeIDFromCoordinates(rowIndex-1, columnIndex+1,gameSize));
                    _gameGraphPtr->addUndirectedEdge(*fromNodePtr, *toNodePtr, 1.0);
                    //EDGE 2
                    toNodePtr = _gameGraphPtr->getNodePtr(GetNodeIDFromCoordinates(rowIndex, columnIndex+1, gameSize));
                    _gameGraphPtr->addUndirectedEdge(*fromNodePtr, *toNodePtr, 1.0);
                }
                else if (columnIndex == gameSize-1)
                {
                    //create a game square with 2 edges (bottom right corner)
                    //All edges have been created by previous nodes.
                }
                else
                {
                    //create a game square with 4 edges. (bottom row of squares without corners), 2 edges have already been created by previous nodes.
                    //EDGE 1
                    fromNodePtr = _gameGraphPtr->getNodePtr(GetNodeIDFromCoordinates(rowIndex,columnIndex, gameSize));
                    toNodePtr = _gameGraphPtr->getNodePtr(GetNodeIDFromCoordinates(rowIndex-1, columnIndex+1, gameSize));
                    _gameGraphPtr->addUndirectedEdge(*fromNodePtr, *toNodePtr, 1.0);
                    //EDGE 2
                    toNodePtr = _gameGraphPtr->getNodePtr(GetNodeIDFromCoordinates(rowIndex, columnIndex+1, gameSize));
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
                    fromNodePtr = _gameGraphPtr->getNodePtr(GetNodeIDFromCoordinates(rowIndex,columnIndex,gameSize));
                    toNodePtr = _gameGraphPtr->getNodePtr(GetNodeIDFromCoordinates(rowIndex+1, columnIndex, gameSize));
                    _gameGraphPtr->addUndirectedEdge(*fromNodePtr, *toNodePtr, 1.0);
                }
                //Create all other squares that have 6 edges, 3 edges already been created by previous nodes.
                else
                {
                    //EDGE 1
                    fromNodePtr = _gameGraphPtr->getNodePtr(GetNodeIDFromCoordinates(rowIndex,columnIndex, gameSize));
                    toNodePtr = _gameGraphPtr->getNodePtr(GetNodeIDFromCoordinates(rowIndex-1, columnIndex+1, gameSize));
                    _gameGraphPtr->addUndirectedEdge(*fromNodePtr, *toNodePtr, 1.0);
                    //EDGE 2
                    toNodePtr = _gameGraphPtr->getNodePtr(GetNodeIDFromCoordinates(rowIndex, columnIndex+1, gameSize));
                    _gameGraphPtr->addUndirectedEdge(*fromNodePtr, *toNodePtr, 1.0);
                    //EDGE 3
                    toNodePtr = _gameGraphPtr->getNodePtr(GetNodeIDFromCoordinates(rowIndex+1, columnIndex, gameSize));
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
void HexGame::_startGameLoop()
{
    //Initialize the game and set gameOver to false.
    bool gameOver = false;
    //PRINT HEX BOARD
    _printGameBoard();
    
    int numberHumanPlayers;
    //Ask how many human players will play the game. You may choose from 0 to 2 human players. 2 Player is played "hot seat" fashion.
    cout << "Please enter \"1\" if you want to play single player or \"2\" for a hot seat two player game of Hex. :";
    cin >> numberHumanPlayers;
    cout << endl;
    //Continue asking user until we get a valid number of human players.
    while (numberHumanPlayers < 0 || numberHumanPlayers > 2 )
    {
        cout << "You did not enter a number between 1 and 2. The number must be an integer: ";
        cin >> numberHumanPlayers;
        cout << endl;
    }
    //EASTER EGG - KooKoo!
    if (numberHumanPlayers == 0) {
        cout << endl;
        cout << "Congratulations! You found my easter egg! Sit back and relax while artificial intelligent players compete against each other." << endl;
        cout << endl;
    }
    //Ask what color the player 1 wants.
    cout << "What color will player 1 choose? (B)lue or (R)ed? Remember, Red goes first." << endl;
    char player1ColorChoiceInput;
    cin >> player1ColorChoiceInput;
    cout << endl;
    //Set colorValide to false;
    bool colorValide = false;
    //While we don't have a valid color choice, request a valid color.
    while (colorValide == false)
    {
        switch (player1ColorChoiceInput) {
            case 'R': //Falls through
            case 'r':
            {
                //Set color of the players
                player1 = Player(1, HexNodeState::RED, ((numberHumanPlayers>=1) ? PlayerType::HUMAN : PlayerType::AI));
                player2 = Player(2, HexNodeState::BLUE, ((numberHumanPlayers>=2) ? PlayerType::HUMAN : PlayerType::AI));
                colorValide = true;
                
                break;
            }
            case 'B': //Falls through
            case 'b':
            {
                //Set color of the players
                player1 = Player(1, HexNodeState::BLUE, ((numberHumanPlayers>=1) ? PlayerType::HUMAN : PlayerType::AI));
                player2 = Player(2, HexNodeState::RED, ((numberHumanPlayers>=2) ? PlayerType::HUMAN : PlayerType::AI));
                colorValide = true;
                break;
            }
            default:
            {
                // wrong character input
                cout << "Character: " << player1ColorChoiceInput << " is not valid." << endl;
                cout << "Please choose 'B' for Blue and 'R' for Red: ";
                cin >> player1ColorChoiceInput;
                break;
            }
        }
    }
    //Indicate what each player will be.
    cout << "Player 1 will be " << HexNodeStateToString(player1.color) << " and is a " << PlayerTypeToString(player1.type)  << " player."<< endl;
    cout << "Player 2 will be " << HexNodeStateToString(player2.color) << " and is a " << PlayerTypeToString(player2.type) << " player." << endl;
    cout << endl;

    //Initialize only one AIPlayer
    //For a AI against AI game, the same AIPlayer can be used since it does not remember moves.
    int aiIterations = 0;
    if (player1.type == PlayerType::AI || player2.type == PlayerType::AI)
    {
        cout << "Please input number of possible boards that the AI player will try each turn. Warning! The more game boards and the bigger the game size the more the AI will take time to do it's turn. We recommend less than 5000 for a 20 by 20 game board with average AI turn time of 4.5sec. (100+): ";
        cin >> aiIterations;
        cout << endl;
        aiBrain = AIPlayer(gameSize, aiIterations);
    }
    //Set the current player to RED player.
    Player currentPlayer = (player1.color == HexNodeState::RED) ? player1 : player2;
    
    //Loop while the game is not over.
    while (!gameOver)
    {
        //Ask player 1 for a move
        HexNode * nodePtr = _askPlayerForCoordinates(currentPlayer);
        nodePtr->state = currentPlayer.color;
        //Print game grid
        _printGameBoard();
        //Check Player 1 won with this last move.
        if (_checkIfPlayerHasWonFromLastMove(currentPlayer, nodePtr))
        {
            //Message that the current player has won and end the game loop.
            _playerHasWon(currentPlayer);
            gameOver = true;
            break;
        }
        //Switch the current player to the next player. This is one of the best example of using the ternary operator "?" in c++.
        currentPlayer = (currentPlayer==player1) ? player2 : player1;
    }
}

//Helper function to check if a player has won after each move. Since a player can only win on his last move, we don't have to check if there's a route without this last move.
bool HexGame::_checkIfPlayerHasWonFromLastMove(Player currentPlayer, HexNode * nodePtr)
{

    //Check from current move if there is a path to other side of the board.
    int columnIndex = 0;
    int rowIndex = 0;
    int opositeRowIndex = gameSize-1;
    int opositeColumnIndex = gameSize-1;
    //RED PLAYER Check from nodePtr to any Northern most squares (row 0) and if there is a path check from nodePtr to any South most squares (row gameSize-1), BLUE PLAYER Check from nodePtr to any Eastern most square (column 0) to Western most square (column gameSize-1) ;
    //Get minimum spaning tree from last square entered and check that it has both a node NorternMost Square and a SouthernMost node for BLUE PLAYER and EastMost and WestMost for RED PLAYER;
    MST<HexNode>* mst = _gameGraphPtr->getMinimumSpanningTreePRIM(nodePtr->nodeID, currentPlayer.color);
    //Find if MST includes any of first boarder side squares
    for (int index1=0; index1 < gameSize; index1++)
    {
        // If the current player is RED, then set the rowIndex to increment or set the columIndex to increment if its the BLUE player.
        currentPlayer.color == HexNodeState::RED ? (columnIndex = index1) : (rowIndex = index1);
        //Check sqaures to East or North boarders if the square exist in the MST if so check that the tree contains also a node to the oposite end.
        if (mst->nodeExist(GetNodeIDFromCoordinates(rowIndex, columnIndex, gameSize)))
        {
            //Find if MST also includes any of the oposite side squares by iterating through possible squares.
            for (int index2=0; index2 < gameSize; index2++)
            {
                //Set opositeRowIndex to increment if color of player is RED or set opositeColumnIndex to increment if the player color is BLUE. (Vice-versa)
                currentPlayer.color == HexNodeState::RED ? ({opositeColumnIndex = index2;}) : ({opositeRowIndex = index2;});
                //Only If we have an MST from current square move to both side of the boarders we return true. Here we return early to reduce the work load since we only need to know if we connect both sides of the game board.
                if(mst->nodeExist(GetNodeIDFromCoordinates(opositeRowIndex, opositeColumnIndex, gameSize)) && _gameGraphPtr->getNodePtr(GetNodeIDFromCoordinates(opositeRowIndex, opositeColumnIndex, gameSize))->state == currentPlayer.color)
                {
                    //We could get the shortestPath algorithm to return the wining path and highlight all the winning squares on the board here. Warning!!! To change the command window colors seems to be something C++ still strugles for platform independency.
                    delete mst;
                    //Return true to caller
                    return true;
                    
                }
            }
        }
    }
    if (mst != nullptr)
    {
        delete mst;
    }
    return false;
}

//Indicate to the console which player has wone.
void HexGame::_playerHasWon(Player currentPlayer)
{
    cout << "Congratulations Player " << currentPlayer.ID << " you have won the game!" << endl;
    cout << "Good luck next time Player "  << ((currentPlayer == player1) ? player2 : player1).ID << "..." << endl;
}

//Helper function to ask the current player the coordinates they want to move a token to.
HexNode * HexGame::_askPlayerForCoordinates(Player currentPlayer)
{
    switch (currentPlayer.type) {
        case PlayerType::HUMAN :
        {
            //Initialize row and column to -1 because 0 in this case would be a valid choice
            int rowIndex=-1, columnIndex=-1;
            //Initialize bool variable a default values.
            bool coordinatesAreValid = false;
            bool gameSquareOccupied = false;
            //Initialize the color string variable to "None". We will never see the default value on the screen, this is just for arbitrary debugging.
            
            cout << endl;
            //Prompt user for info.
            cout << "Player " << currentPlayer.ID << " : " << "(" << HexNodeStateToString(currentPlayer.color) << ")" << endl;
            cout << " Please enter coordinates for your next move." << endl;
            cout << "row #: ";
            cin >> rowIndex;
            cout << endl;
            cout << "column #: ";
            cin >> columnIndex;
            cout << "Verifying if (" << rowIndex << ", " << columnIndex << ") is a valid location." << endl;
            //Verify if coordinates are valid and if the game square is occupied.
            coordinatesAreValid = _verifyCoordinates(rowIndex, columnIndex);
            if (coordinatesAreValid)
            {
                gameSquareOccupied = _isGameSquareOccupied(rowIndex, columnIndex);
            }
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
            return _gameGraphPtr->getNodePtr(GetNodeIDFromCoordinates(rowIndex, columnIndex, gameSize));
            break;
        }
        case PlayerType::AI :
        {
            //If AI player has not been instantiated, do this here.
            return aiBrain.getPossibleWinningMove(_gameGraphPtr, currentPlayer.color);
            break;
        }
        default:
            break;
    }


    
}

//Helper function to verify if the coordinates chosen are valid. Return true if valid.
bool HexGame::_verifyCoordinates(int rowIndex, int columnIndex)
{
    if (rowIndex >= 0 && rowIndex < gameSize && columnIndex >= 0 && columnIndex < gameSize)
    {
        return true;
    }
    return false;
}

//Helper function to check if the chosen game square is occupied or not.
bool HexGame::_isGameSquareOccupied(int rowIndex, int columnIndex)
{
    //HexNode * nodePtr = (HexNode *)(_gameGraphPtr->getNodePtr(_getNodeIDFromCoordinates(rowIndex, columnIndex)));
    if ( static_cast<HexNode*>(_gameGraphPtr->getNodePtr(GetNodeIDFromCoordinates(rowIndex, columnIndex, gameSize)))->state == HexNodeState::UNOCCUPIED)
    {
        return false;
    }
    return true;
}




//Helper function that calls the global function PrintGameBoard that prints the board game to the console window. See HexGameHelperFunctions.hpp file for more info.
void HexGame::_printGameBoard()
{
    PrintGameBoard(_gameGraphPtr, gameSize);
}

//Deconstructor for the HexGame object to make sure
HexGame::~HexGame()
{
    //Delete the object pointed by this pointer
    delete _gameGraphPtr;
    //cout << "HexGame DECONSTRUCTOR CALLED" << endl;
    //Set game to isActive to false. isActive is mostly used for debugging.
    isActive = false;
}

