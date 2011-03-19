#include "Stack.h"

Stack::Stack(int c) {
	top = -1;
	capacity = c;
	storage = new int[capacity];
}

void Stack::push(int pos, int val) {
	top++;
	//if (top == capacity) {
	//	fprintf(stderr, "Error at %i: stack overflow\n", pos);
	//	fprintf(stderr, "tried to add: %i\n", val);
	//	fprintf(stderr, "contains: %i & %i\n", storage[0], storage[1]);
	//	top--;
	//}
	//else {
		storage[top] = val;
	//}
}

int Stack::pop() {
	int val = storage[top];
	top--;
	return val;
}

Stack::~Stack() {
	delete[] storage;
}