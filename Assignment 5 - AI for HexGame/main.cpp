//
//  main.cpp
//  CPP_testing
//
//  Created then Modified by Patrick Miron on 2022-02-07.
//
// NOTE TO THE READER : The Node,Edge,Graph,PriorityQueue,and MST classes were created in previous assignments, these were modified to become template class so they can be used just like a vector<Type> class. The new typename used is NodeType and is use like so Node<NodeType> or Edge<NodeType> ect. Please node that this task was not required but I wanted to learn generic template programing and polymorphism.
//  The game of hex was implemented in HexGame.hpp and is instantiated here in the main file.
//  A HexNode superclass was made from the Node<HexNode> class and a superclass HexGameGraph from the Graph<HexNode> base type was create to override functions of their base classes or to add data variables to support the game.
//  In this text file all files are merged into one to enable us to put up all program in the assignment text file as required. See bellow.
// As per required, the game hex program :
//   1-AIPlayer generates random moves on the game board using Monte Carlo based.
//   2-A human player can play against this program.
//   3-Prevent human player getting board by waiting too long for the AI to make a move. Here I was able to achive an average of 1 sec wait time on a 20 by 20 board with 1000 epoch/trials. These trials are counted as win for the first initial move of the epoch. The AI then returns the moves of the trials that yielded the most wins.
//   4-The player that chooses Red goes first.
//   5-The program will take a move from a human or AI and will display and determine if the current play results a winner and annonces it.
//   6-The program checks for legal moves and asks for another choice if its not.
//   7-As a bonus more advance solution, it was possible to make the AI evaluate using both Monte Carlo and Alpha/Beta pruning alogithm. Unfortunely time was to short. Please see example of wining board from player 2 who was RED at the end of this file. The game was 20 x 20 squares between 2 AIs who took on average less than 1 minute to make a choice with 1000 epochs.
//   8-<random> library was used.

#include <iostream>
#include <stdio.h>
#include "HexGame.hpp"

using namespace std;

void makeGame(int gameSize)
{
    HexGame newHexGame(gameSize); // The HexGame.cpp contains all code to start, play and end a game. To be portable and modifiable and could have multiple implementations using the templates.
}

int main(int argc, const char * argv[]) {
    //Set minimum legal size of board as default.
    int gameSize = 5;
    cout << endl;
    //PRINT TITLE
    cout << "Welcome to a game of Hex programed by Patrick Miron for the C++ class from professor Ira Pohl, Febuary 2022." << endl;
    //PRINT INSTRUCTION
    cout << "Please enter the size of game you wish to play (Minimum game size => 5): " << endl;
    //Get user chosen game board size.
    cin >> gameSize;
    cout << endl;
    
    //Create a game.
    makeGame(gameSize);
    
    
    return 0;
}
