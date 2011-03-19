#include "Parser.h"

Parser::Parser(Lexer *l, ZoneMgr *z, ByteCoder *b) 
{
	lexer = l;
	zoneMgr = z;
	byteCoder =	b;
	
	move();
}

void Parser::move() {
	look = lexer->scan();
}

void Parser::match(short tag) {
	if (look.tag == tag) {
		move();
	}
	else {
		throw Error(tag, look.tag);
	}
}

//=================
// program -> block
//=================
void Parser::program() {
	block();
}

//===================
// block -> { stmts }
//===================
void Parser::block() {
	match('{');
	zoneMgr->saveZone();
	zoneMgr->createZone();
	stmts();
	match('}');
	zoneMgr->loadZone();
}

//===================================
// decl -> basic id | basic id = bool
// or...
// decl -> basic id restDecl
// restDecl -> = bool | e
//===================================
void Parser::decl() {
	// decl -> basic id
	std::string type = look.lexeme;
	move();
	std::string id = look.lexeme;
	match(ID);
	zoneMgr->addVar(id, type);
	// restDecl -> = bool
	if (look.tag == '=') {
		match('=');
		char rhs = boolean();
		byteCoder->assignVar(id, rhs);
	}
	// restDecl -> e
}

//==============================================
// stmts -> stmt stmts | e
// stmt -> block | decl | id = bool | print bool
//		 | if bool block
//       | while bool block 
//	 	 | do block while bool
//==============================================
void Parser::stmts() {
	char lhs, rhs;
	std::string id;
	
	while (look.tag == '{' || look.tag == ID || look.tag == BASIC || look.tag == PRINT || 
		   look.tag == IF || look.tag == WHILE || look.tag == DO) {
		switch (look.tag) {
			// stmt -> block
			case '{':
				block();
				break;
			// stmt -> decl
			case BASIC:
				decl();
				break;
			// stmt -> id = bool
			case ID:
				id = look.lexeme;
				move(); 
				match('=');
				rhs = boolean();
				byteCoder->assignVar(id, rhs);
				break;
			// stmt -> print bool
			case PRINT:
				move(); 
				rhs = boolean();
				byteCoder->print(rhs);
				break;
			// stmt -> if bool block
			case IF:
				move(); 
				boolean();
				byteCoder->jump();
				block();
				byteCoder->land();
				break;
			// stmt -> while bool block
			case WHILE:
				move();
				byteCoder->startLoop();
				boolean();
				byteCoder->jump();
				block();
				byteCoder->endLoop();
				byteCoder->land();
				break;
			// stmt -> do block while bool
			case DO:
				move();
				byteCoder->startLoop();
				block();
				match(WHILE);
				boolean();
				byteCoder->jump();
				byteCoder->endLoop();
				byteCoder->land();
				break;
		}
	}
}

//=================================
// bool -> bool || join | join
// or...
// bool -> join restBool
// restBool -> || join restBool | e
//=================================
char Parser::boolean() {
	char lhs, rhs;
	lhs = join(); // bool -> join restBool
	
	// restBool -> || join restBool
	while (look.tag == OR) {
		move();
		rhs = join();
		lhs = byteCoder->evalBool(OR, lhs, rhs);
	}
	// restBool -> e
	return lhs;
}

//=====================================
// join -> join && equality | equality
// or...
// join -> equality restJoin
// restJoin -> && equality restJoin | e
//=====================================
char Parser::join() {
	char lhs, rhs;
	lhs = equality(); // join -> equality restJoin
	
	// restJoin -> && equality restJoin
	while (look.tag == AND) {
		move();
		rhs = equality();
		lhs = byteCoder->evalBool(AND, lhs, rhs);
	}
	// restJoin -> e
	return lhs;
}

//==============================================================
// equality -> equality == rel | equality != rel | rel
// or...
// equality -> rel restEquality
// restEquality -> == rel restEquality | != rel restEquality | e
//==============================================================
char Parser::equality() {
	short oper;
	char lhs, rhs;
	lhs = rel(); // equality -> rel restEquality
	
	// restEquality -> == rel restEquality | != rel restEquality
	while (look.tag == EQ || look.tag == NE) {
		oper = look.tag;
		move();
		rhs = rel();
		lhs = byteCoder->evalBool(oper, lhs, rhs);
	}
	// restEquality -> e
	return lhs;
}

//===================================================================================
// rel -> expr < expr | expr > expr | expr <= expr | expr >= expr | expr
// or...
// rel -> expr restRel
// restRel -> < expr restRel | > expr restRel | <= expr restRel | >= expr restRel | e
//===================================================================================
char Parser::rel() {
	short oper;
	char lhs, rhs;
	lhs = expr(); // rel -> expr restRel
	
	// restRel -> < expr restRel | > expr restRel | <= expr restRel | >= expr restRel
	while (look.tag == '<' || look.tag == '>' || look.tag == LE || look.tag == GE) {
		oper = look.tag;
		move();
		rhs = expr();
		lhs = byteCoder->evalBool(oper, lhs, rhs);
	}
	// restRel -> e
	return lhs;
}

//==================================================
// expr -> expr + term | expr - term | term
// or...
// expr -> term restExpr
// restExpr -> + term restExpr | - term restExpr | e
//==================================================
char Parser::expr() {
	char oper, lhs, rhs;
	lhs = term(); // expr -> term restExpr
	
	// restExpr -> + term restExpr | - term restExpr
	while (look.tag == '+' || look.tag == '-') {
		oper = look.tag;
		move(); 
		rhs = term();
		lhs = byteCoder->evalArith(oper, lhs, rhs);
	}
	// restExpr -> e
	return lhs;
}

//====================================================
// term -> term * unary | term / unary | unary
// or...
// term -> unary restTerm
// restTerm -> * unary restTerm | / unary restTerm | e
//====================================================
char Parser::term() {
	char oper, lhs, rhs;
	lhs = unary(); // term -> unary restTerm
	
	// restTerm -> * unary restTerm | / unary restTerm
	while (look.tag == '*' || look.tag == '/') {
		oper = look.tag;
		move(); 
		rhs = unary();
		lhs = byteCoder->evalArith(oper, lhs, rhs);
	}
	// restTerm -> e
	return lhs;
}

//==========================
// unary -> - unary | factor
//==========================
char Parser::unary() {
	char rhs;
	
	// unary -> - unary
	if (look.tag == '-') {
		move(); 
		rhs = unary();
		return byteCoder->evalUnary(rhs);
	}
	// unary -> factor
	return factor();
}

//===========================================
// factor -> true | false | int | string | id
//===========================================
char Parser::factor() {
	int x;
	std::string s;
	
	switch (look.tag) {
		// factor -> true
		case TRUE:
			move();
			return byteCoder->makeBool(1);
		// factor -> false
		case FALSE:
			move();
			return byteCoder->makeBool(0);
		// factor -> int
		case NUM:
			x = look.val;
			move();
			return byteCoder->makeInt(x);
		// factor -> string
		case QUOTE:
			s = look.lexeme;
			move();
			return byteCoder->makeString(s);
		// factor -> id
		case ID:
			s = look.lexeme;
			move();
			return byteCoder->getVar(s);
	}
	throw Error("expected datatype");
}