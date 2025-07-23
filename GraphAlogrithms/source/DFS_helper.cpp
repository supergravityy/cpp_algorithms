#include "../header/graphAlgorithms.h"

/*---------------------------------------*/
// Helper
/*---------------------------------------*/

void* get_oldVertex_from_dfsNode(typSingleList_Node* node)
{
	return ((typAdjList*)node->data)->vertex;
}
void set_newVertex_from_dfsNode(typSingleList_Node* node, void* newVertex)
{
	((typAdjList*)node->data)->vertex = newVertex;
}

typVertexClr get_vertexClr_from_dfsNode(typSingleList_Node* node)
{
	return ((typDfsVertex*)((typAdjList*)node->data)->vertex)->color;
}
void set_vertexClr_from_dfsNode(typSingleList_Node* node, typVertexClr newClr)
{
	((typDfsVertex*)((typAdjList*)node->data)->vertex)->color = newClr;
}

typVertexClr get_vertexClr_from_dfsAdjList(typAdjList* adjlist)
{
	return ((typDfsVertex*)adjlist->vertex)->color;
}
void set_vertexClr_from_dfsAdjList(typAdjList* adjlist, typVertexClr newClr)
{
	((typDfsVertex*)adjlist->vertex)->color = newClr;
}

void* get_realVertex_from_dfsNode(typSingleList_Node* node)
{
	return ((typDfsVertex*)((typAdjList*)node->data)->vertex)->realVertex;
}
Set* get_adjacentSet_from_dfsNode(typSingleList_Node* node)
{
	return &(((typAdjList*)node->data)->Adjacents);
}

typDfsVertex* get_newVertex_from_dfsNode(typSingleList_Node* node)
{
	return (typDfsVertex*)((typAdjList*)node->data)->vertex;
}
void* get_realVertex_from_dfsAdjList(typAdjList* adjlist)
{
	return ((typDfsVertex*)adjlist->vertex)->realVertex;
}