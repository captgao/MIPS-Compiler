#pragma once
typedef unsigned int OPNUM;
using namespace std;
class TableEntry;
enum OperandType {
	OPCINT,OPCCHAR,OPTEMP,OPENTRY,OPNULL,OPLABEL
};
typedef struct {
	enum OperandType optype;
	union {
		unsigned int temp;
		int constint;
		char constchar;
		TableEntry* entry;
	}value;
	enum TYPE type;
} Operand;

Operand genConstChar(char _c);

Operand genConstInt(int _i);

Operand genTemp();

Operand genEntry(TableEntry * entry);

Operand genNULL();

Operand genLabel();

