//
//  HexGameHelperFunctions.hpp
//  CPP_testing
//
//  Created by Patrick Miron on 2022-02-25.
//

#ifndef HexGameHelperFunctions_hpp
#define HexGameHelperFunctions_hpp

#include <iostream>
#include <iomanip>
#include <tuple>
#include "HexGameGraph.hpp"

//Global helper function to get cordinates from nodeID and gameSize. This function is in HexGameGraph.hpp since it can only be used by class that include this header file
inline std::tuple<int, int> GetCoordinatesFromNodeID(int nodeID, int gameSize)
{
    return std::make_tuple(nodeID/gameSize, nodeID%gameSize);
}

//Global Helper function to get the nodeID from square coodinates on the game board and gameSize. This function is in HexGameGraph.hpp since it can only be used by class that include this header file
inline int GetNodeIDFromCoordinates(int row,int col, int gameSize)
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
        std::cout << "ERROR - ROW or COL OUTSIDE OF RANGE in _getNodePtrFromCoordinates" << std::endl;
    }
    return -1;
}

//Global Helper function that print's the board game to the console window.
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
inline void PrintGameBoard(HexGameGraph* hexGameGraphPtr, int gameSize)
{
    cout << endl;
    int currentNodeID = 0;
    int realRowCount = 0;
    
    //Print the row title
    cout << setw(gameSize*2);
    cout << "RED" << endl;
    cout << endl;
    
    //Label columns
    cout << "   ";
    for (int columnIndex = 0; columnIndex < gameSize; columnIndex++)
    {
        if (columnIndex < 10)
        {
            cout << columnIndex << setw(3);
            cout << " ";
        }
        else
        {
            cout << columnIndex << setw(2);
            cout << " ";
        }
    }
    cout << endl;
    //loop through all the rows in the game board
    for (int rowIndex = 0; rowIndex<gameSize ; rowIndex++)
    {
        if (realRowCount == (gameSize-2))
        {
            cout << "B ";
        }
        else if (realRowCount == (gameSize-1))
        {
            cout << "L ";
        }
        else if (realRowCount == (gameSize))
        {
            cout << "U ";
        }
        else if (realRowCount == (gameSize+1))
        {
            cout << "E ";
        }
        else
        {
            cout << "  ";
        }
        //Indent the row by the number of the row count + 1 space
        cout << right << setw(realRowCount+1);
        //Increment the row count. This could be done at the end of the row but no other code bellow uses realRowCount.
        cout << rowIndex << " ";

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
            switch (hexGameGraphPtr->getNodePtr(currentNodeID)->state ) {
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
                if (realRowCount == (gameSize-1))
                {
                    cout << "    B";
                }
                else if (realRowCount == (gameSize))
                {
                    cout << "   L";
                }
                else if (realRowCount == (gameSize+1))
                {
                    cout << "  U";
                }
                else if (realRowCount == (gameSize+2))
                {
                    cout << " E";
                }
                cout << endl;
            }
            //Increment the currentNodeID (acts like a node count)
            currentNodeID++;
        }
        
        
        if (realRowCount == (gameSize-2))
        {
            cout << "B ";
        }
        else if (realRowCount == (gameSize-1))
        {
            cout << "L ";
        }
        else if (realRowCount == (gameSize))
        {
            cout << "U ";
        }
        else if (realRowCount == (gameSize+1))
        {
            cout << "E ";
        }
        else
        {
            cout << "  ";
        }
        cout << right << setw(realRowCount+5);
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
        if (realRowCount == (gameSize-1))
        {
            cout << "     B";
        }
        else if (realRowCount == (gameSize))
        {
            cout << "    L";
        }
        else if (realRowCount == (gameSize+1))
        {
            cout << "   U";
        }
        else if (realRowCount == (gameSize+2))
        {
            cout << "  E";
        }
        else
        {
            cout << "  ";
        }
        
        cout << endl;
    }
    //Print bottom title
    cout << right << setw(gameSize*2+realRowCount) << "RED" << endl;
    cout << endl;
}


#endif /* HexGameHelperFunctions_hpp */
