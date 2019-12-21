#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "type.h"

FILE* file;
FILE* code;

char ch;
char buffer[1000];
char token_buffer[1000];
char token_string[1000];
char ident_buffer[1000];
char ident_string[1000];
char g_prev_buffer[1000];

int length;
int line_num;
int max_token;
int n;
int in;
int Error;
int function_depth;
int g_prev_type;

typedef enum
{
	WORD, IDENT, NUMBER, DIGIT, LETTER, SPACE, NELI, TAB, ERROR, UNKNOWN, BEGIN, END,
	MARK_TYPE,            SEPERATOR, OPERATOR, EQUALIZER, OPEN_PAREN, CLOSE_PAREN, SEMICOLON, ASSIGN, EQUAL, LARROW, LPAREN, RPAREN, COMMA, PERIOD, LBRACKET, RBRACKET,
	LITERALS,          NUM_LITERAL,
    MATH_TYPES,        ADDITION, SUBTRACTION, MULTIPLICATION, DIVISION,  ADD, SUB, MULTI, DIVI,
	PREDIRECT,         USE, SYSTEM, TRANSLATE,
	HEADER,            GRAPHICS, HAVEN, S_TYPE, M_TYPE, N_TYPE, IO,
	DATA_TYPE,         NUM, MARK, DECI, DECII, QUAD, STRAND, HOLLOW,
	DATA_TOOL,        CHECK, ELSE, LOOP, SET, CONSTRUCT, BRIDGE, LIST, UNTIL,
	DATA_MODIFIER,
	FUNCTION_TYPE,     READ, WRITE,  SOFI, FILER,
} token_types;

token_types token_type;

//char data_modifiers[][12] = { "extern","static", "register","short", "long", "signed", "unsigned" };

/***********   Syntax tree for parsing ************/
typedef enum { StmtK, ExpK } Node_Kind;
typedef enum { checkK, identK, readK, writeK, predirectK, endK } Statement_Kind;
typedef enum { OpK, ConstK, IdK } Expression_Kind;

/* ExpType is used for type checking */
typedef enum { Hollow, Num, Pare, List, Function, Assign, Assign_Id } Expression_Type;

typedef struct treeNode
{
	struct tree_Node * child[max_children];
	struct tree_Node * sibling;

	int lineno;

	Node_Kind nodekind;

	union 
	{ 
		Statement_Kind stmt;
		Expression_Kind exp; 
	} kind;

	union 
	{
		token_types op;
		int val;
		char * name;
	} attr;

	Expression_Type type; /* for type checking of exps */
} TreeNode;

token_types get_token(void);


void print_token(token_types token, const char* token_string);
void print_tree(TreeNode *);
TreeNode * parse(void);

char * copy_string(char *);
TreeNode * newStmtNode(StmtKind);
TreeNode * newExpNode(ExpKind);


TreeNode * parse(void);
