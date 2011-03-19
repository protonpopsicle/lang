#include "Error.h"

Error::Error(std::string m) {
	msg = m;
}

// handle syntax error
Error::Error(short tag, short near) {
	msg = "expected " + tagToStr(tag) + " near " + tagToStr(near);
}

std::string Error::tagToStr(short tag) {
	switch (tag) {
		case 32: 
			return "space";
		case 255: 
			return "program end";
		case NUM: 
			return "numeric literal";
		case QUOTE: 
			return "string literal";
		case ID: 
			return "identifier";
		case BASIC: 
			return "basic type";
		case TRUE: 
			return "true keyword";
		case FALSE: 
			return "false keyword";
		case PRINT: 
			return "print statement";
		case IF: 
			return "if statement";
		case WHILE: 
			return "while statement";
		case LE: 
			return "relational expression";
		case GE: 
			return "relational expression";
		case EQ: 
			return "relational expression";
		case NE: 
			return "relational expression";
		case AND: 
			return "relational expression";
		case OR: 
			return "relational expression";
		default: 
			// returns a string formed by 1 repetition of (char)tag
			return std::string(1, (unsigned char)tag);
	}
}