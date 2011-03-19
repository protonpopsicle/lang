#include <stdio.h>
#include <string>
#include "defines.h"
#include "Stack.h"

struct Op {
	short opcode;
	int operand;
};

int main(int argc, char *argv[]) {
	if (argv[1] == 0) {
		fprintf(stderr, "missing argument 1: filename\n");
		return(0);
	}
	
	/*
	* Read bytecode
	*/
	char *file = argv[1];
	FILE *fp = fopen(file, "r");
	if (fp == 0) {
		fprintf(stderr, "error opening file\n");
		return(0);
	}
	
	int boolCount, intCount, strCount, litCount, opCount;
	
	if (fscanf(fp, "%i %i %i %i %i:", &boolCount, &intCount, &strCount, &litCount, &opCount) != 5) {
		fprintf(stderr, "error reading bytecode");
		return 0;
	}
	
	strCount = strCount + litCount + 1; // string vars + string literals + temp string
	
	char bools[boolCount];
	int ints[intCount];
	std::string strings[strCount];
	Op ops[opCount];
	
	char c;
	for (int i = 0; i < litCount; i++) {
		c = fgetc(fp);
		while (c != ':') {
			strings[i] += c;
			c = fgetc(fp);
		}
	}

	for (int i = 0; i < opCount; i++) {
		fscanf(fp, "%hi %i ", &ops[i].opcode, &ops[i].operand);
	}
	
	fclose(fp);
	
	/*
	* Print debug information
	*/
	/*	
	printf("\nBools...\n");
	for (int i = 0; i < boolCount; i++) {
		printf("%i: %i\n", i, bools[i]);
	}
	
	printf("\nInts...\n");
	for (int i = 0; i < intCount; i++) {
		printf("%i: %i\n", i, ints[i]);
	}
	
	printf("\nStrings...\n");
	for (int i = 0; i < strCount; i++) {
		printf("%i: %s\n", i, strings[i].c_str());
	}
	
	printf("\nOpcodes...\n");
	for (int i = 0; i < opCount; i++) {
		std::string opcode;
		
		switch (ops[i].opcode) {
			case OP_PUSH:
				opcode = "OP_PUSH";
				break;
			case OP_GET_BOOL:
				opcode = "OP_GET_BOOL";
				break;
			case OP_GET_INT:
				opcode = "OP_GET_INT";
				break;
			case OP_GET_STRING:
				opcode = "OP_GET_STRING";
				break;
			case OP_ASSIGN_BOOL:
				opcode = "OP_ASSIGN_BOOL";
				break;
			case OP_ASSIGN_INT:
				opcode = "OP_ASSIGN_INT";
				break;
			case OP_ASSIGN_STRING:
				opcode = "OP_ASSIGN_STRING";
				break;
			case OP_ADD:
				opcode = "OP_ADD";
				break;
			case OP_SUBTRACT:
				opcode = "OP_SUBTRACT";
				break;
			case OP_MULTIPLY:
				opcode = "OP_MULTIPLY";
				break;
			case OP_DIVIDE:
				opcode = "OP_DIVIDE";
				break;
			case OP_CAT_STRING:
				opcode = "OP_CAT_STRING";
				break;
			case OP_CAT_STRING_INT:
				opcode = "OP_CAT_STRING_INT";
				break;
			case OP_CAT_INT_STRING:
				opcode = "OP_CAT_INT_STRING";
				break;
			case OP_PRINT_INT:
				opcode = "OP_PRINT_INT";
				break;
			case OP_PRINT_STRING:
				opcode = "OP_PRINT_STRING";
				break;
			case OP_JUMP:
				opcode = "OP_JUMP";
				break;
			case OP_JUMP_IF_FALSE:
				opcode = "OP_JUMP_IF_FALSE";
				break;
			case OP_L:
				opcode = "OP_L";
				break;
			case OP_G:
				opcode = "OP_G";
				break;
			case OP_LE:
				opcode = "OP_LE";
				break;
			case OP_GE:
				opcode = "OP_GE";
				break;
			case OP_EQ:
				opcode = "OP_EQ";
				break;
			case OP_NE:
				opcode = "OP_NE";
				break;
			case OP_AND:
				opcode = "OP_AND";
				break;
			case OP_OR:
				opcode = "OP_OR";
				break;
			default:
				opcode = "undefined";
		}
		
		printf("%i: %s %i\n", i, opcode.c_str(), ops[i].operand);
	}
	printf("\n");
	*/	
	/*
	* Run program
	*/
	bool a, b;
	int x, y;
	std::string strA, strB;
	char intBuffer[12];
	Stack stack(2);
	
	for (int i = 0; i < opCount; i++) {
		switch (ops[i].opcode) {
			case OP_PUSH:
				stack.push(i, ops[i].operand);
				break;
			case OP_GET_BOOL:
				stack.push(i, bools[ops[i].operand]);
				break;
			case OP_GET_INT:
				stack.push(i,ints[ops[i].operand]);
				break;
			case OP_GET_STRING:
				// offset by litCount because variables come after literals in array
				stack.push(i, ops[i].operand + litCount);
				break;
			case OP_ASSIGN_BOOL:
				bools[ops[i].operand] = stack.pop();
				break;
			case OP_ASSIGN_INT:
				ints[ops[i].operand] = stack.pop();
				break;
			case OP_ASSIGN_STRING:
				strings[ops[i].operand + litCount] = strings[stack.pop()];
				break;
			case OP_ADD:
				y = stack.pop(); 
				x = stack.pop();
				stack.push(i, x + y);
				break;
			case OP_SUBTRACT:
				y = stack.pop();
				x = stack.pop();
				stack.push(i, x - y);
				break;
			case OP_MULTIPLY:
				y = stack.pop();
				x = stack.pop();
				stack.push(i, x * y);
				break;
			case OP_DIVIDE:
				y = stack.pop();
				x = stack.pop();
				stack.push(i, x / y);
				break;
			case OP_CAT_STRING:
				strB = strings[stack.pop()];
				strA = strings[stack.pop()];
				strings[strCount - 1] = strA + strB;
				stack.push(i, strCount - 1);
				break;
			case OP_CAT_STRING_INT:
				x = stack.pop();
				strA = strings[stack.pop()];
				snprintf(intBuffer, sizeof(intBuffer), "%i", x);
				strings[strCount - 1] = strA.append(intBuffer);
				stack.push(i, strCount - 1);
				break;
			case OP_CAT_INT_STRING:
				strA = strings[stack.pop()];
				x = stack.pop();
				snprintf(intBuffer, sizeof(intBuffer), "%i", x);
				strings[strCount - 1] = strA.insert(0, intBuffer);
				stack.push(i, strCount - 1);
				break;
			case OP_PRINT_INT:
				x = stack.pop();
				printf("%i", x);
				break;
			case OP_JUMP:
				i += ops[i].operand - 1;
				break;
			case OP_JUMP_IF_FALSE:
				if (!stack.pop()) {
					i += ops[i].operand;
				}
				break;
			case OP_PRINT_STRING:
				strA = strings[stack.pop()];
				printf("%s", strA.c_str());
				break;
			case OP_L:
				y = stack.pop();
				x = stack.pop();
				stack.push(i, x < y);
				break;
			case OP_G:
				y = stack.pop();
				x = stack.pop();
				stack.push(i, x > y);
				break;
			case OP_LE:
				y = stack.pop();
				x = stack.pop();
				stack.push(i, x <= y);
				break;
			case OP_GE:
				y = stack.pop();
				x = stack.pop();
				stack.push(i, x >= y);
				break;
			case OP_EQ:
				y = stack.pop();
				x = stack.pop();
				stack.push(i, x == y);
				break;
			case OP_NE:
				y = stack.pop();
				x = stack.pop();
				stack.push(i, x != y);
				break;
			case OP_AND:
				b = stack.pop();
				a = stack.pop();
				stack.push(i, a && b);
				break;
			case OP_OR:
				b = stack.pop();
				a = stack.pop();
				stack.push(i, b || a);
				break;
		}
	}
	
	return 0;
}
