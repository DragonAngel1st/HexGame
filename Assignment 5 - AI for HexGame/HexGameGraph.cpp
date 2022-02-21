//
//  HexGameGraph.cpp
//  Assignment 5 - AI for HexGame
//
//  Created by Patrick Miron on 2022-02-20.
//

#include <stdio.h>
#include "HexGameGraph.hpp"

inline HexGameGraph::HexGameGraph(int gameSize): Graph<HexNode>{gameSize*gameSize}{};

inline MST<HexNode>* HexGameGraph::getMinimumSpanningTreePRIM(int startingNodeID, HexNodeState state)
{
    //Create the tree
    MST<HexNode>* minimumSpanningTree = new MST<HexNode>(true);
    //Create a priority queue for all unvisited nodes
    PriorityQueue<HexNode> pqUnvisited;
    //Arbitraly choose the starting node
    HexNode* ptrToCurrentNode = getNodePtr(startingNodeID);
    
    //Reset shortestDistance of each Node to infinity
    for (HexNode* nodePtr : _graphNodeList)
    {
        nodePtr->shortestDistanceToCurrentNodeInPath = INFINITY;
        //Do not include the starting node in the unvisited nodes, saves on more work later
        //Modification that is different from base Graph function to only add node of correct state
        if (nodePtr->nodeID != startingNodeID && nodePtr->state == state)
        {
            pqUnvisited.addNode(nodePtr);
        }
    }
    //Set starting node's shortest distance to 0.
    ptrToCurrentNode->shortestDistanceToCurrentNodeInPath = 0;
    
    //Repeat until the unvisited node queue is empty or the graph is not connected.
    while (!pqUnvisited.empty())
    {
        //Iterate through the neighbors of the current node and updates it's shortest distance to it.
        vector<HexNode*> vectorOfPtrsToCurrentNodesNeigbors = *(ptrToCurrentNode->ptrToVectorOfNeighborPtrs);
        for (auto currentNeighborPtr : vectorOfPtrsToCurrentNodesNeigbors )
        {
            //Modification that is different from base Graph function to only add node of correct state
            if (currentNeighborPtr->state == state)
            {
                int costToCurrentNeighbor = getEdgeLenght(ptrToCurrentNode, currentNeighborPtr);
                //If the current edge cost to the current neighbor is less then previous cost then store this value as the new shortestDistance...
                //Make sure to record the short edge to get to the current neighbor
                if (costToCurrentNeighbor < currentNeighborPtr->shortestDistanceToCurrentNodeInPath)
                {
                    currentNeighborPtr->shortestDistanceToCurrentNodeInPath = costToCurrentNeighbor;
                    //Record the shortest edge of the current neighboor
                    currentNeighborPtr->ptrToEdgeWithShortestDistance = getEdgePtr(*ptrToCurrentNode, *currentNeighborPtr);
                }
            }
        }
        //Sort he unvisited queue of nodes by shortest edges to each.
        // The next node with the shortest edge is pushed at the front of the queue.
        pqUnvisited.sortByShortestPath();
        //Pop the queue to get the front node
        ptrToCurrentNode = pqUnvisited.pop();
        //If the graph is disconnected we must end the current loop inform the user that the graph is disconnected and does not contain a minimum spanning tree.
        //We also set the valid flag on the MST to be false.
        if (ptrToCurrentNode->shortestDistanceToCurrentNodeInPath == INFINITY)
        {
            cout << "Warning - There are still unvisited nodes but the graph is broken (incomplete)" << endl;
            //Note that all MSTs are initialized with true as the isValid flag.
            minimumSpanningTree->isValid = false;
            break;
        }
        //Add the edge with the smallest cost of the current node.
        //Modification that is different from base Graph function to only add node of correct state
        assert(ptrToCurrentNode->state == state);
        minimumSpanningTree->addEdge(ptrToCurrentNode->ptrToEdgeWithShortestDistance);
    }
    //Return the current MST even if it is not valid.
    return minimumSpanningTree;
}

inline Path<HexNode>* HexGameGraph::getShortestPath(HexNode * ptrToStartNode, HexNode * ptrToEndNode, HexNodeState nodeState)
{
    // My class Path is a vector of nodes and only assigns starting node when initializing with
    // 0.0 as it's Node.shortestDistanceToCurrentNodeInPath as a double.
    if (!_graphNodeList.empty())
    {
        // Next for loop is necessary evil until shortest distance is ported to Path as it should.
        for (auto* nodePtr : _graphNodeList)
        {
            //HexNode* hexNodePtr = static_cast<HexNode*>(nodePtr);
            nodePtr->shortestDistanceToCurrentNodeInPath = Infinity;
        }
        // create a Path with the first node
        Path<HexNode> * shortestVisitedPath = new Path<HexNode>(ptrToStartNode);
        // We set the paths starting distance as good coding since the Path class does that automatically when adding the start node.
        shortestVisitedPath->totalDistance = 0.0;
        // Ptr to node currently being evaluated during the next loop
        
        HexNode * ptrToCurrentNode;
        // Ptr to shortest node currently being evaluated in the "Neighboring" nodes loop.
        HexNode * ptrToShortestEdgeNode = nullptr;
        
        // Create new queue for unvisited nodes
        PriorityQueue<HexNode> unvisitedQueue;
        // Create new priority queue called pQ for visited nodes.
        PriorityQueue<HexNode> pQ;
        
        
        // add all nodes to unvisited PriorityQueue
        for(auto* nodePtr : _graphNodeList)
        {
            if (nodePtr->state == nodeState)
            {
                unvisitedQueue.addNode(nodePtr);
            }
        }
        // Remove the start node from the unvisited queue.
        unvisitedQueue.removeNode(ptrToStartNode);
        // Add the start node to the visited queue
        pQ.addNode(ptrToStartNode);
        // Set the currently evaluated node ptr to the start node.
        ptrToCurrentNode = ptrToStartNode;
        // Make sure it's shortest distance to it for the path is 0.0 THis is redundant again but good practice.
        ptrToCurrentNode->shortestDistanceToCurrentNodeInPath = 0.0;
        
        //Start loop here
        int iterationCount = 0; // iteration counter
        while (!unvisitedQueue.empty())
        {
            // Set the current shortest edge for the loop of ptr to neighbor nodes.
            double currentShortestEdge = Infinity;
            // Get a vector with the pointers of neighboring nodes of the current node
            vector<HexNode*>* vectorOfPtrsToCurrentNodesNeigbors = ptrToCurrentNode->ptrToVectorOfNeighborPtrs;
            // If there is neighboring nodes then continue, else the path is unconnected with the next nodes from starting node see else for what happens next if this is the case.
            if (vectorOfPtrsToCurrentNodesNeigbors->size() > 0)
            {
                // Run through all the neighbor nodes and record to evaluate each of them
                for (HexNode* currentNeighborPtr : *vectorOfPtrsToCurrentNodesNeigbors)
                {
                    // If the node exist in the in the unvisted queue (here we could also use if it's not in visited queue) and updates it
                    if (currentNeighborPtr->state == nodeState){
                        if (unvisitedQueue.nodeExist(currentNeighborPtr))
                        {
                            // Get the edge length/cost between current node and its current neighboor
                            double edgeLengthCurrentToNeighbor = getEdgeLenght(ptrToCurrentNode, currentNeighborPtr);
                            
                            // Update the shortest distance to current neighbor or current to distance from current node (edgeLengtCurrentToNeighbor) + the previous node's(here we used the current node since the previous of the current neighbor is the current node) shortest distance
                            if (edgeLengthCurrentToNeighbor < currentNeighborPtr->shortestDistanceToCurrentNodeInPath)
                            {
                                (currentNeighborPtr->shortestDistanceToCurrentNodeInPath) = ptrToCurrentNode->shortestDistanceToCurrentNodeInPath + edgeLengthCurrentToNeighbor;
                            }
                            
                            // If the currentNeighbor's distance is less then the current shortest edge, then set the currentShortestEdge to edge of the current neighbor as well as ptr to shortest edge node to current neighbor node.
                            if (edgeLengthCurrentToNeighbor < currentShortestEdge)
                            {
                                currentShortestEdge = edgeLengthCurrentToNeighbor;
                                ptrToShortestEdgeNode = currentNeighborPtr;
                            }
                        }
                    }
                }
                
                //Warning if the shortest edge node is the same ptr as the ptrToCurrent node, then the graph is unconnected and the last node did not have any neighbors left in the queue. Or in the case of the startNode, this pointer will still be nullptr.
                if (ptrToCurrentNode == ptrToShortestEdgeNode || ptrToShortestEdgeNode == NULL)
                {
                    // Set the current node to previous node
                    HexNode * ptrToPreviousNode = ptrToCurrentNode;
                    // We get the next current node from the unvisited queue. Order does not really matter here.
                    ptrToCurrentNode = (unvisitedQueue.getNextNode());
                    // Remove this current node from the unvisited queue.
                    unvisitedQueue.removeNode(ptrToCurrentNode);
                    //FIXME: This work is not required for homework but should be fixed to do store the node in a disconnected set
                    // If the next node is still the same then pop it off the unvisited Q
                    // We have to evaluate this just in case that the new node from the unvisited queue
                    //  happens to be shortest edge neighbor node to the previous node (previous to current node)
                    //  which was not removed from the stack since it's previous node has no neighboor.
                    //  Note, after changing the order of this and bellow "if" statements, this does not seem to
                    //  hapen anymore and maybe redundant.
                    if (ptrToCurrentNode == ptrToPreviousNode)
                    {
                        //cout << "WARNING - The graph is disconnected from node " << ptrToCurrentNode->nodeID << endl;
                        ptrToCurrentNode = (unvisitedQueue.getNextNode());
                    }
                    //Set the ptr to shortest edge node to nullptr for next iteration of the current loop.
                    ptrToShortestEdgeNode = nullptr;
                    // If the univisted queue is empty or the ptrToCurrentNode is a nullptr, exit the function and
                    // return the current path as the current shortest path. This returns the current "CONNECTED" shortest
                    // path.
                    if (unvisitedQueue.empty() || ptrToCurrentNode == nullptr)
                    {
                        return shortestVisitedPath;
                    }
                    iterationCount++;
                    //Exit current loop early
                    continue;
                }
                else
                {
                    cout << "---- THE GRAPH IS UNCONNECTED ----" << endl;
                    return NULL;
                }
                // Add the ptr to shortest edge node of the neighbor to the shortest path.
                shortestVisitedPath->addNode(ptrToShortestEdgeNode);
                // Remove the ptr to shortes edge node from the unvisted queue
                unvisitedQueue.removeNode(ptrToShortestEdgeNode);
                // Add this pointer to visited queue
                pQ.addNode(ptrToShortestEdgeNode);
                // Set the next current node to the current shortest edge node.
                ptrToCurrentNode = ptrToShortestEdgeNode;
            }
            // If the case fails, set ptr to current node to the next node on the unvisited queue.
            //  which one is chosen is not important and does not matter
            else
            {
                ptrToCurrentNode = (unvisitedQueue.getNextNode());
                // Set the ptr to shortest edgenode to nullptr. Done for good pratice.
                ptrToShortestEdgeNode = nullptr;
            }
            
            // If we discover that ptr to current node is the same a the ptrToEnd node, exit the shortest path function and return the current shortest path, we have arrived at destination.
            if(ptrToEndNode != nullptr && ptrToCurrentNode->nodeID == ptrToEndNode->nodeID)
            {
                return shortestVisitedPath;
            }
            // If our ptr to current node has not been set to next node or has been set to nullptr, exit shortest path function and return the current shortest path, there are no more nodes left in the unvisited queue.
            if (ptrToCurrentNode == nullptr)
            {
                return shortestVisitedPath;
            }
            iterationCount++;
        }
        return shortestVisitedPath;
    }
    return NULL;
}
