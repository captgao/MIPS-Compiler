#include "pch.h"
#include "Lexical.h"
#include "error.h"
#include <stdio.h>
#include <string.h>
FILE *fp;
char* buffer;
int bufoff = 0;
char c;
char token[1000] = { 0 };
int tokenoff = 0;
int num = 0;
int row = 0;
char constchar = 0;
enum SYMBOL symbol;
char is_new_line = 0;
void file_init() {
	printf("input file:");
	char name[1000] = { '\0' };
	gets_s(name);
	fopen_s(&fp, name, "r");
	FILE *out;
	fopen_s(&out, "out.txt", "w");
	if (fp == NULL) {
		printf("invalid file");
		exit(1);
	}
	char file_buffer[2048002] = { '\0' };
	int buffer_i = 0; char buffer_c;
	while (1) {
		buffer_c = fgetc(fp);
		if (buffer_c == EOF) break;
		else file_buffer[buffer_i++] = buffer_c;
	}
	buffer = file_buffer;
}
void readchar() {
	c = buffer[bufoff++];
}
void retract() {
	bufoff--;
}
int getsym() {
	clearToken();
	readchar();
	while (isSpace() || isNewline() || isTab())
		readchar();
	if (c == '\0') {
		symbol = EOFSY;
		return 1;
	}
	else if (isLetter())
	{
		while (isLetter() || isDigit())
		{
			catToken();
			readchar();
		}
		retract();
		reserver();
	}
	else if (isDigit()) {
		while (isDigit()) {
			catToken();
			readchar();
		}
		retract();
		transNum();
		symbol = INTEGER;
	}

	else if (isPlus()) symbol = ADDSY;
	else if (isMinus()) symbol = SUBSY;
	else if (isStar()) symbol = MULSY;
	else if (isDivi()) symbol = DIVSY;
	else if (isLessthan()) {
		readchar();
		if (isEqu())
			symbol = LEQUSY;
		else {
			retract();
			symbol = LTSY;
		}
	}
	else if (isGreaterthan()) {
		readchar();
		if (isEqu())
			symbol = GEQUSY;
		else {
			retract();
			symbol = GTSY;
		}
	}
	else if (isEqu()) {
		readchar();
		if (isEqu())
			symbol = EQUSY;
		else {
			retract();
			symbol = ASSIGNSY;
		}
	}
	else if (c == '!') {
		readchar();
		if (c == '=') {
			symbol = NOTEQUSY;
		}
		else {
			retract();
			symbol = INVALIDSY;
			error(E_LEXICAL_INVALID);
		}
	}
	else if (isLpar()) symbol = LPARSY;
	else if (isRpar()) symbol = RPARSY;
	else if (isLbrace()) symbol = LBRACESY;
	else if (isRbrace()) symbol = RBRACESY;
	else if (isLbrack()) symbol = LBRACKSY;
	else if (isRbrack()) symbol = RBRACKSY;
	else if (isColon()) symbol = COLONSY;
	else if (isComma()) symbol = COMMASY;
	else if (isSemi()) symbol = SEMISY;
	else if (isSingleQuote()) {
		readchar();
		if (isPlus() || isMinus() || isStar() || isDivi() || isLetter() || isDigit()) {
			constchar = c;
			readchar();
		}
		else {
			if (isSingleQuote()) {
				constchar = 0;
				symbol = CHARCONST;
				return 0;
			}
			else {
				error(E_LEXICAL_SINGLEQUOTE_MISMATCH);
				constchar = 0;
				readchar();
			}
		}
		if (isSingleQuote()) {
			symbol = CHARCONST;
			return 0;
		}
		else {
			do {
				readchar();
			} while (!isComma() && !isSemi() && !isSingleQuote() && !isNewline() && c);
			if (isComma() || isSemi() || isNewline()) {
				error(E_LEXICAL_CHAR_NOT_END);
				symbol = INVALIDSY;
			}
			else if (isSingleQuote()) {
				error(E_LEXICAL_MULTI_CHAR);
				symbol = INVALIDSY;
			}
			else if (c == '\0') {
				error(E_UNEXPECTED_FILE_ENDED);
				symbol = EOFSY;
				return 1;
			}
		}
	}
	else if (isDoubleQuote()) {
		readchar();
		while (c >= 32 && c <= 126 && c != 34)
		{
			catToken();
			readchar();
		}
		if (isDoubleQuote()) {
			symbol = STRING;
		}
		else {
			error(E_LEXCIAL_STRING_NOT_END);
			symbol = INVALIDSY;
		}
	}
	else {
		error(E_LEXICAL_INVALID);
		symbol = INVALIDSY;
	}
	return 0;
}
void clearToken() {
	int i;
	for (i = 0; i < 1000; i++) token[i] = '\0';
	tokenoff = 0;
}
char isLetter() {
	return (c >= 'a'&&c <= 'z' || c >= 'A' && c <= 'Z' || c == '_') ? 1 : 0;
}
char isDigit() {
	return (c >= '0' && c <= '9') ? 1 : 0;
}
char isColon() {
	return c == ':' ? 1 : 0;
}
char isEqu() {
	return c == '=' ? 1 : 0;
}
char isPlus() {
	return c == '+' ? 1 : 0;
}
char isMinus() {
	return c == '-' ? 1 : 0;
}
char isStar() {
	return c == '*' ? 1 : 0;
}
char isDivi() {
	return c == '/' ? 1 : 0;
}
char isLpar() {
	return c == '(' ? 1 : 0;
}
char isRpar() {
	return c == ')' ? 1 : 0;
}
char isComma() {
	return c == ',' ? 1 : 0;
}
char isSemi() {
	return c == ';' ? 1 : 0;
}
char isSpace() {
	return c == ' ' ? 1 : 0;
}
char isLbrace() {
	return c == '{' ? 1 : 0;
}
char isRbrace() {
	return c == '}' ? 1 : 0;
}
char isLbrack() {
	return c == '[' ? 1 : 0;
}
char isRbrack() {
	return c == ']' ? 1 : 0;
}
char isLessthan() {
	return c == '<' ? 1 : 0;
}
char isGreaterthan() {
	return c == '>' ? 1 : 0;
}
char isNewline() {
	if (c == '\n') {
		is_new_line = 1;
		row++;
	}
	return c == '\r' || c == '\n' ? 1 : 0;
}
char isTab() {
	return c == '\t' ? 1 : 0;
}
char isSingleQuote() {
	return c == '\'' ? 1 : 0;
}
char isDoubleQuote() {
	return c == '\"' ? 1 : 0;
}
bool isRelationSymbol() {
	return symbol == LTSY || symbol == GTSY || symbol == LEQUSY || symbol == GEQUSY || symbol == EQUSY || symbol == NOTEQUSY;
}
int reserver() {
	if (strcmp(token, "const") == 0) {
		symbol = CONSTSY;
	}
	else if (strcmp(token, "int") == 0) {
		symbol = INTSY;
	}
	else if (strcmp(token, "char") == 0) {
		symbol = CHARSY;
	}
	else if (strcmp(token, "void") == 0) {
		symbol = VOIDSY;
	}
	else if (strcmp(token, "main") == 0) {
		symbol = MAINSY;
	}
	else if (strcmp(token, "if") == 0) {
		symbol = IFSY;
	}
	else if (strcmp(token, "while") == 0) {
		symbol = WHILESY;
	}
	else if (strcmp(token, "switch") == 0) {
		symbol = SWITCHSY;
	}
	else if (strcmp(token, "case") == 0) {
		symbol = CASESY;
	}
	else if (strcmp(token, "default") == 0) {
		symbol = DEFAULTSY;
	}
	else if (strcmp(token, "scanf") == 0) {
		symbol = SCANFSY;
	}
	else if (strcmp(token, "printf") == 0) {
		symbol = PRINTFSY;
	}
	else if (strcmp(token, "return") == 0) {
		symbol = RETURNSY;
	}
	else {
		symbol = IDEN;
		return 0;
	}
	return 1;
}
void transNum() {
	num = 0;
	int i;
	for (i = 0; i < 1000; i++) {
		if (token[i] == '\0') break;
		num = num * 10 + token[i] - '0';
	}
}
void catToken() {
	token[tokenoff++] = c;
}