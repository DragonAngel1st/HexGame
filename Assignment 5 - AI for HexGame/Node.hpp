//
//  Node.hpp
//  Part B - Assignment 4 - Hex Game
//
//  Created then Modified by Patrick Miron on 2022-01-24.
//

//Node is a template class for the typename NodeType. This was modified so that Node can be modified later for each type of NodeType -> Node<NodeType>, in other words, the Node<NodeType> can be subclassed in a new NodeType that has extra data. This is to recreate the functionallity like a vector<Type> from the standard template library. Here it is used to add a variable named "state" in the Node<HexNode> see the HexNode.hpp class.

#ifndef Node_hpp
#define Node_hpp

#include <vector>
#include <cmath>
#include <iostream>
//#include "Edge.hpp"
using namespace std;

const double Infinity = INFINITY;

template <typename NodeType>
class Edge;

template <typename NodeType>
class Node
{

public:
    int nodeID; // I decided to use integers for nodeIDs but it is only for internal use. A table could be created to return letters instead.
    int indexOfNodeInGraphVectorOfNodes; // The node memoize it's current location in the graph's vector of edges.
    double shortestDistanceToCurrentNodeInPath = INFINITY; // All nodes are automatically instantiated with an Infinite value for shortest current path.
    int indexOfNodeInUnvistedQueue; // The node memoize it's current location in the unvisited queue. Allows for O(1) time for all operations.
    
    vector<NodeType*> * ptrToVectorOfNeighborPtrs; // A vector of node ptrs for each node is maintained on the node for existing neighboring nodes. Allows for O(1) time for all operations.
    vector<Edge<NodeType>*> * ptrToVectorDirectedEdges; // A vector of edge ptrs for each edge is maintained on the node for existing connected edges. Allows for O(1) time for all operations.

    Edge<NodeType> * ptrToEdgeWithShortestDistance;
    
    //Here the empty node constructor is not needed since the nodeID is default to 0 in the next constructor signature and the compiler will automatically use it for no parameter.
    //    Node():nodeID(0) //All nodes must be created with an ID, if using the empty parameter constructor, remember to update the node ID after.
    //    {
    //        // For evaluation purpouses, and program pathing, it's better to instantiate empty vector of neighbor nodes or edges instead of dealing with an null pointer.
    //        ptrToVectorDirectedEdges = new vector<Edge*>;
    //        ptrToVectorOfNeighborPtrs = new vector<Node*>;
    //    }

    // The self construction of the Graph is only using this node constructor.
    Node<NodeType>(int nodeName=0);
    
    //Copy CONSTRUCTOR
    Node<NodeType>(const NodeType &node);
    // For evaluating the order of the nodes. The node with the current shortest distance to it is the one that comes first.
//    void * operator new(size_t size);
//    void operator delete(void * memory, size_t size) noexcept;
    bool operator>(const NodeType& other) const;
    bool operator<(const NodeType& other) const;
    bool operator==(const NodeType& other) const;
    bool operator!=(const NodeType& other) const;
    static bool compareNodeID(NodeType * node, NodeType * other);
    static bool compareNodeCurrentShortestPath(NodeType * node, NodeType * other);
    NodeType& operator=(const NodeType& other);
    ~Node();
    Node(NodeType &&other) noexcept;
    NodeType& operator=(NodeType &&other) noexcept;
    // Friend function to be able to cout a node by it's ID. See the overiden ostream operator "<<" below.
    friend std::ostream& operator<<(std::ostream& stream, const Node<NodeType>& node);
};

//IMPLEMENTATION
//The self construction of the Graph is only using this node constructor.
template <typename NodeType>
inline Node<NodeType>::Node(int nodeName):nodeID(nodeName){
    // For evaluation purpouses, and program pathing, it's better to instantiate empty vector of neighbor nodes or edges instead of dealing with an null pointer.
    ptrToVectorDirectedEdges = new vector<Edge<NodeType>*>;
    ptrToVectorOfNeighborPtrs = new vector<NodeType*>;
    //cout << "The Node default CONSTRUCTOR was called. ID: " << nodeID << endl;
}

template <typename NodeType>
inline Node<NodeType>::Node(const NodeType &node)
{
    //This constructor should not be called since the implementation uses ptrs of nodes. Never directly copy a node! It can have too much data.
    cout << "The Node Copy CONSTRUCTOR was called." << endl;
}

template <typename NodeType>
inline NodeType& Node<NodeType>::operator=(const NodeType& other)
{
    //This assignment operator should not be called since the implementation uses ptrs of nodes. Never directly copy a node! It can have too much data.
    cout << "The Node Assignment OPERATOR was called." << endl;
    return *this;
};

//Move Constructor
template <typename NodeType>
inline Node<NodeType>::Node(NodeType &&other) noexcept
{
    cout << "The Node Move CONSTRUCTOR was called." << endl;
}
//Move Assignment
template <typename NodeType>
inline NodeType& Node<NodeType>::operator=(NodeType &&other) noexcept
{
    cout << "The Node Move ASSIGNMENT was called." << endl;
    return *this;
}

// For evaluating the order of the nodes. The node with the current shortest distance to it is the one that comes first.
template <typename NodeType>
inline bool Node<NodeType>::operator>(const NodeType& other) const
{
    return this->shortestDistanceToCurrentNodeInPath > other.shortestDistanceToCurrentNodeInPath;
};

template <typename NodeType>
inline bool Node<NodeType>::operator<(const NodeType& other) const
{
    return this->shortestDistanceToCurrentNodeInPath < other.shortestDistanceToCurrentNodeInPath;
};

template <typename NodeType>
inline bool Node<NodeType>::operator==(const NodeType& other) const
{
    return this->shortestDistanceToCurrentNodeInPath == other.shortestDistanceToCurrentNodeInPath;
}

template <typename NodeType>
inline bool Node<NodeType>::operator!=(const NodeType& other) const
{
    return this->shortestDistanceToCurrentNodeInPath != other.shortestDistanceToCurrentNodeInPath;
}

template <typename NodeType>
inline bool Node<NodeType>::compareNodeID(NodeType * node, NodeType * other)
{
    return (node->nodeID < other->nodeID);
}

template <typename NodeType>
inline bool Node<NodeType>::compareNodeCurrentShortestPath(NodeType * node, NodeType * other)
{
    return (node->shortestDistanceToCurrentNodeInPath < other->shortestDistanceToCurrentNodeInPath);
}

template <typename NodeType>
inline Node<NodeType>::~Node()
{
    //cout << "The Node DECONSTRUCTOR was called. ID: " << nodeID << endl;
    this->ptrToEdgeWithShortestDistance = nullptr;
    this->ptrToVectorOfNeighborPtrs->clear();
    this->ptrToVectorDirectedEdges->clear();
    //delete this->ptrToEdgeWithShortestDistance;
    delete this->ptrToVectorOfNeighborPtrs;
    delete this->ptrToVectorDirectedEdges;
}



// Overloading the "<<" operator in ostream for the "std::cout" to be a "std::cout" to print the description of the node. Not completely implemented.
template <typename NodeType>
inline std::ostream& operator<<(std::ostream& stream, const Node<NodeType>& node)
{
    return stream << "The node ID: " << node.nodeID;
}
#endif /* Node_hpp */
