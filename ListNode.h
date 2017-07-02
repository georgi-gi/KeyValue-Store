#pragma once

template <class T>
class Node
{
public:
	T val;
	Node<T>* next;
public:
	/*Node()
	{
	next = nullptr;
	}*/
	Node(const Node& o);
	Node(const T& val, Node<T>* next = nullptr)
	{
		this->val = val;
		this->next = next;
	}
};

template<class T>
Node<T>::Node(const Node & o)
{
	this->val = o.val;
	this->next = o.next;
}
