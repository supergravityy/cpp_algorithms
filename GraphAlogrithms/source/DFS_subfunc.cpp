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

		newWrapper = new typDfsVertex;										// 1. ���ο� ���� ����ü ����
		if (newWrapper == nullptr)
			return false;

		newWrapper->realVertex = get_oldVertex_from_dfsNode(iterNode);		// 2. �����۾�
		set_newVertex_from_dfsNode(iterNode, newWrapper);

		adjSet = get_adjacentSet_from_dfsNode(iterNode);					// 3. ��������Ʈ ���ε� ����
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

		tgtWrapper = (typDfsVertex*)get_oldVertex_from_dfsNode(iterNode);	// 1. ���ε� ���� get
		if (tgtWrapper == nullptr)
			return false;
		else
		{
			set_newVertex_from_dfsNode(iterNode, tgtWrapper->realVertex);	// 2. ���� ����
			delete tgtWrapper;

			adjSet = get_adjacentSet_from_dfsNode(iterNode);
			if (DFS_Unwrapping_adjacentsSet(adjSet) == false)				// 3. ��������Ʈ ���ε� ����
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

	set_vertexClr_from_dfsAdjList(startAdjList, GRAY);								// 6-1. ���ڷ� ���� ��������Ʈ��忡 ȸ�� ĥ�ϱ� (Ž����)

	for (setMember = (typSingleList_Node*)adjSet->begin(); setMember != nullptr;
		setMember = (typSingleList_Node*)adjSet->next(setMember))
	{
		currVertex = (typDfsVertex*)setMember->data;

		nextAdjList = DFS_find_AdjList(wrappedGraph, currVertex, graphCmpFunc);		// 6-2. ��������Ʈ���� �������� �̿��� ��������Ʈ���(���� Ÿ��)�� ȹ��
		if (nextAdjList == nullptr)
		{
#ifdef DEBUG
			debugNumber_dfs = 4;
#endif
			return false;
		}

		if (get_vertexClr_from_dfsAdjList(nextAdjList) == WHITE)					// 6-3. ���� Ÿ���� �ػ�(�湮X)�̸� ���� �������� �� ��� �̵�
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

	set_vertexClr_from_dfsAdjList(startAdjList, BLACK);								// 6-4. startVertex�� ������������ �湮�� ������, ������(Ž���Ϸ�) ĥ�ϱ�
	
	temp = get_realVertex_from_dfsAdjList(startAdjList);
	if (orderedList->push_front(temp) == false)										// 6-5. ���ĸ���Ʈ�� �Ǿտ� ���� (�ش������� ����������� �켱������ ����Ǿ��ϴ� �۾� X)
	{
#ifdef DEBUG
		debugNumber_dfs = 6;
		return false;
#endif
	}
	else
		return true;
}