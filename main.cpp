#include "Error.h"
#include "Lexer.h"
#include "ZoneMgr.h"
#include "ByteCoder.h"
#include "Parser.h"

int main(int argc, char *argv[]) {
	if (argv[1] == 0) {
		fprintf(stderr, "missing argument 1: filename");
		return(0);
	}
	
	/*
	* Load source file
	*/
	char *source = argv[1];
	FILE *fp = fopen(source, "r");
	if (fp == 0) {
		fprintf(stderr, "error opening file");
		return(0);
	}
	
	/*
	* Initialize compiler
	*/
	Lexer *lexer = new Lexer(fp);
	ZoneMgr *zoneMgr = new ZoneMgr();
	ByteCoder *byteCoder = new ByteCoder(zoneMgr);
	Parser *parser = new Parser(lexer, zoneMgr, byteCoder);
	
	/*
	* Run compiler
	*/
	try {
		parser->program();
	}
	catch (Error err) {
		fprintf(stderr, "near line %i: %s\n", lexer->line, err.msg.c_str());
	}
	
	/*
	* Print bytecode
	*/
	std::string fname(source);
	size_t pos = fname.find_last_of(".");
	if (pos != -1) {
		fname = fname.substr(0, pos);
	}
	fname += ".x";
	
	fp = fopen(fname.c_str(), "w");
	byteCoder->write(fp);
	fclose(fp);
	
	/*
	* Cleanup
	*/
	delete lexer;
	delete zoneMgr;
	delete byteCoder;
	delete parser;
	
	return 0;
}