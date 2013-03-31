#ifndef TOKEN_H
#define TOKEN_H

#include <string>

class Token {
	public:
		short tag;
		int val;
		std::string lexeme;
		Token();
		Token(short t);
		Token(short t, int v);
		Token(short t, std::string l);
};

#endif