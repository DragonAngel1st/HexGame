//
//  Path.hpp
//  Part B - Assignment 4 - Hex Game
//
//  Created then Modified by Patrick Miron on 2022-01-25.
//

//Path is a template class for the typename NodeType. This was modified so that the templated Graph/Node/Edge classes can be modified later for each type of NodeType -> ex: Graph<NodeType>, in other words, the Node<NodeType> can be subclassed in a new NodeType that has extra data. This is to recreate the functionallity like a vector<Type> from the standard template library.


#ifndef Path_hpp
#define Path_hpp

#include <stdio.h>
#include "Node.hpp"


// The class path is for storing paths/shortest paths resulting from Dijkstra's algorithm.
template <typename NodeType>
class Path
{
public:
    double totalDistance; // Records the total/current distance of the path.
    
    vector<Node<NodeType>*> nodesVector; // Records the nodes in order of the path recorded/traversed
    
    // Create a default constructor since a custom one was created bellow. The graph itself never uses the empty constructor.
    //  It is used to create an empty path in the results to hold the shortest and longest paths of the iteration of averaging the shortest path algorithm.
    Path();
    
    // Each path is instantiated by the Dijkstra's algoritm in the graph with a ptr to the start node and a starting cost/distance of 0.0
    Path(Node<NodeType> * ptrToStartNode);
    
    // Public function for Adding a node ptr to the path
    void addNode(Node<NodeType> * ptrToNewNode);
    
    // Helper function to see the results visually to the console.
    void print();
};

using namespace std;
template <typename NodeType>
Path<NodeType>::Path(){};

// Each path is instantiated by the Dijkstra's algoritm in the graph with a ptr to the start node and a starting cost/distance of 0.0
template <typename NodeType>
Path<NodeType>::Path(Node<NodeType> * ptrToStartNode):totalDistance(0.0)
{
    nodesVector.push_back(ptrToStartNode);
}

// Public function for Adding a node ptr to the path
template <typename NodeType>
void Path<NodeType>::addNode(Node<NodeType> * ptrToNewNode)
{
    nodesVector.push_back(ptrToNewNode);
    //The total is recorded in the last node. This next line rewrites the total cost going through all nodes in path.
    totalDistance = ptrToNewNode->shortestDistanceToCurrentNodeInPath;
}

// Helper function to see the results visually to the console.
template <typename NodeType>
void Path<NodeType>::print()
{
    cout << "From start node " << nodesVector[0]->nodeID << " to End node " << nodesVector[nodesVector.size()-1]->nodeID << " :" << endl;
    double totalPathCost = 0.0;
    int nodeCount = 0;
    cout << "  " << setw(3);
    for(Node<NodeType> * nodePtr : nodesVector)
    {
        cout << right << nodePtr->nodeID  << setw(2);
        if (nodeCount != nodesVector.size()-1)
        {
            cout << "->" << left << setw(2);
        }
        if ((nodeCount+1) % 20 == 0)
        {
            cout << endl;
            cout << "" << setw(3);
        }
        
        
        //The total is recorded in the last node. This next line rewrites the total cost going through all nodes in path.
        totalPathCost = nodePtr->shortestDistanceToCurrentNodeInPath;
        nodeCount++;
    }
    cout << endl;
    cout << "Total Path Cost: " << totalPathCost << endl;
    cout << endl;
}

#endif /* Path_hpp */
