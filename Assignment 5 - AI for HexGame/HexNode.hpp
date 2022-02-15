//
//  HexNode.hpp
//  Part B - Assignment 4 - Hex Game
//
//  Created then Modified by Patrick Miron on 2022-02-01.
//

//HexNode is a template class for the typename NodeType. This was modified so that Node can be modified later for each type of NodeType -> Node<NodeType>, in other words, the Node<NodeType> can be subclassed in a new NodeType that has extra data. This is to recreate the functionallity like a vector<Type> from the standard template library. Here it is used to add a variable named "state" in the Node<HexNode> see bellow.

#ifndef HexNode_hpp
#define HexNode_hpp

#include <stdio.h>
//If the object of the bellow class must be instantiated here in this template they must also have their file included here.
#include "Node.hpp"
#include "HexNodeState.hpp"

//Bellow class is forwarded
template<typename HexNode>
class Edge;

//Definition, Implementation, Super Class of Node<NodeType>
class HexNode: public Node<HexNode>
{
public:
    //Set the node state to default to Unoccupied.
    HexNodeState state = HexNodeState::UNOCCUPIED;
    //Set move score to 0. This variable is used by AI to evaluate best potential moves.
    int moveScore = 0;
    //Default constructor
    HexNode(int nodeName=0);
private:
    
};



//IMPLEMENTATION
HexNode::HexNode(int nodeName): Node<HexNode>{nodeName} {
    ptrToVectorDirectedEdges = new vector<Edge<HexNode>*>;
};

#endif /* HexNode_hpp */
