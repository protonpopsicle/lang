#include "Token.h"

Token::Token() {
    tag = 0; 
    val = 0; 
}
    
Token::Token(short t) {
    tag = t;
    val = 0;
}
    
Token::Token(short t, int v) {
    tag = t; 
    val = v; 
}
    
Token::Token(short t, std::string l) {
    tag = t; 
    val = 0; 
    lexeme = l; 
}
