#include "Lexer.h"

Lexer::Lexer(FILE *f) {
	fp = f;
	peek = ' '; 
	line = 1;
	
	keywords.insert(std::make_pair("int", BASIC));
	keywords.insert(std::make_pair("string", BASIC));
	keywords.insert(std::make_pair("bool", BASIC));
	keywords.insert(std::make_pair("true", TRUE));
	keywords.insert(std::make_pair("false", FALSE));
	keywords.insert(std::make_pair("print", PRINT));
	keywords.insert(std::make_pair("if", IF));
	keywords.insert(std::make_pair("while", WHILE));
	keywords.insert(std::make_pair("do", DO));
}

void Lexer::read() {
	peek = fgetc(fp); // advance peek
}

bool Lexer::read(char c) {
	read();
	if (peek != c) {
		return false;
	}
	peek = ' '; // clear peek
	return true; 
}

Token Lexer::scan() {
	// skip white space & comments
	for (;; read()) {
		if (peek == ' ' || peek == '\t') {
			continue;
		}
		// increment line number by either LF or CRLF
		else if (peek == '\n' || peek == '\r' && read('\n')) {
			line++;
		}
		else if (peek == '/' && read('/')) {
			for (;; read()) {
				if (peek == '\n' || peek == '\r' && read('\n')) {
					line++;
					break;
				}
			}
		}
		else break;
	}
	// handle composite tokens
	switch (peek) {
		case '<': if (read('=')) return Token(LE);  return Token('<');
		case '>': if (read('=')) return Token(GE);  return Token('>');
		case '=': if (read('=')) return Token(EQ);  return Token('=');
		case '!': if (read('=')) return Token(NE);  return Token('!');
		case '&': if (read('&')) return Token(AND); return Token('&');
		case '|': if (read('|')) return Token(OR);  return Token('|');
	}
	// handle numeric literals
	if (peek >= '0' && peek <= '9') {
		int x = 0;
		do {
			x = 10 * x + peek - '0'; // peek - '0' converts peek char to int
			read();
		} while (peek >= '0' && peek <= '9');
		return Token(NUM, x);
		/*
		if (peek != '.') {
			return Token(NUM, x);
		}
		float x = i;
		float d = 10;
		for(;;) {
			read();
			if (peek < '0' || peek > '9') break;
			x = x + (peek - '0') / d;
			d = d * 10;
		}
		return Token(REAL, x);
		*/
	}
	// handle reserved words and identifiers
	if (peek >= 'A' && peek <= 'Z' || peek >= 'a' && peek <= 'z') {
		std::string word;
		do {
			word += peek;
			read();
		} while (peek >= '0' && peek <= '9' || peek >= 'a' && peek <= 'z' ||
				 peek >= 'a' && peek <= 'z');
		// check if word is already mapped
		// if so it's a keyword, otherwise it's an identifier
		wordTable::iterator iter = keywords.find(word);
		if (iter != keywords.end()) {
			return Token(iter->second, word); // iter->second points to this keyword's tag
		}
		return Token(ID, word);
	}
	// handle string literals
	if (peek == '"') {
		std::string q;
		for (;;) {
			read();
			if (peek != '"') {
				q += peek;
			}
			else {
				read();
				break;
			}
		}
		return Token(QUOTE, q);
	}
	// if peek satisfies none of the above
	// return token with peek char as tag
	unsigned char c = peek;
	peek = ' '; // clear peek
	return Token(c);
}