//
//  MST.hpp
//  Hex Game
//
//  Created then Modified by Patrick Miron on 2022-01-25.
//

//MST is a template class for the typename NodeType. This was modified so that Graph/Node/Edge can be modified later for each type of NodeType -> Graph<NodeType>, in other words, the Node<NodeType> can be subclassed in a new NodeType that has extra data. This is to recreate the functionallity like a vector<Type> from the standard template library.

#ifndef MST_hpp
#define MST_hpp

#include <stdio.h>
#include <iostream>


using namespace std;

//Minimum spanning tree. "As a collection of edges"
template<typename NodeType>
class MST
{
public:
    bool isValid; // This is a flag to know if the minimum spanning tree created is valid and used all nodes in the graph.
    
    void addEdge(Edge<NodeType>* ptrToEdge)
    {
        _edges.push_back(ptrToEdge);
    }
    
    bool nodeExist(int nodeID)
    {
        for (Edge<NodeType>* edgePtr : _edges)
        {
            if (edgePtr->fromNodeID==nodeID || edgePtr->toNodeID==nodeID)
            {
                return true;
            }
        }
        return false;
    }
    
    //Helper function to see the MST on the console
    void print()
    {
        {
            int totalWeightOfTree =0;
            cout << "~";
            for (auto edge : _edges)
            {
                cout << "Edge " <<edge->fromNodePtr->nodeID  << "<->" << edge->toNodePtr->nodeID << "(" << edge->cost << "), ";
                totalWeightOfTree += edge->cost;
            }
            cout << endl;
            //cout << "The total edge cost is: " << totalWeightOfTree << endl;
        }
    }
    //Constructor takes care of empty constructor also by setting isValid default to true.
    MST(bool isValid):isValid(isValid=true){}
    MST& operator=(const MST&& other){return this;};
private:
    vector<Edge<NodeType>*> _edges; // hidden vector of edges for the MST
};

#endif /* MST_hpp */
