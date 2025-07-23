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
	typAdjList* currAdjList = nullptr, * nextAdjList = nullptr;
	typBfsVertex* currVertex = nullptr, * nextVertex = nullptr;
	typSingleList_Node* currNode = nullptr, * setMember = nullptr;
	Set* adjacentNode = nullptr;
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
	else if(BFS_WrappingFunc(myGraph) == false)													// 2. ��������Ʈ ��� �����۾�
	{
#ifdef DEBUG
		debugNumber_bfs = 3;
#endif
		// ���ǻ���! : ���� �����۾��� ������, �׷��� ���� ����� �����Լ��� ����ϴ� �޼��带 ���� �ȵ�!!
		return false;
	}
	else {}

	BFS_initVertex(myGraph, startVertex, graphCmpFunc);											// 3. ���ε� �׷����� ��� ���� �ʱ�ȭ
	
	nextAdjList = BFS_find_AdjList(myGraph,startVertex, graphCmpFunc);							// 4. ���������� ��������Ʈ�� �̺�Ʈ ť�� ����
	if (nextAdjList == nullptr)
	{
		BFS_UnwrappingFunc(myGraph);
#ifdef DEBUG
		debugNumber_bfs = 4;
#endif
		return false;
	}
	else if(todoList_Queue.enqueue(nextAdjList) == false)
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
		currAdjList = (typAdjList*)todoList_Queue.peek();									// 5. ť �Ǿ��� ��������Ʈ ��������
		adjacentNode = &(currAdjList->Adjacents);

		for (setMember = (typSingleList_Node*)adjacentNode->begin(); setMember != nullptr;	// 6. ���� ��������Ʈ�� �� �������� ��ȸ
			setMember = (typSingleList_Node*)adjacentNode->next(setMember))
		{
			nextVertex = (typBfsVertex*)setMember->data;									// 6-1. ���� ���տ��� �̿�����(������) �ϳ� ��������

			nextAdjList = BFS_find_AdjList(myGraph, nextVertex, graphCmpFunc);					// 6-2. ���� �̿������� ��������Ʈ ���� ��������
			if (nextAdjList == nullptr)
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
				currVertex = (typBfsVertex*)nextAdjList->vertex;

				if (currVertex->color == WHITE)												// 6-3. ��� ������ ȸ���� ĥ�ϱ�(ť�� ���ٴ� ǥ��)
				{
					currVertex->color = GRAY;
					tempDist = get_vertexDist_from_bfsAdjList(currAdjList);
					currVertex->dist = tempDist + 1;										// 6-4. ������ �������� �Ÿ� +1

					if (todoList_Queue.enqueue(nextAdjList) == false)						// 6-5. �� �������� ��������Ʈ�� enqueue
					{
						todoList_Queue.init(graphCmpFunc, dummyFunc::print, dummyFunc::destroy);
						BFS_UnwrappingFunc(myGraph);
#ifdef DEBUG
						debugNumber_bfs =7;
#endif
						return false;
					}
				}
			}
		}

		if (todoList_Queue.dequeue((void**)&currAdjList) == true)							// 7. ���� ��������Ʈ�� dequeue �Ͽ� ������ ĥ�ϱ� 
		{
			set_vertexClr_from_bfsAdjList(currAdjList, BLACK);	// ť���� �������Դٴ� ǥ��	
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