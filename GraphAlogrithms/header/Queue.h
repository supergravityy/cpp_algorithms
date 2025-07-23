#pragma once

#include"List.h"
#pragma comment (lib,"lib/singleLinkedList_debug.lib")
// queue

class Queue : public List
{
public:
	/*Management*/
	Queue();
	~Queue();

	bool enqueue(void* data);
	bool dequeue(void** saveData);
	const void* peek();

	/*Utility*/
protected:
	/*Utility-1*/

	/*Accessor*/
private:
	/* Hide */
	using List::set_Data;
	using List::get_Data;
	using List::lookup_Node;
	using List::push_back;
	using List::insert_nextNode;
	using List::remove_nextNode;
};