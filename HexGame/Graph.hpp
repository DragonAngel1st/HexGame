//
//  Graph.hpp
//  Part B - Assignment 4 - Hex Game
//
//  Created then Modified by Patrick Miron on 2022-01-24.
//

// Note to self: To change the path where the program runs in and load other file from (in XCode) :
//   Goto to the menu Product > Scheme > Edit Scheme > Run > options and enable the Working Directory [] Use custom directory and choose where the main.cpp and input files are.

//Graph is a template class for the typename NodeType. This was modified so that Graph can be modified later for each type of NodeType -> Graph<NodeType>, in other words, the Node<NodeType> can be subclassed in a new NodeType that has extra data. This is to recreate the functionallity like a vector<Type> from the standard template library.

#ifndef Graph_h
#define Graph_h
#include <iostream>
#include <queue>
#include <algorithm>
#include <sys/time.h>
#include <fstream>
#include <cmath>
//If the object of the bellow class must be instantiated in this template they must also have their file included here.
#include "Edge.hpp"
#include "Node.hpp"

using namespace std;

//Bellow class is forwarded
template <typename NodeType>
class Edge;

//Bellow class is forwarded
template <typename NodeType>
class Node;

//Bellow class is forwarded
template <typename NodeType>
class Path;

//Bellow class is forwarded
template <typename NodeType>
class MST;

//Bellow class is forwarded
template <typename NodeType>
class PriorityQueue;

/// Graph of nodes, running in 0(1) time.
template <typename NodeType>
class Graph
{
public:
    int TotalNodes() {return _totalNodes;}; // Maintains number of nodes/vertices in graph
    int TotalEdges() {return _totalEdges;}; // Maintains number of edges in graph
    int srandSeed() {return _srandSeed;}; // For reporting purpouse, each graphs randomizer seed is recorded here.
    const vector<NodeType*> getNodes() const
    {
        return _graphNodeList;
    };
    const vector<Edge<NodeType>*> getEdges() const
    {
        const vector<Edge<NodeType>*> * const edgeList(_graphEdgeList.begin(),_graphEdgeList.end());
        return edgeList;
    };
    // Function that return true if an edge exist in the graph in O(1) times.
    bool isEdge(const NodeType & ptrStartNode, const NodeType & ptrToEndNode) const
    {   if (_isNodeValid(ptrStartNode.nodeID) && _isNodeValid(ptrToEndNode.nodeID))
        {
            if (_connectedNodesDirectedEdgePtrsMatrix[ptrStartNode.nodeID][ptrToEndNode.nodeID] != nullptr)
            {
                return true;
            }
        }
        return false;
    };
    
    // Returns an edge in O(1)
    Edge<NodeType> * getEdgePtr(const NodeType & fromNode, const NodeType & toNode)
    {
        if (isEdge(fromNode, toNode))
        {
            return _connectedNodesDirectedEdgePtrsMatrix[fromNode.nodeID][toNode.nodeID];
        }
        else
        {
            cout << "Error, the edge does not exist." << endl;
        }
        return nullptr;
    };
    
    // Returns true if node exist in the graph in O(1). Since the graph constrols the ID creation of each node, we can use the size of the vector of node ptrs.
    // Warning, creating an empty graph and not adding nodes in order of IDs starting a 0 will not give correct values from this function.
    //   Since this was not requested to create graph manually in the assignment, this function should not be used if one does so.
    bool nodeExistInGraphNodeList(int nodeID)
    {
        if (!_graphNodeList.empty())
        {
            if (_graphNodeList.size() >= nodeID+1)
            {
                return true;
            }
        }
        return false;
    };
    
    // Function to create a graph from a vector of vectors of doubles (A matrix of edges cost per nodes, undirected)
    // Traverse tree with breath first and get all nodes that are connected
    void createGraphStructureFrom(vector<vector<double>> matrixOfNodeEdgeCosts)
    {
        int rowIndex = 0;
        int colIndex;
        
        // Here the graph maintains a matrix of edge ptrs to enable retrieve of an edge and nodes in O(1).
        _connectedNodesDirectedEdgePtrsMatrix = vector<vector<Edge<NodeType>*>>(matrixOfNodeEdgeCosts.size(), vector<Edge<NodeType>*>(matrixOfNodeEdgeCosts.size(), nullptr ));
        
        // Row data is a vector of cost/distances between the row as the fromNode ID to the toNode ID which is a column.
        // In other words, the row index is the id of the node that the edge start at and the column index is the id of the node that the edge ends.
        // Iterate through all the edges each start nodes as rows
        for(vector<double> eachRow : matrixOfNodeEdgeCosts)
        {
            colIndex = 0;
            NodeType * ptrToRowNode;
            if (!nodeExistInGraphNodeList(rowIndex))
            {
                ptrToRowNode = addNode(rowIndex);
            }
            else
            {
                ptrToRowNode = _graphNodeList[rowIndex];
            }
            
            // Iterate through all the edges end nodes as columns
            for(double distance : eachRow)
            {
                // Ignore all edges that result in a node pathing to itself
                if (rowIndex != colIndex)
                {
                    NodeType * ptrToConnectingNode;
                    // If the node ptr does not already exist in the graph create it and if it exist get ptr to it.
                    if (!nodeExistInGraphNodeList(colIndex))
                    {
                        ptrToConnectingNode = addNode(colIndex);
                    }
                    else
                    {
                        ptrToConnectingNode = _graphNodeList[colIndex];
                        //cout << "Column-Found node with ID: " << ptrToConnectingNode->nodeID << endl;
                    }
                    // If the cost/distance is greater then 0.0 create an directed edge.
                    if (distance > 0.0)
                    {
                        //cout << "--------------------------------------------" << endl;
                        //cout << "Trying to add directed edge from node:" << ptrToRowNode->nodeID << " to node: " << ptrToConnectingNode->nodeID << endl;
                        //The createGraphStructure creates null ptr for all the
                        addDirectedEdge(*ptrToRowNode, *ptrToConnectingNode, distance);
                        //cout << "Added directed edge from node:" << ptrToRowNode->nodeID << " to node: " << ptrToConnectingNode->nodeID << endl;
                    }
                }
                colIndex++;
            }
            rowIndex++;
        }
    };
    
    //Add undirectedEdge in O(1).
    void addUndirectedEdge(NodeType & fromNodePtr, NodeType & toNodePtr, double edgeLenght)
    {
        if (_isNodeValid(fromNodePtr.nodeID) && _isNodeValid(toNodePtr.nodeID)) {
            //        if (_connectedNodesDirectedEdgePtrsMatrix[fromNodePtr->nodeID][toNodePtr->nodeID] == NULL)
            //        {
            //            // Here we assume that a undirected edge is the same as two edges from both the starting and ending node.
            addDirectedEdge(fromNodePtr, toNodePtr, edgeLenght);
            addDirectedEdge(toNodePtr, fromNodePtr, edgeLenght);
            //        }
        }
        // Mostly for debuging. May be used in the future for exception handling.
        else
        {
            if (!_isNodeValid(fromNodePtr.nodeID)) {
                cout << "Error, the fromNode does not exist." << endl;
                return;
            }
            cout << "Error, the toNode does not exist." << endl;
            return;
        }
    };
    
    //Add directedEdge in O(1).
    void addDirectedEdge(NodeType & fromNodePtr, NodeType & toNodePtr, double edgeLenght)
    {
        if (_isNodeValid(fromNodePtr.nodeID) && _isNodeValid(toNodePtr.nodeID)) {
            //If the edge does not exist at all
            Edge<NodeType> * edgePtr = new Edge<NodeType>(fromNodePtr, toNodePtr);
            _totalEdges++;
            edgePtr->cost = edgeLenght;
            
            _connectedNodesDirectedEdgePtrsMatrix[fromNodePtr.nodeID][toNodePtr.nodeID] = edgePtr;
            fromNodePtr.ptrToVectorDirectedEdges->push_back(edgePtr);
            fromNodePtr.ptrToVectorOfNeighborPtrs->push_back(&toNodePtr);
            _graphEdgeList.push_back(edgePtr);
            edgePtr->indexOfDirectedEdgeInGraphVectorOfEdges = int(_graphEdgeList.size()-1);
        }
        // The Dijkstra's function properly uses the addDirectedEdge function. This else if for future exception handling and if a user chooses to create graph manualy which is not recommended but possible.
        else
        {
            cout << "Error, either the fromNode or the toNode does not exist." << endl;
            return;
        }
    };
    
    // Warning, remove Edge is in O(1+1+1) which is same theorically a O(1) times since it has to look for the edge at 3 locations.
    void removeDirectedEdge(NodeType & fromNode, NodeType & toNode)
    {
        if (_isNodeValid(fromNode.nodeID) && _isNodeValid(toNode.nodeID))
        {
            if (_connectedNodesDirectedEdgePtrsMatrix[fromNode.nodeID][toNode.nodeID] != nullptr)
            {
                //Delete edge ptr in graph's edge list
                int edgeIndex = getEdgePtr(fromNode, toNode)->indexOfDirectedEdgeInGraphVectorOfEdges;
                delete(_graphEdgeList[edgeIndex]);
                
                //Delete edge ptr in Node list's vector of edge pointers
                int nodeIndex = fromNode.indexOfNodeInGraphVectorOfNodes;
                
                delete(_graphNodeList[nodeIndex]);
                _graphNodeList.erase(_graphNodeList.begin()+nodeIndex);
                
                //Delete edge ptr in connectedNodesMatrix, here we only set the pointer to nullptr since the matrix structure must be maintained.
                _connectedNodesDirectedEdgePtrsMatrix[fromNode.nodeID][toNode.nodeID] = nullptr;
                
                //Decrement TotalEdges
                _totalEdges--;
            }
        }
        // Else mostly used for debugging and future exception handling.
        else
        {
            cout << "Error, the edge does not exist." << endl;
        }
    };
    
    // Sets edge lenght/cost in O(1).
    void setDirectedEdgeLenght(const NodeType & fromNode, const NodeType & toNode, double edgeLenght)
    {
        if (_isNodeValid(fromNode.nodeID) && _isNodeValid(toNode.nodeID))
        {
            if (_connectedNodesDirectedEdgePtrsMatrix[fromNode.nodeID][toNode.nodeID] != nullptr)
            {
                cout << "Error, a directed edge between fromNode and toNode does not exist." << endl;
                return;
            }
            Edge<NodeType> * currentEdgePtr = _connectedNodesDirectedEdgePtrsMatrix[fromNode.nodeID][toNode.nodeID];
            currentEdgePtr->cost = edgeLenght;
        }
        
    };
    
    // Sets a undirected edge (edge in both direction) to fromNode and toNode.
    void setUndirectedEdgeLenght(NodeType & fromNode, NodeType & toNode, double edgeLenght)
    {
        if ((_connectedNodesDirectedEdgePtrsMatrix[fromNode.nodeID][toNode.nodeID] != nullptr) && (_connectedNodesDirectedEdgePtrsMatrix[toNode.nodeID][fromNode.nodeID] != nullptr))
        {
            setDirectedEdgeLenght(fromNode, toNode, edgeLenght);
            setDirectedEdgeLenght(toNode, fromNode, edgeLenght);
        }
        // THis else should not happen but is there for debugging and future exception handling.
        else
        {
            if (_connectedNodesDirectedEdgePtrsMatrix[fromNode.nodeID][toNode.nodeID] != nullptr)
            {
                cout << "Error, a directed edge between fromNode and toNode does not exist." << endl;
                return;
            }
            else
            {
                cout << "Error, a directed edge between toNode and fromNode does not exist." << endl;
                return;
            }
        }
    };
    
    // Returns the edge lenght in O(1).
    double getEdgeLenght(const NodeType * fromNode, const NodeType * toNode) const
    {
        if (_isNodeValid(fromNode->nodeID) && _isNodeValid(toNode->nodeID))
        {
            if (isEdge(*fromNode, *toNode))
            {
                return _connectedNodesDirectedEdgePtrsMatrix[fromNode->nodeID][toNode->nodeID]->cost;
            }
        }
        return INFINITY;
    };
    
    // Adds a column and row to the connected nodes matrix in O(1) and returns a pointer to the Node created.
    NodeType * addNode(int nodeID)
    {

        NodeType * newNodePtr;
        newNodePtr = new NodeType(nodeID);
        _graphNodeList.push_back(newNodePtr);
        assert(_graphNodeList[nodeID]->nodeID == nodeID);
        newNodePtr->indexOfNodeInGraphVectorOfNodes = int(_graphNodeList.size()-1);
        
        _totalNodes++;
        return newNodePtr;
    };
    
    // Prints graph to the console as well as the all internal vectors and matrix.
    void print()
    {
        cout << "{";
        int rowIndex = 0;
        for (vector<Edge<NodeType>*> row: _connectedNodesDirectedEdgePtrsMatrix)
        {
            int columnIndex = 0;
            if (rowIndex > 0)
            {
                cout << " ";
            }
            cout << "{ ";
            for (auto item: row) {
                cout << rowIndex << "->" << columnIndex;
                if (item != NULL)
                {
                    cout << " " << item->cost;
                }
                else{
                    cout << " -";
                }
                if (columnIndex < row.size()-1)
                {
                    cout << ", ";
                }
                columnIndex++;
            }
            cout << " }";
            if (rowIndex < row.size()-1)
            {
                cout << endl;
            }
            rowIndex++;
        }
        cout << "}" << endl;
    };
    
    // This method returns the shortest path between 2 nodes using Diskstra's Shortest Path Algorithm
    Path<NodeType> * getShortestPath(NodeType * ptrToStartNode, NodeType * ptrToEndNode)
    {
        // My class Path is a vector of nodes and only assigns starting node when initializing with
        // 0.0 as it's Node.shortestDistanceToCurrentNodeInPath as a double.
        if (!_graphNodeList.empty())
        {
            // Next for loop is necessary evil until shortest distance is ported to Path as it should.
            for (auto eachNodePtr : _graphNodeList)
            {
                eachNodePtr->shortestDistanceToCurrentNodeInPath = INFINITY;
            }
            // create a Path with the first node
            Path<NodeType> * shortestVisitedPath = new Path(ptrToStartNode);
            // We set the paths starting distance as good coding since the Path class does that automatically when adding the start node.
            shortestVisitedPath->totalDistance = 0.0;
            // Ptr to node currently being evaluated during the next loop
            
            Node<NodeType> * ptrToCurrentNode;
            // Ptr to shortest node currently being evaluated in the "Neighboring" nodes loop.
            Node<NodeType> * ptrToShortestEdgeNode = nullptr;
            
            // Create new queue for unvisited nodes
            PriorityQueue<NodeType> unvisitedQueue;
            // Create new priority queue called pQ for visited nodes.
            PriorityQueue<NodeType> pQ;
            
            
            // add all nodes to unvisited PriorityQueue
            for (Node<NodeType> * nodePtr : _graphNodeList)
            {
                unvisitedQueue.addNode(nodePtr);
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
                double currentShortestEdge = INFINITY;
                // Get a vector with the pointers of neighboring nodes of the current node
                vector<Node<NodeType>*> vectorOfPtrsToCurrentNodesNeigbors = *(ptrToCurrentNode->ptrToVectorOfNeighborPtrs);
                // If there is neighboring nodes then continue, else the path is unconnected with the next nodes from starting node see else for what happens next if this is the case.
                if (vectorOfPtrsToCurrentNodesNeigbors.size() > 0)
                {
                    // Run through all the neighbor nodes and record to evaluate each of them
                    for( NodeType * currentNeighborPtr : vectorOfPtrsToCurrentNodesNeigbors)
                    {
                        // If the node exist in the in the unvisted queue (here we could also use if it's not in visited queue) and updates it
                        if (unvisitedQueue.nodeExist(currentNeighborPtr))
                        {
                            // Get the edge length/cost between current node and its current neighboor
                            double edgeLengthCurrentToNeighbor = getEdgeLenght(ptrToCurrentNode, currentNeighborPtr);
                            
                            // Update the shortest distance to current neighbor or current to distance from current node (edgeLengtCurrentToNeighbor) + the previous node's(here we used the current node since the previous of the current neighbor is the current node) shortest distance
                            if (edgeLengthCurrentToNeighbor < currentNeighborPtr->shortestDistanceToCurrentNodeInPath)
                            {
                                currentNeighborPtr->shortestDistanceToCurrentNodeInPath = ptrToCurrentNode->shortestDistanceToCurrentNodeInPath + edgeLengthCurrentToNeighbor;
                            }
                            
                            // If the currentNeighbor's distance is less then the current shortest edge, then set the currentShortestEdge to edge of the current neighbor as well as ptr to shortest edge node to current neighbor node.
                            if (edgeLengthCurrentToNeighbor < currentShortestEdge)
                            {
                                currentShortestEdge = edgeLengthCurrentToNeighbor;
                                ptrToShortestEdgeNode = currentNeighborPtr;
                            }
                        }
                    }
                    
                    //Warning if the shortest edge node is the same ptr as the ptrToCurrent node, then the graph is unconnected and the last node did not have any neighbors left in the queue. Or in the case of the startNode, this pointer will still be nullptr.
                    if (ptrToCurrentNode == ptrToShortestEdgeNode || ptrToShortestEdgeNode == NULL)
                    {
                        // Set the current node to previous node
                        Node<NodeType> * ptrToPreviousNode = ptrToCurrentNode;
                        // We get the next current node from the unvisited queue. Order does not really matter here.
                        ptrToCurrentNode = unvisitedQueue.getNextNode();
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
                            ptrToCurrentNode = unvisitedQueue.getNextNode();
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
                    ptrToCurrentNode = unvisitedQueue.getNextNode();
                    // Set the ptr to shortest edgenode to nullptr. Done for good pratice.
                    ptrToShortestEdgeNode = nullptr;
                }
                
                // If we discover that ptr to current node is the same a the ptrToEnd node, exit the shortest path function and return the current shortest path, we have arrived at destination.
                if(ptrToEndNode != nullptr && ptrToCurrentNode->nodeID == ptrToEndNode->nodeID)
                {
                    return shortestVisitedPath;
                }
                // If our ptr to current node has not been set to next node or has be set to nullptr, exit shortest path function and return the current shortest path, there are no more nodes left in the unvisited queue.
                if (ptrToCurrentNode == nullptr)
                {
                    return shortestVisitedPath;
                }
                iterationCount++;
            }
            return shortestVisitedPath;
        }
        return NULL;
    };
    
    // Return a node pointer from it node ID from the internal vector of ptr of nodes.
    NodeType * getNodePtr(int nodeID)
    {
        return _graphNodeList[nodeID];
    };
    
    //--------------------   Added getMinimumSpanningTree function for assignment 4.
    //Function that returns the minimum spanning tree for the graph using PRIM's algorithm
    MST<NodeType>* getMinimumSpanningTreePRIM(int startingNodeID)
    {
        //Create the tree
        MST<NodeType>* minimumSpanningTree = new MST<NodeType>;
        //Create a priority queue for all unvisited nodes
        PriorityQueue<NodeType> pqUnvisited;
        //Arbitraly choose the starting node
        NodeType * ptrToCurrentNode = getNodePtr(startingNodeID);
        
        //Reset shortestDistance of each Node to infinity
        for (auto &nodePtr : _graphNodeList)
        {
            nodePtr->shortestDistanceToCurrentNodeInPath = INFINITY;
            //Do not include the starting node in the unvisited nodes, saves on more work later
            if (nodePtr->nodeID != startingNodeID)
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
            vector<NodeType*> vectorOfPtrsToCurrentNodesNeigbors = *(ptrToCurrentNode->ptrToVectorOfNeighborPtrs);
            for (auto currentNeighborPtr : vectorOfPtrsToCurrentNodesNeigbors )
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
            minimumSpanningTree->addEdge(ptrToCurrentNode->ptrToEdgeWithShortestDistance);
        }
        //Return the current MST even if it is not valid.
        return minimumSpanningTree;
    };
    
    
    ///Constructors
    ///Empty constructor to be used if we want to use the function to create a graph from matrix of edge lenghts. See description of Graph class.
    Graph()
    {
        _totalNodes=0;
        _totalEdges=0;
    };
    ///Warning this constructor only creates an empty _connectedNodesDirectedEdgePtrsMatrix with all null pointers. Use this construction only if user needs to create a graph manually. NOT recommended.
    Graph(int totalNodes)
    {
        _totalNodes=0;
        _totalEdges=0;
        //Initialize a vector of vector of doubles with both lenght = totalNodes with all 0
        // in other words this creates a square matrix of size totalNodes full of 0 distance values to neighboor
        _connectedNodesDirectedEdgePtrsMatrix = vector(totalNodes, vector<Edge<NodeType>*>(totalNodes));
    };
    
    // This constructor creates a graph of graphTotalNodes with random edge density between edgeDensityMin and edgeDensityMax
    //
    /// This constructor creates a graph of graphTotalNodes with random edge density between edgeDensityMin and edgeDensityMax and with random edge lenght between edgeLenghtMin and edgeLenghtMax
    Graph(int graphTotalNodes, double edgeDensity, double edgeLenghtMin, double edgeLenghtMax)
    {
        // This next instantiation is for creating a random graph and since the program can run through all its creation and evaluation within milliseconds, a more precise seed creator was needed.
        struct timeval t1;
        gettimeofday(&t1, NULL);
        _srandSeed = (int)(t1.tv_usec * t1.tv_sec);
        srand(_srandSeed);
        // Create empty matrix of directed edges
        _connectedNodesDirectedEdgePtrsMatrix = vector(graphTotalNodes, vector<Edge<NodeType>*>(graphTotalNodes));
        
        // For each row and column run through all the nodes as to and from nodes.
        for(int rowIndex = 0; rowIndex < graphTotalNodes; rowIndex++)
        {
            NodeType * ptrToRowNode; // Data at [Row] are distance to the ID of the row itself and it's column node.
            // If node does not exist in the graph create it.
            if (!nodeExistInGraphNodeList(rowIndex))
            {
                // Add node to graph.
                ptrToRowNode = addNode(rowIndex);
            }
            // If node exist retrieve it. (When one iteration of the row loop is done, all necessary nodes should have been created even if there is no edges between it and the current row node.
            else
            {
                // Get node ptr from graph.
                ptrToRowNode = _graphNodeList[rowIndex];
            }
            //Iterate through all the nodes as columns.
            for(int colIndex = 0; colIndex < graphTotalNodes; colIndex++)
            {
                //Warning, here we only want to do the upper triangle of the edge matrix since the next code adds directed edges so we don't overide the existing edges.
                if (rowIndex < colIndex)
                {
                    // Create a ptr to record the node the edge will pointing at.
                    NodeType * ptrToConnectingNode;
                    //If the node does not already exist, add it to the graph.
                    if (!nodeExistInGraphNodeList(colIndex))
                    {
                        ptrToConnectingNode = addNode(colIndex);
                    }
                    // If the node exist in the graph retrieve a ptr to it
                    else
                    {
                        ptrToConnectingNode = _graphNodeList[colIndex];
                    }
                    // Here we create an undirected edge depending random value optain is with the edge density request by the function
                    // Remember : At the beginning of the function, the edge matrix of edge ptrs was instantiate with all nullptr
                    if (_createEdgeDependingOn(edgeDensity))
                    {
                        // Add an undirected edge (as per requested in assignment.) with a random lenght/cost between the edgeLengthMin and edgeLenghtMax.
                        addUndirectedEdge(*ptrToRowNode, *ptrToConnectingNode, _randomEdgeLenghtBetween(edgeLenghtMin, edgeLenghtMax));
                        
                    }
                }
            }
        }
    };
    
    // This constructor loads a graph from file where the first integer (must be first item in the file) is the size of the graph (total number of nodes) and all the next lines are integer triples where the first and second are nodes(forming an edge from first to second) with the third being the cost of this edge.
    Graph(string inputFile)
    {
        ifstream dataFile("data.txt");
        
        if (dataFile.fail())
        {
            cout << "Error -- File cannot be opened." << endl;
        }
        istream_iterator<int> start(dataFile), end;
        vector<int> data(start, end);
        _totalNodes = *data.begin();
        int count = 1;
        // Because we are provided with the number of edges, we can use this value to verify that the file and that our codes creates the correct number of edges.
        for (auto iterator = data.begin()+1; iterator !=data.end(); iterator++)
        {
            NodeType * startNodePtr = nullptr;
            NodeType * endNodePtr = nullptr;
            
            int startNodeID = * iterator;
            
            //if start node does not exist create it
            if (!nodeExistInGraphNodeList(startNodeID))
            {
                startNodePtr = addNode(startNodeID);
            }
            else
            {
                startNodePtr = getNodePtr(startNodeID);
                //cout << "NODE with ID: " << startNodeID << " already exist." << endl;
            }
            
            int endNodeID = * ++iterator;
            
            //if end node does not exist create it.
            if (!nodeExistInGraphNodeList(endNodeID))
            {
                endNodePtr = addNode(endNodeID);
            }
            else
            {
                endNodePtr = getNodePtr(endNodeID);
                //cout << "NODE with ID: " << endNodeID << " already exist." << endl;
            }
            
            int cost = * ++iterator;
            //check if edge exist, if not create it.
            if (!isEdge(*startNodePtr, *endNodePtr))
            {
                addUndirectedEdge(*startNodePtr, *endNodePtr, cost);
            }
            else
            {
                //cout << "An edge from nodeID: " << startNodeID << " to nodeID: " << endNodeID << " already exist." << endl;
            }
            
            //            cout << "Line: " << count << " _________" << endl;
            //            cout << "The start node: " << startNodeID << endl;
            //            cout << "The end node: " << endNodeID << endl;
            //            cout << "The cost: " << cost << endl;
            //            cout << "--------------------------------" << endl;
            count++;
        }
    };
    
    // When the graph is delete, delete all pointers owned by graph.
    ~Graph()
    {
        //cout << "The Graph Deconstructor was called." << endl;
        for (auto nodePtr : _graphNodeList)
        {
            nodePtr = nullptr;
            delete nodePtr;
            _totalNodes--;
            
        }
        for (auto edgePtr : _graphEdgeList)
        {
            edgePtr = nullptr;
            delete edgePtr;
            _totalEdges--;
            
        }
        _graphEdgeList.clear();
        _graphEdgeList.clear();
    };
    
    // This is the default copy constructor and currently not implemented.
    Graph(const Graph& graph) = delete;

protected:
    // Private matrix of edge ptrs, not accessible outside graph since adding and removing edge require other maintenance to keep the graph operation at O(1) times.
    vector<vector<Edge<NodeType>*>> _connectedNodesDirectedEdgePtrsMatrix;
    
    // function that return true if the node is valid from it's ID. Operates at O(1).
    bool _isNodeValid(const int nodeID) const
    {
        if (nodeID < _graphNodeList.size())
        {
            return true;
        }
        else
        {
            cout << "Node ID is NOT valid" << endl;
            return false;
        }
    };
    
    // private function to return a random lenght depending on a min and max lenght from random graph creation function.
    double _randomEdgeLenghtBetween(double minLenght, double maxLenght)
    {
        return minLenght + (double)rand() / (double)(RAND_MAX/(maxLenght-minLenght));
    };
    
    // private funtion that return true if the random generated value is within the edge density by random graph creation function.
    bool _createEdgeDependingOn(double edgeDensity)
    {
        double probToCreateEdge = (double)rand() / RAND_MAX;
        if (probToCreateEdge <= edgeDensity)
        {
            return true;
        }
        return false;
    };
    
    // These are private varriable that must be maintained by the graph and therefore cannot be access externally.
    vector<NodeType*> _graphNodeList;
    vector<Edge<NodeType>*> _graphEdgeList;
    int _totalNodes = 0;
    int _totalEdges = 0;
    int _srandSeed;
};


#endif /* Graph_h */
