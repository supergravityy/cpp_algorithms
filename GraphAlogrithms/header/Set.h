#pragma once

#include "List.h"
#pragma comment (lib,"lib/singleLinkedList_debug.lib")
// set

class Set : public List
{
public:
	/*Management*/
	Set();
	~Set();
	bool init(typCmpResult(*compareFunc)(const void* key1, const void* key2)
		, void (*printFunc)(const void* data)
		, void (*destroyDataFunc)(void* data)
		, void* (*copyDataFunc)(void* data));
	bool insert(void* data);
	bool remove(void* data, void** saveData);

	/*Operator OverLoad*/
	Set* operator|(Set& otherSet);
	Set* operator&(Set& otherSet);
	Set* operator-(Set& otherSet);
	Set* operator^(Set& otherSet);
	bool operator<(Set& otherSet);
	bool operator>(Set& otherSet);
	bool operator==(Set& otherSet);
	void operator=(Set& otherSet);  // 연속대입하지 말것 (내부적으로 초기화과정 이루어짐)
	
	/*Utility*/
	bool isMember(void* data);

protected:
	/*Operation*/
	Set* unionOpr(Set* otherSet);			// set1 | set2
	Set* intersectOpr(Set* otherSet);		// set1 & set2
	Set* diffOpr(Set* otherSet);			// set1 - set2
	Set* symmDiffOpr(Set* otherSet);		// set1 ^ set2

	/*Utility-1*/
	bool isSubset_1(Set* otherSet);			// set1 ( set2
	bool isSubset_2(Set* otherSet);			// set1 ) set2
	bool isEqual(Set* otherSet);			// set1 == set2
	void deepCopySet(Set* otherSet);		// set1 = set2
	virtual void* makeInst();
	virtual void freeInst(Set* tgtInst);
	virtual void* deepCopyData(void* srcData);

	/*Variable*/
	void* (*copyDataFunc)(void* srcData);

private:
	/*Hide*/
	using List::insert_nextNode;
	using List::remove_nextNode;
	using List::push_back;
	using List::push_front;
	using List::set_Data;
	using List::get_Data;
	using List::init;
};
