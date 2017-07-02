#pragma once
#include <iostream>
#include "ListNode.h"

template <class T>
class Iterator
{
private:
	Node<T>* begin;
	Node<T>* current;
public:
	Iterator(Node<T>* begin);
public:
	T& getCurrent() const;
	//	void setCurrent(T const& o);
	void moveNext();
	void setToStart();
	bool isEnd() const;
};


template<class T>
Iterator<T>::Iterator(Node<T>* begin)
{
	this->begin = begin;
	this->current = begin;
}

template<class T>
T & Iterator<T>::getCurrent() const
{
	return current->val;
}

template<class T>
void Iterator<T>::moveNext()
{
	if (current)
		current = current->next;
}

template<class T>
void Iterator<T>::setToStart()
{
	current = begin;
}

template<class T>
bool Iterator<T>::isEnd() const
{
	return current == nullptr;
}