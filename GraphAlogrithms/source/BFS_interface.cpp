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
		|| distList->init(graphCmpFunc, dummyFunc::print, dummyFunc::destroy) == false)			// 1. 알고리즘에 필요한 자료구조 초기화
	{
#ifdef DEBUG
		debugNumber_bfs = 2;
#endif
		return false;
	}
	else if(BFS_WrappingFunc(myGraph) == false)													// 2. 인접리스트 노드 래핑작업
	{
#ifdef DEBUG
		debugNumber_bfs = 3;
#endif
		// 주의사항! : 정점 래핑작업이 끝나면, 그래프 내의 사용자 지정함수를 사용하는 메서드를 쓰면 안됨!!
		return false;
	}
	else {}

	BFS_initVertex(myGraph, startVertex, graphCmpFunc);											// 3. 래핑된 그래프의 모든 정점 초기화
	
	nextAdjList = BFS_find_AdjList(myGraph,startVertex, graphCmpFunc);							// 4. 시작정점의 인접리스트를 이벤트 큐에 삽입
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

	while (todoList_Queue.getSize() > 0)	// 너비우선탐색 수행
	{
		currAdjList = (typAdjList*)todoList_Queue.peek();									// 5. 큐 맨앞의 인접리스트 가져오기
		adjacentNode = &(currAdjList->Adjacents);

		for (setMember = (typSingleList_Node*)adjacentNode->begin(); setMember != nullptr;	// 6. 현재 인접리스트의 각 인접정점 순회
			setMember = (typSingleList_Node*)adjacentNode->next(setMember))
		{
			nextVertex = (typBfsVertex*)setMember->data;									// 6-1. 엣지 집합에서 이웃정점(목적지) 하나 가져오기

			nextAdjList = BFS_find_AdjList(myGraph, nextVertex, graphCmpFunc);					// 6-2. 위의 이웃정점의 인접리스트 집합 가져오기
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

				if (currVertex->color == WHITE)												// 6-3. 흰색 정점에 회색을 칠하기(큐에 들어간다는 표식)
				{
					currVertex->color = GRAY;
					tempDist = get_vertexDist_from_bfsAdjList(currAdjList);
					currVertex->dist = tempDist + 1;										// 6-4. 루프가 돌때마다 거리 +1

					if (todoList_Queue.enqueue(nextAdjList) == false)						// 6-5. 그 정점들의 인접리스트를 enqueue
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

		if (todoList_Queue.dequeue((void**)&currAdjList) == true)							// 7. 현재 인접리스트를 dequeue 하여 검정색 칠하기 
		{
			set_vertexClr_from_bfsAdjList(currAdjList, BLACK);	// 큐에서 빠져나왔다는 표식	
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

	if (BFS_GatheringVisitedVerticles(myGraph, distList) == false)							// 8. 각 정점의 거리를 넘겨받기
	{
		distList->init(graphCmpFunc, dummyFunc::print, dummyFunc::destroy);
		BFS_UnwrappingFunc(myGraph);
#ifdef DEBUG
		debugNumber_bfs = 9;
#endif
		return false;
	}

	BFS_UnwrappingFunc(myGraph);															// 9. 그래프 원상복구
	return true;
}