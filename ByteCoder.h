#ifndef BYTECODER_H
#define BYTECODER_H

#include <stdio.h>
#include <string>
#include <vector>
#include "defines.h"
#include "Error.h"
#include "ZoneMgr.h"

class ByteCoder {
	private:
		struct Op {
			short opcode;
			int operand;
		};
		struct Jump {
			int id;
			int shift;
		};
		
		int jumpCounter;
		std::vector<int> activeLoops;
		std::vector<std::string> stringLiterals;
		std::vector<Op> ops;
		std::vector<Jump> activeJumps;
		ZoneMgr *zoneMgr;
	public:
		ByteCoder(ZoneMgr *z);
		void addOp(short opcode, int operand);
		char makeBool(char b);
		char makeInt(int i);
		char makeString(std::string s);
		char getVar(std::string id);
		char assignVar(std::string s, char rhs);
		char evalUnary(char rhs);
		char evalArith(char oper, char lhs, char rhs);
		char evalBool(short oper, char lhs, char rhs);
		void print(char rhs);
		void startLoop();
		void endLoop();
		void jump();
		void land();
		void write(FILE *fp);
};

#endif