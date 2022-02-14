//
//  PriorityQueue.hpp
//  Part B - Assignment 4 - Hex Game
//
//  Created then Modified by Patrick Miron on 2022-01-25.
//

//PriorityQueue is a template class for the typename NodeType. This was modified so that Graph/Node/Edge classes can be modified later for each type of NodeType -> Graph<NodeType>, in other words, the Node<NodeType> can be subclassed in a new NodeType that has extra data. This is to recreate the functionallity like a vector<Type> from the standard template library.

#ifndef PriorityQueue_hpp
#define PriorityQueue_hpp

#include <stdio.h>
#include <string>

//Bellow class is forwarded
template<typename NodeType>
class Node;

using namespace std;

// PriorityQueue class is used to contained the labeled/visited nodes and the unlabeled/unvisited nodes.
// The queue only holds pointers to the nodes to prevent high amounts of data being copied if nodes ever carry more data.
template <typename NodeType>
class PriorityQueue
{
public:
    // The default constructor here is overidden with making sure that the queue does not have a nullptr, this is to facilitate
    //  branching and program pathing
    PriorityQueue();
    // Public function to sort queue by nodePtr->shortestDistanceToCurrentNodeInPath
    void sortByShortestPath();
    
    // Public function to access local members and prevent unwanted use.
    // Returns a node ptr that cannot be changed by caller.
    const vector<NodeType*> getNodesInQueue() const;
    
    NodeType * pop();
    
    // Checks is a node is part of the queue.
    bool nodeExist(NodeType* nodePtr);
    
    // Adds a node to the queue.
    void addNode(NodeType* ptrToNode);
    
    // Function to add a bunch of nodes through a vector of ptrs to nodes.
    void addVectorOfNodes(vector<NodeType*> neighboringNodes);
    
    // This function returns the next node ptr in the front of the queue WITHOUT deleting it.
    NodeType * getNextNode();
    
    // This function removes a node ptr in the queue at any location in the queue and returns nothing.
    // Please note that this function will run in O(degre of Nodes) because it has to update each nodes location when it deletes it.
    void removeNode(NodeType * nodePtr);
    
    // Returns true if the queue is empty
    bool empty();
    
    // Helper function to visualy see a queue's contents
    void print(string nameOfQueue);
    // The vector<Node *> is accessible only by public functions.
    vector<NodeType*> * _ptrToPriorityQueue;
    
};

// The default constructor here is overidden with making sure that the queue does not have a nullptr, this is to facilitate
//  branching and program pathing
template <typename NodeType>
PriorityQueue<NodeType>::PriorityQueue()
{
    _ptrToPriorityQueue = new vector<NodeType*>;
}
// Public function to sort queue by nodePtr->shortestDistanceToCurrentNodeInPath

template <typename NodeType>
void PriorityQueue<NodeType>::sortByShortestPath()
{
    sort(_ptrToPriorityQueue->begin(),_ptrToPriorityQueue->end(), [](const Node<NodeType> * lhs, const Node<NodeType> * rhs){
        return lhs->shortestDistanceToCurrentNodeInPath < rhs->shortestDistanceToCurrentNodeInPath;
    });
}

// Public function to access local members and prevent unwanted use.
// Returns a node ptr that cannot be changed by caller.
template <typename NodeType>
const vector<NodeType*> PriorityQueue<NodeType>::getNodesInQueue() const
{
    return *_ptrToPriorityQueue;
}

template <typename NodeType>
NodeType* PriorityQueue<NodeType>::pop()
{
    NodeType * nodePtr = *_ptrToPriorityQueue->begin();
    _ptrToPriorityQueue->erase(_ptrToPriorityQueue->begin());
    return nodePtr;
}

// Checks is a node is part of the queue.
template <typename NodeType>
bool PriorityQueue<NodeType>::nodeExist(NodeType * nodePtr)
{
    for(int index=0 ; index < _ptrToPriorityQueue->size() ; index++)
    {
        if((*_ptrToPriorityQueue)[index]->nodeID == nodePtr->nodeID)
        {
            return true;
        }
    }
    return false;
}

// Adds a node to the queue.
template <typename NodeType>
void PriorityQueue<NodeType>::addNode(NodeType * ptrToNode)
{
    _ptrToPriorityQueue->push_back(ptrToNode);
    // Set the node's memoization of its location in the queue. In future version of program, this could be done with an Hashtable without adding to much cost in O(times)
    ptrToNode->indexOfNodeInUnvistedQueue = int(_ptrToPriorityQueue->size()-1);
}

// Function to add a bunch of nodes through a vector of ptrs to nodes.
template <typename NodeType>
void PriorityQueue<NodeType>::addVectorOfNodes(vector<NodeType*> neighboringNodes)
{
    for (NodeType * eachNode : neighboringNodes)
    {
        _ptrToPriorityQueue->push_back(eachNode);
        eachNode->indexOfNodeInUnvistedQueue = int(_ptrToPriorityQueue->size()-1);
    }
}

// This function returns the next node ptr in the front of the queue WITHOUT deleting it.
template <typename NodeType>
NodeType* PriorityQueue<NodeType>::getNextNode()
{
    return _ptrToPriorityQueue->front();
}

// This function removes a node ptr in the queue at any location in the queue and returns nothing.
// Please note that this function will run in O(degre of Nodes) because it has to update each nodes location when it deletes it.
template <typename NodeType>
void PriorityQueue<NodeType>::removeNode(NodeType* nodePtr)
{
    int nodeIDtoRemove = nodePtr->indexOfNodeInUnvistedQueue;
    *_ptrToPriorityQueue->erase(_ptrToPriorityQueue->begin()+nodeIDtoRemove);
    for (Node<NodeType> * ptrNode : vector<Node<NodeType>*>(_ptrToPriorityQueue->begin()+nodeIDtoRemove,_ptrToPriorityQueue->end()))
    {
        ptrNode->indexOfNodeInUnvistedQueue -= 1;
    }
}

// Returns true if the queue is empty
template <typename NodeType>
bool PriorityQueue<NodeType>::empty()
{
    return _ptrToPriorityQueue->empty();
}

// Helper function to visualy see a queue's contents
template <typename NodeType>
void PriorityQueue<NodeType>::print(string nameOfQueue)
{
    cout << endl;
    cout << nameOfQueue << " queue: (front of queue is on the left)" << endl;
    for(Node<NodeType> * nodePtr : *_ptrToPriorityQueue)
    {
        cout << nodePtr->nodeID << "=" << nodePtr->shortestDistanceToCurrentNodeInPath << ", ";
    }
    cout << endl;
}

#endif /* PriorityQueue_hpp */
