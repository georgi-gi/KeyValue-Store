#pragma once
#include <iostream>
#include <exception>
#include "ListNode.h"
#include "Iterator.h"

template<class T>
class List
{
private:
	Node<T>* first;
	Node<T>* last;
	size_t m_size;
public:
	List() : m_size(0), first(nullptr), last(nullptr) {};
	List(const List& o);
	List& operator= (const List& o);
	~List();
public:
	void add(const T& obj);
	void remove();	//removes last
	void removeAt(size_t ind);
	T& getTail();
	T& getHead();
	T& getAt(size_t ind);
	void insertAfter(size_t index, T& val);
	size_t getSize();

	Iterator<T> getIterator() const;

private:
	Node<T>* copyChain(Node<T>* first);
	void removeChain(Node<T>* first);
};




template<class T>
List<T>::List(const List & o)
{
	this->first = nullptr;
	this->last = nullptr;
	this->m_size = 0;

	if (o.getSize() == 0)
		return;
	else
	{
		Node<T>* newChain = copyChain(o.first);

		if (newChain)
		{
			this->last->next = newChain;

			Node<T>* current = newChain;
			if (current)
			{
				while (current->next)
				{
					current = current->next;
				}
			}

			this->last = current;
		}
	}
}

template<class T>
List<T> & List<T>::operator=(const List & o)
{
	if (this != &o)
	{
		while (m_size != 0)
		{
			remove();
		}

		if (o.m_size == 0)
			return *this;
		else
		{
			Node<T>* newChain = copyChain(o.first);

			if (newChain)
			{
				this->last->next = newChain;

				Node<T>* current = newChain;
				if (current)
				{
					while (current->next)
					{
						current = current->next;
					}
				}

				this->last = current;
			}
		}
	}

	return *this;
}

template<class T>
List<T>::~List()
{
	while (m_size != 0)
	{
		remove();
	}
}

template<class T>
void List<T>::add(const T & obj)
{
	if (!first)
	{
		first = new Node<T>(obj, nullptr);
		last = first;
		++m_size;
		return;
	}

	Node<T>* n = new Node<T>(obj, nullptr);
	last->next = n;
	last = n;
	++m_size;
}

template<class T>
void List<T>::remove()
{
	if (!last)
	{
		throw std::out_of_range("Empty list. Can't pop.");
	}

	if (first == last)
	{
		delete first;
		first = nullptr;
		last = nullptr;
		--m_size;
		return;
	}

	Node<T>* current = first;
	while (current->next != last)
	{
		current = current->next;
	}
	delete last;
	current->next = nullptr;
	last = current;
	--m_size;
}

template<class T>
void List<T>::removeAt(size_t ind)
{
	if (ind == 0)
	{
		Node<T>* newFirst = first->next;
		delete first;
		first = newFirst;
		--m_size;
		return;
	}

	else if (ind == m_size - 1)
	{
		this->remove();
		return;
	}

	Node<T>* current = first;
	for (int i = 0; i < ind - 1; ++i)
	{
		current = current->next;
	}
	
	Node<T>* newNext = current->next->next;
	delete current->next;
	current->next = newNext;
	--m_size;
	return;
}

template<class T>
T & List<T>::getTail()
{
	/*if (!last)
	{
		return nullptr;
	}*/
	return last->val;
}

template<class T>
T & List<T>::getHead()
{
	/*if (!first)
	{
		return nullptr;
	}*/
	return first->val;
}

template<class T>
T & List<T>::getAt(size_t ind)
{
	Node<T>* current = first;

	for (size_t i = 0; i < ind; ++i)
	{
		current = current->next;
	}

	return current->val;
}

template<class T>
void List<T>::insertAfter(size_t index, T & val)
{
	Node<T>* preceding = first;
	for (size_t i = 0; i < index; ++i)
	{
		preceding = preceding->next;
	}

	Node<T>* newElement = new Node<T>(val, preceding->next);
	preceding->next = newElement;
	++m_size;
}

template<class T>
size_t List<T>::getSize()
{
	return m_size;
}

template<class T>
Iterator<T> List<T>::getIterator() const
{
	return Iterator<T>(this->first);
}

template<class T>
Node<T>* List<T>::copyChain(Node<T>* first)
{
	if (first)
	{
		//start of the new chain
		Node<T>* newChain = new Node(first);

		Node<T>* readFrom = first->next;
		Node<T>* writeAfter = newChain;

		while (readFrom)
		{
			if (!writeAfter)
			{
				removeChain(newChain);
				newChain = nullptr;
				break;
			}

			writeAfter->next = new Node(readFrom);
			writeAfter = writeAfter->next;
			readFrom = readFrom->next;
		}

		return newChain;
	}
	return nullptr;

}

template<class T>
void List<T>::removeChain(Node<T>* first)
{
	Node<T>* current = first;
	Node<T>* old = first;

	while (current)
	{
		old = current;
		current = current->next;
		delete old;
	}
}


