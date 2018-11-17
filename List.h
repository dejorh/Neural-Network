/*
 * List.h
 *
 *  Created on: Nov 5, 2018
 *      Author: dcejorh42
 */

#ifndef LIST_H_
#define LIST_H_

#include <iostream>

using namespace std;

template<class T>
class List{
private:
	int maxLength;
	T* elements;
	int length;
	
	void resize();
	
public:
	List();
	virtual ~List();
	int size();
	void append(T elem);
	T get(int index);
	
};

template<class T>
List<T>::List(){
	maxLength = 10;
	elements = new T[maxLength];
	length = 0;
}

template<class T>
List<T>::~List(){
	delete [] elements;
}

template<class T>
void List<T>::resize(){
	T* temp = elements;
	maxLength *= 2;
	elements = new T[maxLength];
	for(int i = 0; i < length; ++i){
		elements[i] = temp[i];
	}
	
	delete [] temp;
}

template<class T>	
int List<T>::size(){
	return length;
}
	
template<class T>	
void List<T>::append(T elem){
	if(length == maxLength){
		resize();
	}
	elements[length] = elem;
	length++;	
}
	
template<class T>	
T List<T>::get(int index){
	while((index >= length) || (index < 0)){
		cout << "bad index" <<  "re-enter index value" << endl;
		cin >> index;
	}
	//cout << index << endl;
	return elements[index];
}

#endif /* LIST_H_ */