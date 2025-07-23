#include "../header/graphAlgorithms.h"

/*---------------------------------------*/
// Helper
/*---------------------------------------*/

void* get_oldVertex_from_bfsNode(typSingleList_Node* node)
{
	return ((typAdjList*)node->data)->vertex;
}
void set_newVertex_from_bfsNode(typSingleList_Node* node, void* newVertex)
{
	((typAdjList*)node->data)->vertex = newVertex;
}

typVertexClr get_vertexClr_from_bfsNode(typSingleList_Node* node)
{
	return ((typBfsVertex*)((typAdjList*)node->data)->vertex)->color;
}
void set_vertexClr_from_bfsNode(typSingleList_Node* node, typVertexClr newClr)
{
	((typBfsVertex*)((typAdjList*)node->data)->vertex)->color = newClr;
}

int get_vertexDist_from_bfsNode(typSingleList_Node* node)
{
	return ((typBfsVertex*)((typAdjList*)node->data)->vertex)->dist;
}
void set_vertexDist_from_bfsNode(typSingleList_Node* node, int newDist)
{
	((typBfsVertex*)((typAdjList*)node->data)->vertex)->dist = newDist;
}

typVertexClr get_vertexClr_from_bfsAdjList(typAdjList* adjlist)
{
	return ((typBfsVertex*)adjlist->vertex)->color;
}
void set_vertexClr_from_bfsAdjList(typAdjList* adjlist, typVertexClr newClr)
{
	((typBfsVertex*)adjlist->vertex)->color = newClr;
}

int get_vertexDist_from_bfsAdjList(typAdjList* adjlist)
{
	return ((typBfsVertex*)adjlist->vertex)->dist;
}
void set_vertexDist_from_bfsAdjList(typAdjList* adjlist, int dist)
{
	((typBfsVertex*)adjlist->vertex)->dist = dist;
}

void* get_realVertex_from_bfsNode(typSingleList_Node* node)
{
	return ((typBfsVertex*)((typAdjList*)node->data)->vertex)->realVertex;
}
Set* get_adjacentSet_from_bfsNode(typSingleList_Node* node)
{
	return &(((typAdjList*)node->data)->Adjacents);
}