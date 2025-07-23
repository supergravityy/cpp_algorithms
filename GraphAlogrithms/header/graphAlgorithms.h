#pragma once

#include "List.h"
#include "Queue.h"
#include "Set.h"
#include "Graph.h"

#define DEBUG

#pragma comment(lib,"lib/singleLinkedList_debug.lib") 
#pragma comment(lib,"lib/queue_debug.lib")
#pragma comment(lib,"lib/set_debug.lib")
#pragma comment(lib,"lib/graph_debug.lib")

#define BFS_NOT_VISITED			(-1)
#define BFS_INITIAL_DISTANCE	(0)

typedef enum
{
	WHITE,	// Not visited
	GRAY,	// Visited but not fully explored
	BLACK	// Fully explored
}typVertexClr;

typedef struct BfsVertex
{
	void* realVertex;
	typVertexClr color;
	int dist;
}typBfsVertex;

typedef struct DfsVertex
{
	void* realVertex;
	typVertexClr color;
}typDfsVertex;

typedef struct ResultStrct
{
	void* data;  // realVertex
	int dist;
} typResultStrct;

// singleList Node

/*------------------------------------------------
* void* data				->	(void*)typAdjList
* 
* typSingleList_Node* next
--------------------------------------------------*/

// original Node

/*[typAdjList]-----------------------------------------
* void* vertex	->	myData
* 
*                   [typSingleList_Node]---------------
* Set Adjacents	->	void* data				->	myData
*					typSingleList_Node* next
-------------------------------------------------------*/

// wrapped Node (BFS)

/*[typAdjList]------[typBfsVertex]---------------------------------------------
* void* vertex	->	void* realVertex		->	myData
*					typVertexClr color
*					int dist
* 
*                   [typSingleList_Node]--------[typBfsVertex]-----------------
* Set Adjacents	->	void* data				->	void* realVertex	->	myData
*												typVertexClr color
*												int dist
*					typSingleList_Node* next
-------------------------------------------------------------------------------*/

// wrapped Node (DFS)

/*[typAdjList]------[typDfsVertex]--------------------------------------------
* void* vertex	->	void* realVertex	->	myData
*					typVertexClr color
* 
*                   [typSingleList_Node]--------[typDfsVertex]-----------------
* Set Adjacents	->	void* data				->	void* realVertex	->	myData
*												typVertexClr color
*					typSingleList_Node* next
-------------------------------------------------------------------------------*/

namespace GraphAlgorithms
{
	namespace BFS
	{
		bool traverse(Graph* myGraph, typBfsVertex* startVertex, 
			typCmpResult(*graphCmpFunc)(const void* k1, const void* k2), List* distList);
	}
	namespace DFS
	{
		bool traverse(Graph* myGraph,
			typCmpResult(*graphCmpFunc)(const void* k1, const void* k2), List* orderedList);
	}
	namespace dummyFunc
	{
		void print(const void* data);
		void destroy(void* data);
	}
}
#ifdef DEBUG
extern int debugNumber_bfs;
extern int debugNumber_dfs;
#endif // DEBUG