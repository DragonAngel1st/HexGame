//
//  HexGameGraph.hpp
//  Part B - Assignment 4 - Hex Game
//
//  Created then Modified by Patrick Miron on 2022-01-26.
//

#ifndef HexGameGraph_hpp
#define HexGameGraph_hpp

#include <stdio.h>
#include "Graph.hpp"
#include "Path.hpp"
#include "PriorityQueue.hpp"
#include "HexNode.hpp"
#include "MST.hpp"

class HexGameGraph : public Graph<HexNode>
{
public:
    HexGameGraph(int gameSize=5);
    MST<HexNode>* getMinimumSpanningTreePRIM(int startingNodeID, HexNodeState state);
    friend class HexGame;

private:
    Path<HexNode> * getShortestPath(HexNode * ptrToStartNode, HexNode * ptrToEndNode, HexNodeState nodeState);
};

#endif /* HexGameGraph_hpp */
