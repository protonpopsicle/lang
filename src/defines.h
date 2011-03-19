#ifndef DEFINES_H
#define DEFINES_H

// basic types
#define BOOL   0
#define INT    1
#define STRING 2

// tags
#define NUM   256 // numeric literal
#define	QUOTE 257 // string literal
#define	ID    258 // identifier

// keyword tags
#define BASIC 259 // basic type
#define TRUE  260 // true keyword
#define FALSE 261 // false keyword
#define	PRINT 262 // print statement
#define	IF    263 // if statement
#define WHILE 264 // while statement
#define DO    265 // do while statement

// composite tags
#define LE  300 // less-than or equal to
#define GE  301 // greater-than or equal to
#define EQ  302 // equal to
#define NE  303 // not equal to
#define AND 304 // logical and
#define OR  305 // logical or

// opcodes
#define OP_PUSH           0
#define OP_GET_BOOL       1
#define OP_GET_INT        2
#define OP_GET_STRING     3
#define OP_ASSIGN_BOOL    4
#define OP_ASSIGN_INT     5
#define OP_ASSIGN_STRING  6
#define OP_ADD            7
#define OP_SUBTRACT       8
#define OP_MULTIPLY       9
#define OP_DIVIDE         10
#define OP_CAT_STRING     11
#define OP_CAT_STRING_INT 12
#define OP_CAT_INT_STRING 13
#define OP_PRINT_INT      14
#define OP_PRINT_STRING   15
#define OP_JUMP           16
#define OP_JUMP_IF_FALSE  17

#define OP_L    20 // less-than
#define OP_G    21 // greater-than
#define OP_LE   22 // less-than or equal to
#define OP_GE   23 // greater-than or equal to
#define OP_EQ   24 // equal to
#define OP_NE   25 // not equal to
#define OP_AND  26 // logical and
#define OP_OR   27 // logical or

#endif