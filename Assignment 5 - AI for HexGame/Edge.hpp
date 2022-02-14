//
//  Edge.hpp
//  Part B - Assignment 4 - Hex Game
//
//  Created then Modified by Patrick Miron on 2022-01-24.
//

//Edge is a template class for the typename NodeType. This was modified so that the templated Graph/Node/Edge/etc classes can be modified later for each type of NodeType -> ex: Graph<NodeType>, in other words, the Node<NodeType> can be subclassed in a new NodeType that has extra data. This is to recreate the functionallity like a vector<Type> from the standard template library.

#ifndef Edge_hpp
#define Edge_hpp

#include <stdio.h>
#include <iostream>

using namespace std;
//Bellow class is forwarded
template <typename NodeType>
class Node;

template <typename NodeType>
class Edge
{
public:
    Node<NodeType> * fromNodePtr; // starting node of edge.
    Node<NodeType> * toNodePtr; // ending node of edge
    int fromNodeID;
    int toNodeID;
    //--------------------   Added MST f*or assig*nment 4.
    //enum color for the color of the edge, a single color must be assign to an edge or none at all;
    double cost; // cost of the edge, ex: distance if the graph represents roads between towns.
    int indexOfDirectedEdgeInGraphVectorOfEdges; // The edge memoize it's location in the vector of edges in the graph. Allows for all operations in O(1)
    Edge();
    Edge(Node<NodeType> & fromNode, Node<NodeType> & toNode);
    Edge(const Edge& edge);
    Edge& operator=(const Edge& other);
    Edge(Edge<NodeType> &&other) noexcept;
    Edge& operator=(Edge &&other) noexcept;
//    void * operator new(size_t size);
//    void operator delete(void * memory, size_t size) noexcept;
    ~Edge();
};

//IMPLEMENTATION
template <typename NodeType>
Edge<NodeType>::Edge()
{
    //cout << "The Edge Default CONSTRUCTOR was called." << endl;
};

template <typename NodeType>
Edge<NodeType>::Edge(Node<NodeType> & _fromNode, Node<NodeType> & _toNode)//:fromNode(fromNode),toNode(toNode)
{
    fromNodePtr = &_fromNode;
    fromNodeID = fromNodePtr->nodeID;
    toNodePtr = &_toNode;
    toNodeID = toNodePtr->nodeID;
    //cout << "The Edge custom CONSTRUCTOR was called. From nodeID: " <<fromNodePtr->nodeID << " to nodeID: " << toNodePtr->nodeID << endl;
};

template <typename NodeType>
Edge<NodeType>::Edge(const Edge& edge)
{
    //This constructor should not be called since the implementation uses ptrs of nodes. Never directly copy a node! It can have too much data.
    std::cout << "The Edge Copy CONSTRUCTOR was called." << std::endl;
}

template <typename NodeType>
Edge<NodeType>& Edge<NodeType>::operator=(const Edge<NodeType>& other)
{
    //This assignment should not be called since the implementation uses ptrs of nodes. Never directly copy a node! It can have too much data.
    std::cout << "The Edge Assignment OPERATOR was called." << std::endl;
    return *this;
};

//Move Constructor
template <typename NodeType>
Edge<NodeType>::Edge(Edge<NodeType> &&other) noexcept
{
    cout << "The Edge Move CONSTRUCTOR was called." << endl;
}
//Move Assignment
template <typename NodeType>
Edge<NodeType>& Edge<NodeType>::operator=(Edge &&other) noexcept
{
    cout << "The Edge Move ASSIGNMENT was called." << endl;
    return *this;
}


template <typename NodeType>
Edge<NodeType>::~Edge()
{
    //cout << "The Edge DECONSTRUCTOR was called. From nodeID: " <<fromNodeID << " to nodeID: " << toNodeID << endl;
    fromNodePtr = nullptr;
    toNodePtr = nullptr;
    toNodeID = -1;
    fromNodeID = -1;
    indexOfDirectedEdgeInGraphVectorOfEdges = -1;
}





#endif /* Edge_hpp */
