#ifndef DOUBLE_LINK_HPP
#define DOUBLE_LINK_HPP

#include <iostream>
using namespace std;

template<class T>
struct Node {
	public:
		T value;
		Node* prev;
		Node* next;
	public:
		Node() {}
		Node(T value, Node* prev, Node* next) {
			this->value = value;
			this->prev = prev;
			this->next = next;
		}
};

template<class T>
class DoubleLink {
	public:
		DoubleLink();
		~DoubleLink();
		
		int size();
		int isEmpty();
		
		T get(int index);
		T getFirst();
		T getLast();
		
		int insert(int index, T value);
		int insertFirst(T value);
		int insertLast(T value);
		
		int remove(int index);
		int removeFirst();
		int removeLast();
		
	private:
		int count;
		Node<T>* head;
	private:
		Node<T>* getNode(int index);
};

template<class T>
DoubleLink<T>::~DoubleLink() {
	Node<T>* temp;
	Node<T>* curr = head->next;
	while(curr != head) {
		temp = curr;
		curr = curr->next();
		delete temp;
	}
	delete head;
	head = NULL;
}

template<class T>
int DoubleLink<T>::size() {
	return count;
}

template<class T>
int DoubleLink<T>::isEmpty() {
	return count==0;
}

template<class T>
Node<T>* DoubleLink<T>::getNode(int index) {
	if(index<0 || index>=count) {
		cout << "get node fail. the index in out of bound." << endl;
		return NULL;
	}
	// 正向查找
	if(index <= count/2) {
		int i=0;
		Node<T>* curr = head->next;
		while(i++ < index) {
			curr = curr->next;
		}
		return curr;
	}
	// 反向查找
	int j=0;
	int temp = count - index -1;
	Node<T>* curr = head->prev;
	while(j++ < temp) {
		curr = curr->prev;
	}
	return curr;
}

template<class T>
T DoubleLink<T>::get(int index) {
	return getNode(index)->value;
}

template<class T>
T DoubleLink<T>::getFirst() {
	return getNode(0)->value;
}

template<class T>
T DoubleLink<T>::getLast() {
	return getNode(count-1)->value;
}

#endif