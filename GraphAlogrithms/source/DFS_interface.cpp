#include "../header/graphAlgorithms.h"

using namespace GraphAlgorithms;

#ifdef DEBUG
int debugNumber_dfs = 0;
#endif

/*-------------------------------------*/
// Helper Func Prototypes
/*-------------------------------------*/

typVertexClr get_vertexClr_from_dfsNode(typSingleList_Node* node);
bool DFS_UnwrappingFunc(Graph* originGraph);
bool DFS_WrappingFunc(Graph* originGraph);
void DFS_initVertex(Graph* wrappedGraph);
bool DFS_recursiveTraverse(Graph* wrappedGraph, AdjList* startAdjList, List* orderedList,
	typCmpResult(*graphCmpFunc)(const void* k1, const void* k2));

/*-------------------------------------*/
// DFS interface
/*-------------------------------------*/

bool GraphAlgorithms::DFS::traverse(Graph* myGraph,
	typCmpResult(*graphCmpFunc)(const void* k1, const void* k2), List* orderedList)
{
	typSingleList_Node* iterNode = nullptr;

	if (myGraph == nullptr || graphCmpFunc == nullptr || orderedList == nullptr)
	{
#ifdef DEBUG
		debugNumber_dfs = 1;
#endif
		return false;
	}
	else if(orderedList->init(graphCmpFunc, dummyFunc::print, dummyFunc::destroy) == false)	// 1. �˰��� �ʿ��� �ڷᱸ�� �ʱ�ȭ
	{
#ifdef DEBUG
		debugNumber_dfs = 2;
#endif
		return false;
	}
	else if(DFS_WrappingFunc(myGraph) == false)												// 2. ��������Ʈ ��� �����۾�
	{
#ifdef DEBUG
		debugNumber_dfs = 3;
#endif
		return false;
	}
	else {}

	DFS_initVertex(myGraph);																// 3. ���ε� �׷����� ��� ���� �ʱ�ȭ

	// ���̿켱Ž��
	for (auto iter = myGraph->begin(); iter != nullptr; iter = myGraph->next((void*)iter))	// 4. ��� ���(����)���� ��ȸ
	{
		iterNode = (typSingleList_Node*)iter;

		if (get_vertexClr_from_dfsNode(iterNode) == WHITE)									// 5. �ƿ� ������ �ȵ� ������ �������� �ѱ� 
		{
			if (DFS_recursiveTraverse(myGraph, (AdjList*)iterNode->data, 
				orderedList, graphCmpFunc) == false)
			{
				orderedList->init(graphCmpFunc, dummyFunc::print, dummyFunc::destroy);
				DFS_UnwrappingFunc(myGraph);
				return false;
			}
		}
	}

	DFS_UnwrappingFunc(myGraph);															// 7. �׷��� ���󺹱�
	return true;
}