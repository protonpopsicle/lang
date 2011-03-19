#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <unordered_map>
#include "defines.h"
#include "Token.h"

class Lexer {
	private:
		typedef std::unordered_map<std::string, short> wordTable;
		wordTable keywords;
		FILE *fp;
		unsigned char peek;
		void read();
		bool read(char c);
	public:
		int line;
		Lexer(FILE *f);
		Token scan();
};

#endif