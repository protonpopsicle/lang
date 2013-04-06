#ifndef PARSER_H
#define PARSER_H

#include <string>
#include "defines.h"
#include "Error.h"
#include "Lexer.h"
#include "ZoneMgr.h"
#include "ByteCoder.h"

class Parser {
    private:
        Lexer *lexer;
        ZoneMgr *zoneMgr;
        ByteCoder *byteCoder;
        Token look;
        void move();
        void match(short tag);
        void block();
        void decl();
        void stmts();
        char boolean();
        char join();
        char equality();
        char rel();
        char expr();
        char term();
        char unary();
        char factor();
    public:
        Parser(Lexer *l, ZoneMgr *z, ByteCoder *b);
        void program();
};

#endif