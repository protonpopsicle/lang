#include "ByteCoder.h"

ByteCoder::ByteCoder(ZoneMgr *z) {
	zoneMgr = z;
	jumpCounter = 0;
}

void ByteCoder::addOp(short opcode, int operand) {
	Op op = { opcode, operand };
	ops.push_back(op);
	
	if (!activeJumps.empty()) {
		int len = activeJumps.size();
		for (int i = 0; i < len; i++) {
			activeJumps[i].shift++;
		}
	}
	
	if (!activeLoops.empty()) {
		int len = activeLoops.size();
		for (int i = 0; i < len; i++) {
			activeLoops[i]++;
		}
	}
}

char ByteCoder::makeBool(char b) {
	addOp(OP_PUSH, b);
	return BOOL;
}

char ByteCoder::makeInt(int x) {
	addOp(OP_PUSH, x);
	return INT;
}

char ByteCoder::makeString(std::string s) {
	stringLiterals.push_back(s);
	addOp(OP_PUSH, stringLiterals.size() - 1);
	return STRING;
}

char ByteCoder::getVar(std::string id) {
	switch (zoneMgr->getVarType(id)) {
		case BOOL:
			addOp(OP_GET_BOOL, zoneMgr->getVarIndex(id));
			return BOOL;
		case INT:
			addOp(OP_GET_INT, zoneMgr->getVarIndex(id));
			return INT;
		case STRING:
			addOp(OP_GET_STRING, zoneMgr->getVarIndex(id));
			return STRING;
	}
}

char ByteCoder::assignVar(std::string id, char rhs) {
	char type = zoneMgr->getVarType(id);
	if (type == rhs) {
		switch (type) {
			case BOOL:
				addOp(OP_ASSIGN_BOOL, zoneMgr->getVarIndex(id));
				return type;
			case INT:
				addOp(OP_ASSIGN_INT, zoneMgr->getVarIndex(id));
				return type;
			case STRING:
				addOp(OP_ASSIGN_STRING, zoneMgr->getVarIndex(id));
				return type;
		}
	}
	throw Error("type mismatch in variable assignment");
}

char ByteCoder::evalUnary(char rhs) { 
	if (rhs == INT) {
		addOp(OP_PUSH, -1);
		addOp(OP_MULTIPLY, 0);
		return INT;
	}
	throw Error("type mismatch in unary expression");
}

char ByteCoder::evalArith(char oper, char lhs, char rhs) {
	if (lhs == INT && rhs == INT) {
		switch (oper) {
			case '+':
				addOp(OP_ADD, 0);
				break;
			case '-':
				addOp(OP_SUBTRACT, 0);
				break;
			case '*':
				addOp(OP_MULTIPLY, 0);
				break;
			case '/':
				addOp(OP_DIVIDE, 0);
				break;
		}
		return INT;
	}
	if (oper == '+') {			
		if (lhs == STRING && rhs == STRING) {
			addOp(OP_CAT_STRING, 0);
		}
		else if (lhs == STRING && rhs == INT) {
			addOp(OP_CAT_STRING_INT, 0);
		}
		else if (lhs == INT && rhs == STRING) {
			addOp(OP_CAT_INT_STRING, 0);
		}
		return STRING;
	}
	throw Error("type mismatch in arithmetic expression");
}

char ByteCoder::evalBool(short oper, char lhs, char rhs) {
	// relational expression
	if (lhs == INT && rhs == INT) {
		switch (oper) {
			case '<':
				addOp(OP_L, 0);
				break;
			case '>':
				addOp(OP_G, 0);
				break;
			case LE:
				addOp(OP_LE, 0);
				break;
			case GE:
				addOp(OP_GE, 0);
				break;
			case EQ:
				addOp(OP_EQ, 0);
				break;
			case NE:
				addOp(OP_NE, 0);
		}
		return BOOL;
	}
	// logical expression
	if (lhs == BOOL && rhs == BOOL) {
		switch (oper) {
			case AND:
				addOp(OP_AND, 0);
				break;
			case OR:
				addOp(OP_OR, 0);
				break;
		}
		return BOOL;
	}
	throw Error("type mismatch in boolean expression");
}

void ByteCoder::print(char rhs) {
	switch (rhs) {
		case INT:
			addOp(OP_PRINT_INT, 0);
			break;
		case STRING:
			addOp(OP_PRINT_STRING, 0);
			break;
		default:
			throw Error("type mismatch in print statement");
	}
}

void ByteCoder::startLoop() {	
	activeLoops.push_back(0);
}

void ByteCoder::endLoop() {
	int loopLen = activeLoops.back();
	activeLoops.pop_back();
	addOp(OP_JUMP, -loopLen);
}

void ByteCoder::jump() {
	jumpCounter++;
	addOp(OP_JUMP_IF_FALSE, jumpCounter);
	
	Jump jump = { jumpCounter, 0 };
	activeJumps.push_back(jump);
}

void ByteCoder::land() {
	Jump jump = activeJumps.back();
	activeJumps.pop_back();
	
	int len = ops.size();
	for (int i = len; i > 0; i--) {
		if (ops[i].opcode == OP_JUMP_IF_FALSE && ops[i].operand == jump.id) {
			ops[i].operand = jump.shift;
		}
	}
}

void ByteCoder::write(FILE *fp) {
	fprintf(fp, "%i ", zoneMgr->boolCount);    // # of bool vars
	fprintf(fp, "%i ", zoneMgr->intCount);     // # of int vars
	fprintf(fp, "%i ", zoneMgr->stringCount);  // # of string vars
	fprintf(fp, "%i ", stringLiterals.size()); // # of string literals
	fprintf(fp, "%i:", ops.size());            // # of operations
	
	int len = stringLiterals.size();
	for (int i = 0; i < len; i++) {
		fprintf(fp, "%s:", stringLiterals[i].c_str());
	}
	
	len = ops.size();
	for (int i = 0; i < len; i++) {
		fprintf(fp, "%i %i ", ops[i].opcode, ops[i].operand);
	}
}