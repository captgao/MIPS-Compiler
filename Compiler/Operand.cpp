#include "pch.h"
#include "Operand.h"
#include "SymbolTable.h"
extern TableEntry* cur_funcentry;
OPNUM currentlabel = 1;
Operand genConstChar(char _c) {
	Operand op;
	op.optype = OPCCHAR;
	op.value.constchar = _c;
	op.type = CHAR;
	return op;
}
Operand genConstInt(int _i) {
	Operand op;
	op.optype = OPCINT;
	op.value.constint = _i;
	op.type = INT;
	return op;
}
Operand genTemp() {
	Operand op;
	op.optype = OPTEMP;
	op.value.temp = cur_funcentry->tmpindex++;
	op.type = INT;
	return op;
}
Operand genEntry(TableEntry* entry) {
	Operand op;
	op.optype = OPENTRY;

	if (entry != NULL) {
		op.type = entry->type;
		if (entry->type == CONSTCHAR) {
			op.value.constchar = entry->value;
		}
		else if (entry->type == CONSTINT) {
			op.value.constint = entry->value;
		}
		else {
			op.value.entry = entry;
		}
	}
	else {
		cout << "NULL Entry, Operand.cpp " << __LINE__ << endl;
	}
	return op;
}
Operand genNULL() {
	Operand op;
	op.optype = OPNULL;
	return op;
}
Operand genLabel() {
	Operand op;
	op.optype = OPLABEL;
	op.value.temp = currentlabel++;
	return op;
}