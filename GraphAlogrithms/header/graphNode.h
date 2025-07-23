#pragma once

#include "Set.h"

typedef struct AdjList
{
	void* vertex;
	Set Adjacents;
}typAdjList;

typedef enum graphType
{
	DIRECTED,
	UNDIRECTED
}typGraphType;