#pragma   once  
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
typedef enum SYMBOL {
	IDEN,		//identifier
	INTEGER,	//123456789
	CHARCONST,	//'0'
	STRING,		//"123123123"
	CONSTSY,	//const
	INTSY,		//int
	CHARSY,		//char
	VOIDSY,		//void
	MAINSY,		//main
	IFSY,		//if
	WHILESY,	//while
	SWITCHSY,	//switch
	CASESY,		//case
	DEFAULTSY,	//default
	SCANFSY,	//scanf
	PRINTFSY,	//printf
	RETURNSY,	//return
	ADDSY,		//+
	SUBSY,		//-
	MULSY,		//*
	DIVSY,		///
	LTSY,	//<
	LEQUSY,		//<=
	GTSY,	//>
	GEQUSY,		//>=
	EQUSY,		//==
	NOTEQUSY,	//!=
	ASSIGNSY,	//=
	LPARSY,		//(
	RPARSY,		//)
	COLONSY,	//:
	SEMISY,		//;
	COMMASY,	//,
	LBRACESY,	//{
	RBRACESY,	//}
	LBRACKSY,	//[
	RBRACKSY,	//]
	SQSY,		//'
	DQSY,		//"
	EOFSY,
	INVALIDSY
}SYMBOL_ENUM;


void file_init();

void readchar();

void retract();

int getsym();

void clearToken();

char isLetter();

char isDigit();

char isColon();

char isEqu();

char isPlus();

char isMinus();

char isStar();

char isDivi();

char isLpar();

char isRpar();

char isComma();

char isSemi();

char isSpace();

char isLbrace();

char isRbrace();

char isLbrack();

char isRbrack();

char isLessthan();

char isGreaterthan();

char isNewline();

char isTab();

char isSingleQuote();

char isDoubleQuote();

bool isRelationSymbol();

int reserver();

void transNum();

void catToken();

