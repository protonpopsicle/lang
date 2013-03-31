#ifndef STACK_H
#define STACK_H

#include <stdio.h>

class Stack {
	private:
		int top;
		int capacity;
		int *storage;
	public:
		Stack(int c);
		~Stack();
		void push(int pos, int val);
		int pop();
};

#endif