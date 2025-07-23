#include "../header/graphAlgorithms.h"

/*-------------------------------------*/
// Helper Func Prototypes
/*-------------------------------------*/

void* get_realVertex_from_bfsNode(typSingleList_Node* node);
void* get_oldVertex_from_bfsNode(typSingleList_Node* node);
void set_newVertex_from_bfsNode(typSingleList_Node* node, void* newVertex);
Set* get_adjacentSet_from_bfsNode(typSingleList_Node* node);
typVertexClr get_vertexClr_from_bfsAdjList(typAdjList* adjlist);
void set_vertexClr_from_bfsAdjList(typAdjList* adjlist, typVertexClr newClr);
int get_vertexDist_from_bfsAdjList(typAdjList* adjlist);
void set_vertexDist_from_bfsAdjList(typAdjList* adjlist, int dist);
typVertexClr get_vertexClr_from_bfsNode(typSingleList_Node* node);
void set_vertexClr_from_bfsNode(typSingleList_Node* node, typVertexClr newClr);
void set_vertexDist_from_bfsNode(typSingleList_Node* node, int newDist);

/*---------------------------------------*/
// SubFunc
/*---------------------------------------*/

typAdjList* BFS_find_AdjList(Graph* wrappedGraph, typBfsVertex* tgtVertex,
	typCmpResult(*graphCmpFunc)(const void* k1, const void* k2))
{
	typSingleList_Node* iterNode = nullptr;
	typBfsVertex* nodeVertex = nullptr;
	typAdjList* result = nullptr;

	for (auto iter = wrappedGraph->begin(); iter != nullptr; iter = wrappedGraph->next((void*)iter))
	{
		iterNode = (typSingleList_Node*)iter;

		if (graphCmpFunc(tgtVertex->realVertex, get_realVertex_from_bfsNode(iterNode)) == EQUAL)
		{
			result = (typAdjList*)iterNode->data;
			break;
		}
	}

	return result;
}

bool BFS_Wrapping_adjacentsSet(Set* adjSet)
{
	if (adjSet == nullptr) return false;

	typSingleList_Node* adjMember = nullptr;
	typBfsVertex* wrappedAdj = nullptr;
	void* rawData = nullptr;

	for (adjMember = (typSingleList_Node*)adjSet->begin(); adjMember != nullptr;
		adjMember = (typSingleList_Node*)adjSet->next(adjMember))
	{
		rawData = adjMember->data;

		wrappedAdj = new typBfsVertex;
		if (wrappedAdj == nullptr) return false;

		wrappedAdj->realVertex = rawData;

		adjMember->data = wrappedAdj;
	}
	return true;
}

bool BFS_Unwrapping_adjacentsSet(Set* adjSet)
{
	if (adjSet == nullptr) return false;

	typSingleList_Node* adjMember = nullptr;
	typBfsVertex* wrappedAdj = nullptr;

	for (adjMember = (typSingleList_Node*)adjSet->begin(); adjMember != nullptr;
		adjMember = (typSingleList_Node*)adjSet->next(adjMember))
	{
		 wrappedAdj = (typBfsVertex*)adjMember->data;
		if (wrappedAdj == nullptr) return false;

		adjMember->data = wrappedAdj->realVertex;
		delete wrappedAdj;
	}
	return true;
}

bool BFS_WrappingFunc(Graph* originGraph)
{
	typSingleList_Node* iterNode = nullptr;
	typBfsVertex* newWrapper = nullptr;
	Set* adjSet = nullptr;

	for (auto iter = originGraph->begin(); iter != nullptr; iter = originGraph->next((void*)iter))
	{
		iterNode = (typSingleList_Node*)iter;

		newWrapper = new typBfsVertex;									// 1. 새로운 래핑 구조체 생성
		if (newWrapper == nullptr)
			return false;
		
		newWrapper->realVertex = get_oldVertex_from_bfsNode(iterNode);		// 2. 래핑작업
		set_newVertex_from_bfsNode(iterNode, newWrapper);

		adjSet = get_adjacentSet_from_bfsNode(iterNode);					// 3. 인접리스트 내부도 래핑
		if (BFS_Wrapping_adjacentsSet(adjSet) == false) 
			return false;
	}

	return true;
}

bool BFS_UnwrappingFunc(Graph* originGraph)
{
	typSingleList_Node* iterNode = nullptr;
	typBfsVertex* tgtWrapper = nullptr;
	Set* adjSet = nullptr;

	for (auto iter = originGraph->begin(); iter != nullptr; iter = originGraph->next((void*)iter))
	{
		iterNode = (typSingleList_Node*)iter;

		tgtWrapper = (typBfsVertex*)get_oldVertex_from_bfsNode(iterNode);		// 1. 래핑된 정점 get
		if (tgtWrapper == nullptr)
			return false;
		else
		{
			set_newVertex_from_bfsNode(iterNode, tgtWrapper->realVertex);		// 2. 정점 언래핑
			delete tgtWrapper;

			adjSet = get_adjacentSet_from_bfsNode(iterNode);
			if (BFS_Unwrapping_adjacentsSet(adjSet) == false)				// 3. 인접리스트 내부도 언래핑
				return false;			
		}
	}

	return true;
}

void BFS_initVertex(Graph* wrappedGraph, typBfsVertex* startVertex,
	typCmpResult(*graphCmpFunc)(const void* k1, const void* k2))
{
	typSingleList_Node* iterNode = nullptr;
	typBfsVertex* nodeVertex = nullptr;

	for (auto iter = wrappedGraph->begin(); iter != nullptr; iter = wrappedGraph->next((void*)iter))
	{
		iterNode = (typSingleList_Node*)iter;

		if (graphCmpFunc(startVertex->realVertex, get_realVertex_from_bfsNode(iterNode)) == EQUAL)
		{
			set_vertexClr_from_bfsNode(iterNode, GRAY);
			set_vertexDist_from_bfsNode(iterNode, BFS_INITIAL_DISTANCE);
		}
		else
		{
			set_vertexClr_from_bfsNode(iterNode, WHITE);
			set_vertexDist_from_bfsNode(iterNode, BFS_NOT_VISITED);
		}
	}
}

bool BFS_GatheringVisitedVerticles(Graph* wrappedGraph, List* distList)
{
	typSingleList_Node* iterNode = nullptr;
	typAdjList* vertex = nullptr;
	typBfsVertex* bfsData = nullptr;
	typResultStrct* result = nullptr;

	for (auto iter = wrappedGraph->begin(); iter != nullptr; 
		iter = wrappedGraph->next((void*)iter))				// 1. 그래프의 정점들 순회
	{
		iterNode = (typSingleList_Node*)iter;
		vertex = (typAdjList*)iterNode->data;
		bfsData = (typBfsVertex*)vertex->vertex;

		if (bfsData->dist != BFS_NOT_VISITED)				// 2. 방문되지 않은 정점은 패스
		{
			result = new typResultStrct;					// 3. distList를 위한 구조체 생성 (traverse에서 unwarpping을 하면 다날라감)
			if (result == nullptr)
				return false;
			else
			{
				result->data = bfsData->realVertex;
				result->dist = bfsData->dist;

				if (distList->push_back(result) == false)	// 4. 방문한 정점만 인접리스트노드를 distList에 삽입
				{
					delete result;
					return false;
				}
			}
		}
	}

	return true;
}