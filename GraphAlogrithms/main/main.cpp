#include "../header/Graph.h"
#include "../header/graphAlgorithms.h"

#pragma comment(lib,"lib/graph_debug.lib")
#pragma comment(lib,"lib/graphAlgorithms.lib")

using namespace GraphAlgorithms;

void bfs_undirectedGraph_test(Graph& g);
void bfs_directedGraph_test(Graph& g);
void dfs_directedGraph_test(Graph& g);
void dfs_undirectedGraph_test(Graph& g);

typCmpResult compareInt(const void* a, const void* b)
{
	int aVal = *(int*)a;
	int bVal = *(int*)b;

	if (aVal < bVal) return LESS;
	else if (aVal > bVal) return GREATER;
	else return EQUAL;
}

void printInt(const void* data)
{
	std::cout << "[" << *(int*)data << "] ";
}

void destroyInt(void* data)
{
	delete (int*)data;
}

int main()
{
	Graph g;
	g.init(compareInt, destroyInt, printInt, UNDIRECTED);

	bfs_undirectedGraph_test(g);
	bfs_directedGraph_test(g);
	dfs_undirectedGraph_test(g);
	dfs_directedGraph_test(g);

	return 0;
}

void bfs_undirectedGraph_test(Graph& g)
{
	// 정점 생성
	int* v0 = new int(0);
	int* v1 = new int(1);
	int* v2 = new int(2);
	int* v3 = new int(3);
	int* v4 = new int(4);

	g.insert_vertex(v0);
	g.insert_vertex(v1);
	g.insert_vertex(v2);
	g.insert_vertex(v3);
	g.insert_vertex(v4);

	// 엣지 삽입 (무방향)
	g.insert_edge(v0, v1);
	g.insert_edge(v0, v2);
	g.insert_edge(v1, v3);
	g.insert_edge(v2, v4);

	// 시작 정점 래핑
	typBfsVertex start;
	start.realVertex = v0;

	List distList;
	bool result = BFS::traverse(&g, &start, compareInt, &distList);

	std::cout << "\n=== BFS Traversal Result (unDirected) ===" << std::endl;

	if (result)
	{
		for (auto iter = distList.begin(); iter != nullptr; iter = distList.next(iter))
		{
			typResultStrct* res = (typResultStrct*)distList.data(iter);

			std::cout << "Vertex: ";
			printInt(res->data);
			std::cout << " | Dist: " << res->dist << std::endl;
		}
	}
	else
	{
#ifdef DEBUG
		std::cout << "BFS traversal failed. : " << debugNumber_bfs << std::endl;
#else
		std::cout << "BFS traversal failed. : " << std::endl;
#endif
	}
}

void bfs_directedGraph_test(Graph& g)
{
	g.init(compareInt, destroyInt, printInt, DIRECTED);

	// 정점 생성
	int* v0 = new int(0);
	int* v1 = new int(1);
	int* v2 = new int(2);
	int* v3 = new int(3);
	int* v4 = new int(4);
	int* v5 = new int(5);

	g.insert_vertex(v0);
	g.insert_vertex(v1);
	g.insert_vertex(v2);
	g.insert_vertex(v3);
	g.insert_vertex(v4);
	g.insert_vertex(v5);

	// 방향 엣지 삽입
	g.insert_edge(v0, v1);
	g.insert_edge(v0, v2);
	g.insert_edge(v1, v3);
	g.insert_edge(v1, v2);
	g.insert_edge(v2, v4);
	g.insert_edge(v4, v5);

	// 시작 정점 래핑
	typBfsVertex start;
	start.realVertex = v0;

	List distList;
	bool result = BFS::traverse(&g, &start, compareInt, &distList);

	std::cout << "\n=== BFS Traversal Result (Directed) ===" << std::endl;

	if (result)
	{
		for (auto iter = distList.begin(); iter != nullptr; iter = distList.next(iter))
		{
			typResultStrct* res = (typResultStrct*)distList.data(iter);

			std::cout << "Vertex: ";
			printInt(res->data);
			std::cout << " | Dist: " << res->dist << std::endl;
		}
	}
	else
	{
#ifdef DEBUG
		std::cout << "BFS traversal failed. : " << debugNumber_bfs << std::endl;
#else
		std::cout << "BFS traversal failed. : " << std::endl;
#endif
	}
}

void dfs_undirectedGraph_test(Graph& g)
{
	g.init(compareInt, destroyInt, printInt, UNDIRECTED);

	// 정점 생성
	int* v0 = new int(0);
	int* v1 = new int(1);
	int* v2 = new int(2);
	int* v3 = new int(3);

	g.insert_vertex(v0);
	g.insert_vertex(v1);
	g.insert_vertex(v2);
	g.insert_vertex(v3);

	// 엣지 삽입 (무방향)
	g.insert_edge(v0, v1);
	g.insert_edge(v0, v2);
	g.insert_edge(v1, v3);

	List orderedList;
	bool result = DFS::traverse(&g, compareInt , &orderedList);

	std::cout << "\n=== DFS Traversal Result (unDirected) ===" << std::endl;

	if (result)
	{
		cout << "Result Order (NOT Topological Order):" << endl;
		for (auto it = orderedList.begin(); it != nullptr; it = orderedList.next(it))
		{
			printInt(orderedList.data(it));
			cout << " ";
		}
		cout << endl;
	}
	else
	{
#ifdef DEBUG
		std::cout << "DFS traversal failed. : " << debugNumber_dfs << std::endl;
#else
		std::cout << "DFS traversal failed. : " << std::endl;
#endif
	}
}

void dfs_directedGraph_test(Graph& g)
{
	g.init(compareInt, destroyInt, printInt, DIRECTED);

	// 정점 생성
	int* v0 = new int(0);
	int* v1 = new int(1);
	int* v2 = new int(2);
	int* v3 = new int(3);
	int* v4 = new int(4);
	int* v5 = new int(5);

	g.insert_vertex(v0);
	g.insert_vertex(v1);
	g.insert_vertex(v2);
	g.insert_vertex(v3);
	g.insert_vertex(v4);
	g.insert_vertex(v5);

	// 엣지 삽입 (방향성)
	g.insert_edge(v5, v2);
	g.insert_edge(v5, v0);
	g.insert_edge(v4, v0);
	g.insert_edge(v4, v1);
	g.insert_edge(v2, v3);
	g.insert_edge(v3, v1);

	List orderedList;
	bool result = DFS::traverse(&g, compareInt, &orderedList);

	std::cout << "\n=== DFS Traversal Result (Directed) ===" << std::endl;

	if (result)
	{
		cout << "Result Order (NOT Topological Order):" << endl;
		for (auto it = orderedList.begin(); it != nullptr; it = orderedList.next(it))
		{
			printInt(orderedList.data(it));
			cout << " ";
		}
		cout << endl;
	}
	else
	{
#ifdef DEBUG
		std::cout << "DFS traversal failed. : " << debugNumber_dfs << std::endl;
#else
		std::cout << "DFS traversal failed. : " << std::endl;
#endif
	}
}