#include "../header/graphAlgorithms.h"

using namespace GraphAlgorithms;

#ifdef DEBUG
int debugNumber_bfs = 0;
#endif

/*-------------------------------------*/
// Utility Func Prototypes
/*-------------------------------------*/

bool BFS_WrappingFunc(Graph* originGraph);
bool BFS_UnwrappingFunc(Graph* originGraph);
void BFS_initVertex(Graph* wrappedGraph, typBfsVertex* startVertex,
	typCmpResult(*graphCmpFunc)(const void* k1, const void* k2));
typAdjList* BFS_find_AdjList(Graph* wrappedGraph, typBfsVertex* tgtVertex,
	typCmpResult(*graphCmpFunc)(const void* k1, const void* k2));
bool BFS_GatheringVisitedVerticles(Graph* wrappedGraph, List* distList);
int get_vertexDist_from_bfsAdjList(typAdjList* adjlist);
void set_vertexClr_from_bfsAdjList(typAdjList* adjlist, typVertexClr newClr);

/*-------------------------------------*/
// BFS interface
/*-------------------------------------*/

bool GraphAlgorithms::BFS::traverse(Graph* myGraph, typBfsVertex* startVertex,
	typCmpResult(*graphCmpFunc)(const void* k1, const void* k2), List* distList)
{
	Queue todoList_Queue;
	typAdjList* peekedAdjList = nullptr, * neighborAdjList = nullptr, * firstAdjList = nullptr;
	typBfsVertex* neighbor_nodeVertex = nullptr, * neighbor_setVertex = nullptr;
	typSingleList_Node* setMember = nullptr;
	Set* adjacentSet = nullptr;
	int tempDist = 0;

	if (myGraph == nullptr || startVertex == nullptr || distList == nullptr || graphCmpFunc == nullptr)
	{
#ifdef DEBUG
		debugNumber_bfs = 1;
#endif
		return false;
	}
	else if (todoList_Queue.init(graphCmpFunc, dummyFunc::print, dummyFunc::destroy) == false
		|| distList->init(graphCmpFunc, dummyFunc::print, dummyFunc::destroy) == false)			// 1. �˰��� �ʿ��� �ڷᱸ�� �ʱ�ȭ
	{
#ifdef DEBUG
		debugNumber_bfs = 2;
#endif
		return false;
	}
	else if (BFS_WrappingFunc(myGraph) == false)													// 2. ��������Ʈ ��� �����۾�
	{
#ifdef DEBUG
		debugNumber_bfs = 3;
#endif
		// ���ǻ���! : ���� �����۾��� ������, �׷��� ���� ����� �����Լ��� ����ϴ� �޼��带 ���� �ȵ�!!
		return false;
	}
	else {}

	BFS_initVertex(myGraph, startVertex, graphCmpFunc);											// 3. ���ε� �׷����� ��� ���� �ʱ�ȭ

	firstAdjList = BFS_find_AdjList(myGraph, startVertex, graphCmpFunc);							// 4. ���������� ��������Ʈ�� �̺�Ʈ ť�� ����
	if (firstAdjList == nullptr)
	{
		BFS_UnwrappingFunc(myGraph);
#ifdef DEBUG
		debugNumber_bfs = 4;
#endif
		return false;
	}
	else if (todoList_Queue.enqueue(firstAdjList) == false)
	{
		BFS_UnwrappingFunc(myGraph);
#ifdef DEBUG
		debugNumber_bfs = 5;
#endif
		return false;
	}
	else {}

	while (todoList_Queue.getSize() > 0)	// �ʺ�켱Ž�� ����
	{
		peekedAdjList = (typAdjList*)todoList_Queue.peek();									// 5. ť �Ǿ��� ��������Ʈ ��������
		adjacentSet = &(peekedAdjList->Adjacents);

		for (setMember = (typSingleList_Node*)adjacentSet->begin(); setMember != nullptr;	// 6. ���� ��������Ʈ�� �� �������� ��ȸ
			setMember = (typSingleList_Node*)adjacentSet->next(setMember))
		{
			neighbor_setVertex = (typBfsVertex*)setMember->data;									// 6-1. ���� ���տ��� �̿�����(������) �ϳ� ��������

			neighborAdjList = BFS_find_AdjList(myGraph, neighbor_setVertex, graphCmpFunc);					// 6-2. ���� �̿������� ��������Ʈ ���� ��������
			if (neighborAdjList == nullptr)
			{
				todoList_Queue.init(graphCmpFunc, dummyFunc::print, dummyFunc::destroy);
				BFS_UnwrappingFunc(myGraph);
#ifdef DEBUG
				debugNumber_bfs = 6;
#endif
				return false;
			}
			else
			{
				neighbor_nodeVertex = (typBfsVertex*)neighborAdjList->vertex;

				if (neighbor_nodeVertex->color == WHITE)												// 6-3. ��� ������ ȸ���� ĥ�ϱ�(ť�� ���ٴ� ǥ��)
				{
					neighbor_nodeVertex->color = GRAY;
					tempDist = get_vertexDist_from_bfsAdjList(peekedAdjList);
					neighbor_nodeVertex->dist = tempDist + 1;										// 6-4. ������ �������� �Ÿ� +1

					if (todoList_Queue.enqueue(neighborAdjList) == false)						// 6-5. �� �������� ��������Ʈ�� enqueue
					{
						todoList_Queue.init(graphCmpFunc, dummyFunc::print, dummyFunc::destroy);
						BFS_UnwrappingFunc(myGraph);
#ifdef DEBUG
						debugNumber_bfs = 7;
#endif
						return false;
					}
				}
			}
		}

		if (todoList_Queue.dequeue((void**)&peekedAdjList) == true)							// 7. ���� ��������Ʈ�� dequeue �Ͽ� ������ ĥ�ϱ� 
		{
			set_vertexClr_from_bfsAdjList(peekedAdjList, BLACK);	// ť���� �������Դٴ� ǥ��	
		}
		else
		{
			todoList_Queue.init(graphCmpFunc, dummyFunc::print, dummyFunc::destroy);
			BFS_UnwrappingFunc(myGraph);
#ifdef DEBUG
			debugNumber_bfs = 8;
#endif
			return false;
		}
	}

	if (BFS_GatheringVisitedVerticles(myGraph, distList) == false)							// 8. �� ������ �Ÿ��� �Ѱܹޱ�
	{
		distList->init(graphCmpFunc, dummyFunc::print, dummyFunc::destroy);
		BFS_UnwrappingFunc(myGraph);
#ifdef DEBUG
		debugNumber_bfs = 9;
#endif
		return false;
	}

	BFS_UnwrappingFunc(myGraph);															// 9. �׷��� ���󺹱�
	return true;
}