#pragma once

template<class T> struct listNode
{
	T value;
	listNode<T> *next;
};