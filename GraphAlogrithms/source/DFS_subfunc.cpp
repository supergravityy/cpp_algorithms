#include "../header/graphAlgorithms.h"

/*-------------------------------------*/
// Helper Func Prototypes
/*-------------------------------------*/

void* get_oldVertex_from_dfsNode(typSingleList_Node* node);
void set_newVertex_from_dfsNode(typSingleList_Node* node, void* newVertex);
typVertexClr get_vertexClr_from_dfsNode(typSingleList_Node* node);
void set_vertexClr_from_dfsNode(typSingleList_Node* node, typVertexClr newClr);
typVertexClr get_vertexClr_from_dfsAdjList(typAdjList* adjlist);
void set_vertexClr_from_dfsAdjList(typAdjList* adjlist, typVertexClr newClr);
void* get_realVertex_from_dfsNode(typSingleList_Node* node);
Set* get_adjacentSet_from_dfsNode(typSingleList_Node* node);
void* get_realVertex_from_dfsAdjList(typAdjList* adjlist);

/*---------------------------------------*/
// SubFunc
/*---------------------------------------*/

typAdjList* DFS_find_AdjList(Graph* wrappedGraph, typDfsVertex* tgtVertex,
	typCmpResult(*graphCmpFunc)(const void* k1, const void* k2))
{
	typSingleList_Node* iterNode = nullptr;
	typDfsVertex* nodeVertex = nullptr;
	typAdjList* result = nullptr;

	for (auto iter = wrappedGraph->begin(); iter != nullptr; iter = wrappedGraph->next((void*)iter))
	{
		iterNode = (typSingleList_Node*)iter;

		if (graphCmpFunc(tgtVertex->realVertex, get_realVertex_from_dfsNode(iterNode)) == EQUAL)
		{
			result = (typAdjList*)iterNode->data;
			break;
		}
	}

	return result;
}

bool DFS_Wrapping_adjacentsSet(Set* adjSet)
{
	if (adjSet == nullptr) return false;

	typSingleList_Node* adjMember = nullptr;
	typDfsVertex* wrappedAdj = nullptr;
	void* rawData = nullptr;

	for (adjMember = (typSingleList_Node*)adjSet->begin(); adjMember != nullptr;
		adjMember = (typSingleList_Node*)adjSet->next(adjMember))
	{
		rawData = adjMember->data;

		wrappedAdj = new typDfsVertex;
		if (wrappedAdj == nullptr) return false;

		wrappedAdj->realVertex = rawData;

		adjMember->data = wrappedAdj;
	}
	return true;
}

bool DFS_Unwrapping_adjacentsSet(Set* adjSet)
{
	if (adjSet == nullptr) return false;

	typSingleList_Node* adjMember = nullptr;
	typDfsVertex* wrappedAdj = nullptr;

	for (adjMember = (typSingleList_Node*)adjSet->begin(); adjMember != nullptr;
		adjMember = (typSingleList_Node*)adjSet->next(adjMember))
	{
		wrappedAdj = (typDfsVertex*)adjMember->data;
		if (wrappedAdj == nullptr) return false;

		adjMember->data = wrappedAdj->realVertex;
		delete wrappedAdj;
	}
	return true;
}

bool DFS_WrappingFunc(Graph* originGraph)
{
	typSingleList_Node* iterNode = nullptr;
	typDfsVertex* newWrapper = nullptr;
	Set* adjSet = nullptr;

	for (auto iter = originGraph->begin(); iter != nullptr; iter = originGraph->next((void*)iter))
	{
		iterNode = (typSingleList_Node*)iter;

		newWrapper = new typDfsVertex;										// 1. 새로운 래핑 구조체 생성
		if (newWrapper == nullptr)
			return false;

		newWrapper->realVertex = get_oldVertex_from_dfsNode(iterNode);		// 2. 래핑작업
		set_newVertex_from_dfsNode(iterNode, newWrapper);

		adjSet = get_adjacentSet_from_dfsNode(iterNode);					// 3. 인접리스트 내부도 래핑
		if (DFS_Wrapping_adjacentsSet(adjSet) == false)
			return false;
	}

	return true;
}

bool DFS_UnwrappingFunc(Graph* originGraph)
{
	typSingleList_Node* iterNode = nullptr;
	typDfsVertex* tgtWrapper = nullptr;
	Set* adjSet = nullptr;

	for (auto iter = originGraph->begin(); iter != nullptr; iter = originGraph->next((void*)iter))
	{
		iterNode = (typSingleList_Node*)iter;

		tgtWrapper = (typDfsVertex*)get_oldVertex_from_dfsNode(iterNode);	// 1. 래핑된 정점 get
		if (tgtWrapper == nullptr)
			return false;
		else
		{
			set_newVertex_from_dfsNode(iterNode, tgtWrapper->realVertex);	// 2. 정점 언래핑
			delete tgtWrapper;

			adjSet = get_adjacentSet_from_dfsNode(iterNode);
			if (DFS_Unwrapping_adjacentsSet(adjSet) == false)				// 3. 인접리스트 내부도 언래핑
				return false;
		}
	}

	return true;
}

void DFS_initVertex(Graph* wrappedGraph)
{
	typSingleList_Node* iterNode = nullptr;
	for (auto iter = wrappedGraph->begin(); iter != nullptr; iter = wrappedGraph->next((void*)iter))
	{
		iterNode = (typSingleList_Node*)iter;
		set_vertexClr_from_dfsNode(iterNode, WHITE);
	}
}

bool DFS_recursiveTraverse(Graph* wrappedGraph, AdjList* startAdjList, List* orderedList,
	typCmpResult(*graphCmpFunc)(const void* k1, const void* k2))
{
	AdjList* nextAdjList = nullptr;
	typDfsVertex* currVertex = nullptr;
	typSingleList_Node* setMember = nullptr;
	Set* adjSet = &startAdjList->Adjacents;
	void* temp = nullptr;

	set_vertexClr_from_dfsAdjList(startAdjList, GRAY);								// 6-1. 인자로 받은 인접리스트노드에 회색 칠하기 (탐색중)

	for (setMember = (typSingleList_Node*)adjSet->begin(); setMember != nullptr;
		setMember = (typSingleList_Node*)adjSet->next(setMember))
	{
		currVertex = (typDfsVertex*)setMember->data;

		nextAdjList = DFS_find_AdjList(wrappedGraph, currVertex, graphCmpFunc);		// 6-2. 인접리스트안의 목적지를 이용해 인접리스트노드(다음 타겟)를 획득
		if (nextAdjList == nullptr)
		{
#ifdef DEBUG
			debugNumber_dfs = 4;
#endif
			return false;
		}

		if (get_vertexClr_from_dfsAdjList(nextAdjList) == WHITE)					// 6-3. 다음 타겟이 휜색(방문X)이면 다음 정점으로 더 깊게 이동
		{
			if (DFS_recursiveTraverse(wrappedGraph, nextAdjList,
				orderedList, graphCmpFunc) == false)
			{
#ifdef DEBUG
				debugNumber_dfs = 5;
#endif
				return false;
			}
		}
	}

	set_vertexClr_from_dfsAdjList(startAdjList, BLACK);								// 6-4. startVertex의 인접정점들의 방문이 끝나면, 검정색(탐색완료) 칠하기
	
	temp = get_realVertex_from_dfsAdjList(startAdjList);
	if (orderedList->push_front(temp) == false)										// 6-5. 정렬리스트의 맨앞에 삽입 (해당정점은 현재기준으로 우선적으로 수행되야하는 작업 X)
	{
#ifdef DEBUG
		debugNumber_dfs = 6;
		return false;
#endif
	}
	else
		return true;
}