#include "pch.h"
#include <stdio.h>
#include "Lexical.h"
//#include "error.h"
#include "Compiler.h"
using namespace std;
void error(enum ERROR e) {
	printf("ERROR %d, symbol %d, row %d\n", e, symbol, row);
	if (symbol == IDEN) printf("IDEN %s\n", token);
	while (symbol != SEMISY && symbol != RBRACESY) {
		getsym();
	}

}
void semantic_error(enum SEMANTIC_ERROR e) {
	printf("Semantic Error %d, row %d, token %s\n", e, row, token);
}
void warning(enum WARNING w) {
	printf("warning %d, row %d\n", w,row);
}
void critical(enum ERROR e) {
	printf("critical warning %d\n", e);
}