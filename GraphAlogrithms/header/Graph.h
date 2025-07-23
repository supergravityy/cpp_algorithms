#pragma once

#include "List.h"
#pragma comment (lib,"lib/singleLinkedList_debug.lib")
#include "Set.h"
#pragma comment (lib,"lib/set_debug.lib")
#include "graphNode.h"

class Graph
{
public:
	/*Management*/
	Graph();
	~Graph();

	bool init(typCmpResult(*compareFunc)(const void* key1, const void* key2), void (*destroyDataFunc)(void* data), 
		void (*printDataFunc)(const void* data), typGraphType type);
	bool insert_vertex(void* data);
	bool insert_edge(void* srcData, void* destData);
	bool remove_vertex(void** saveData);
	bool remove_edge(void** srcData, void** destData);
	
	bool clear_allEdges_of(void* data); 
	bool clear_NodesEdges_of(void* data);
	bool safeRemove_vertex(void** saveData);
	void printGraph();

	/*Utility*/
	int get_vertexCnt();
	int get_edgeCnt();
	void* find_vertex(void** saveData);
	typGraphType get_type();
	typErrcode get_errCode();

	/*Accessor*/
	const void* begin();
	const void* end();
	const void* next(void* node);
	const void* data(void* node);
	
protected:
	/*Utility-1*/
	bool is_Adjacent(void* srcData, void* destData);	// srcData가 destData를 인접노드로 가지고 있는지 확인하는 api
	bool is_vertexReferenced(void* data);
	void destroy();
	
	void* make_node(void* data);
	void init_node(typAdjList* node, void* data);
	void* delete_node(typAdjList* node);
	void clearAdjacents(typAdjList* node);

	/*Accessor-1*/
	List* get_adjListsAddr();							// 연결리스트의 주소를 반환하는 api
	typAdjList* get_adjListNode(void* data);			// 인접노드 리스트의 주소를 반환하는 api
	singleList_Node* get_prevNodeOf(void* data);		// 이전 리스트 주소를 반환하는 api
	void* get_nodeVertex(void* node);					// vertex에 삽입된 데이터 주소반환 api
	void set_nodeVertex(typAdjList* node, void* data);	// vertex에 삽입된 데이터 주소설정 api
	
	/*Variables*/
	typGraphType type;
	int vertexCnt;
	int edgeCnt;
	typErrcode errCode;
	typCmpResult(*compareFunc)(const void* key1, const void* key2);
	void (*destroyDataFunc)(void* data);
	void (*printDataFunc)(const void* data);
	
private:
	/*Privates*/
	List adjLists;
};