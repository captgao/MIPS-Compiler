#include "pch.h"
#include "SymbolTable.h"
#include <iostream>
#include <vector>
#include <assert.h>
#include "Assembly.h"
using namespace std;
extern vector<TableEntry*> glbl_symtable;
extern vector<string> string_table;
TableEntry* dump_curfunc;
unsigned int argcount = 0;
char regnum = 0;
int regcontent[8] = { -1,-1,-1,-1,-1,-1,-1,-1 };
void dumpdata(ostream &out) {
	out << ".data" << endl;
	int stringindex = 0;
	for (unsigned int i = 0; i < glbl_symtable.size(); i++) {
		if (glbl_symtable[i]->type == INT || glbl_symtable[i]->type == CHAR) {
			out << "\t.align 2" << endl;
			out << "\t__glbl_var_" << glbl_symtable[i]->iden << ": .word 0" << endl;
		}
		else if (glbl_symtable[i]->type == INTARRAY) {
			out << "\t.align 2" << endl;
			out << "__glbl_array_" << glbl_symtable[i]->iden << ": .space " << glbl_symtable[i]->length * 4 << endl;
		}
		else if (glbl_symtable[i]->type == CHARARRAY) {
			out << "\t.align 2" << endl;
			out << "__glbl_array_" << glbl_symtable[i]->iden << ": .space " << glbl_symtable[i]->length << endl;
		}
	}
	for (unsigned int i = 0; i < string_table.size(); i++) {
		out << "\t.align 2" << endl;
		out << "__string_" << i << ": .asciiz \"";
		for (unsigned int j = 0; j < string_table[i].size(); j++) {
			if (string_table[i][j] == '\\') {
				out << "\\\\";
			}
			else {
				out << string_table[i][j];
			}
		}
		out << "\"" << endl;
	}
}
void dumpfunchead(ostream &out) {
	out << dump_curfunc->iden << ":" << endl;
	unsigned int stackframesize = calcstack();
	ASM_ADDIU(REG_SP, REG_SP, (-(int)stackframesize));
	ASM_SW(REG_RA, dump_curfunc->stackinfo.ra, REG_SP);
	if (dump_curfunc->iden != "main") {
		for (unsigned int i = 0; i < 8; i++) {
			ASM_SW("$" << (16 + i), dump_curfunc->stackinfo.regs + i * 4, REG_SP);
		}
	}
}
void dumpfuncbody(ostream &out) {
	auto qlist = *(dump_curfunc->qlist);
	for (unsigned int i = 0; i < dump_curfunc->qlist->size(); i++) {
		dumpquat(out, qlist[i]);
	}
}
void loadreg(ostream &out, Operand a, unsigned int r) {
	if (a.value.entry->isglobal) {
		out << "\tlw " << REG(r) << "," << "__glbl_var_" << a.value.entry->iden << endl;
		return;
	}
	ASM_LW(REG(r), getoffset(a), REG_SP);
}
void dumpquat(ostream &out, Quaterntion* q) {
	for (unsigned int i = 0; i < q->labels.size(); i++) {
		out << "__label_" << q->labels[i].value.temp << ":" << endl;
	}
	unsigned int ra, rb, rr;
	if (q->op == add || q->op == sub || q->op == multiple || q->op == divide) {
		out << "#calculate" << endl;
		ra = getreg(q->a, out, false);
		rb = getreg(q->b, out, false);
		rr = getreg(q->r, out, true);
		if (q->op == add) {
			ASM_ADDU(REG(rr), REG(ra), REG(rb));
		}
		else if (q->op == sub) {
			ASM_SUBU(REG(rr), REG(ra), REG(rb));
		}
		else if (q->op == multiple) {
			ASM_MUL(REG(rr), REG(ra), REG(rb));
		}
		else if (q->op == divide) {
			ASM_DIV(REG(rr), REG(ra), REG(rb));
		}
		if (q->r.optype == OPENTRY) {
			if (q->r.value.entry->isglobal) {
				if (q->r.value.entry->type == CHAR) {
					out << "\tsb " << REG(rr) << ",__glbl_var_" << q->r.value.entry->iden << endl;
				}
				else {
					out << "\tsw " << REG(rr) << ",__glbl_var_" << q->r.value.entry->iden << endl;
				}
			}
			else {
				if (q->r.value.entry->type == CHAR) {
					ASM_ANDI(rr, rr, 0xff);
				}
			}
		}
	}
	else if (q->op == Operator::move) {
		out << "#move" << endl;
		ra = getreg(q->a, out, false);
		if (q->r.optype == OPENTRY) {
			if (q->r.value.entry->isglobal) {
				if (q->r.value.entry->type == INT) {
					out << "\tsw " << REG(ra) << ",__glbl_var_" << q->r.value.entry->iden << endl;
				}
				else if (q->r.value.entry->type == CHAR) {
					out << "\tsb " << REG(ra) << ",__glbl_var_" << q->r.value.entry->iden << endl;
				}
				else {
					cout << "Internal Error, Assembly.cpp " << __LINE__ << endl;
				}
			}
		}
		else {
			rr = getreg(q->r, out, true);
			if (q->r.optype == OPENTRY) {
				if (q->r.value.entry->type == CHAR) {
					ASM_ANDI(REG(rr), REG(ra), 0xff);
				}
			}
			else {
				ASM_MOVE(REG(rr), REG(ra));
			}
		}
	}
	else if (q->op == call) {
		argcount = 0;
		out << "#call" << endl;
		ASM_JAL(q->a.value.entry->iden);
	}
	else if (q->op == jmp) {
		out << "#jmp" << endl;
		ASM_J("__label_" << q->a.value.temp);
	}
	else if (q->op == bne) {
		out << "#bne" << endl;
		ra = getreg(q->a, out, false);
		rb = getreg(q->b, out, false);
		ASM_BNE(REG(ra), REG(rb), "__label_" << q->r.value.temp);
		resetreg();
	}
	else if (q->op == index) {
		out << "#index" << endl;
		assert(q->r.optype == OPTEMP || q->r.optype == OPENTRY);
		ra = getarray(q->a, out);
		rb = getreg(q->b, out, false);
		if (q->a.value.entry->type == CHARARRAY) {
			ASM_ADDU(REG(ra), REG(ra), REG(rb));
			rr = getreg(q->r, out, true);
			ASM_LB(REG(rr), 0, REG(ra));
		}
		else if (q->a.value.entry->type == INTARRAY) {
			ASM_SLL(REG(rb), REG(rb), 2);
			ASM_ADDU(REG(ra), REG(ra), REG(rb));
			rr = getreg(q->r, out, true);
			ASM_LW(REG(rr), 0, REG(ra));
		}
	}
	else if (q->op == push) {
		out << "#push" << endl;
		ra = getreg(q->a, out, false);
		if (q->a.type == CHAR || q->a.type == CONSTCHAR) {
			ASM_SB(REG(ra), argcount * 4, REG_SP);
		}
		else {
			ASM_SW(REG(ra), argcount * 4, REG_SP);
		}
		argcount++;
	}
	else if (q->op == ret) {
		out << "#ret" << endl;
		ra = getreg(q->a, out, false);
		ASM_MOVE(REG_V0, REG(ra));
		if (dump_curfunc->type == CHARFUNC) {
			ASM_ANDI(REG_V0, REG_V0, 0xff);
		}
		ASM_J("$end_" << dump_curfunc->iden);
	}
	else if (q->op == nret) {
		out << "#nret" << endl;
		ASM_J("$end_" << dump_curfunc->iden);
	}
	else if (q->op == result) {
		out << "#result" << endl;
		rr = getreg(q->r, out, true);
		ASM_MOVE(REG(rr), REG_V0);
	}
	else if (q->op == assigni) {//a index b res r array
		out << "#assigni" << endl;
		if (q->r.value.entry->isglobal) {
			ra = getreg(q->a, out, false);
			ASM_SLL(REG(ra), REG(ra), 2);
			rb = getreg(q->b, out, false);
			out << "\tsw " << REG(rb) << ",__glbl_array_" << q->r.value.entry->iden << "(" << REG(ra) << ")" << endl;
		}
		else {
			rb = getreg(q->b, out, false);
			rr = getarray(q->r, out);
			ra = getreg(q->a, out, false);
			ASM_SLL(REG(ra), REG(ra), 2);
			ASM_ADDU(REG(rr), REG(rr), REG(ra));
			ASM_SW(REG(rb), 0, REG(rr));
		}
	}
	else if (q->op == assignc) {
		out << "#assignc" << endl;
		if (q->r.value.entry->isglobal) {
			ra = getreg(q->a, out, false);
			rb = getreg(q->b, out, false);
			out << "\tsb " << REG(rb) << ",__glbl_array_" << q->r.value.entry->iden << "(" << REG(ra) << ")" << endl;
		}
		else {
			rb = getreg(q->b, out, false);
			rr = getarray(q->r, out);
			ra = getreg(q->a, out, false);
			ASM_ADDU(REG(rr), REG(rr), REG(ra));
			ASM_SB(REG(rb), 0, REG(rr));
		}
	}
	else if (q->op == readi) {
		out << "#readi" << endl;
		ASM_LI(REG_V0, 5);
		ASM_SYSCALL;
		ra = getreg(q->a, out, true);
		ASM_MOVE(REG(ra), REG_V0);
	}
	else if (q->op == readc) {
		out << "#readc" << endl;
		ASM_LI(REG_V0, 12);
		ASM_SYSCALL;
		ra = getreg(q->a, out, true);
		ASM_MOVE(REG(ra), REG_V0);
	}
	else if (q->op == print) {
		out << "#print" << endl;
		if (q->a.type == CONSTCHAR || q->a.type == CHAR) {
			ASM_LI(REG_V0, 11);
			ra = getreg(q->a, out, false);
			ASM_MOVE(REG_A0, REG(ra));
			ASM_SYSCALL;
		}
		else {
			ASM_LI(REG_V0, 1);
			ra = getreg(q->a, out, false);
			ASM_MOVE(REG_A0, REG(ra));
			ASM_SYSCALL;
		}
	}
	else if (q->op == printstring) {
		out << "#printstring" << endl;
		ASM_LI(REG_V0, 4);
		out << "\tla $a0,__string_" << q->a.value.temp - 1 << endl;
		ASM_SYSCALL;
	}
	else if (q->op == equalto) {
		out << "#equalto" << endl;
		ra = getreg(q->a, out, false);
		rb = getreg(q->b, out, false);
		ASM_BNE(REG(ra), REG(rb), "__label_" << q->r.value.temp);
		resetreg();
	}
	else if (q->op == notequal) {
		out << "#notequal" << endl;
		ra = getreg(q->a, out, false);
		rb = getreg(q->b, out, false);
		ASM_BEQ(REG(ra), REG(rb), "__label_" << q->r.value.temp);
		resetreg();
	}
	else if (q->op == greaterthan) {
		out << "#greaterthan" << endl;
		ra = getreg(q->a, out, false);
		rb = getreg(q->b, out, false);
		ASM_BLE(REG(ra), REG(rb), "__label_" << q->r.value.temp);
		resetreg();
	}
	else if (q->op == greaterequal) {
		out << "#greaterequal" << endl;
		ra = getreg(q->a, out, false);
		rb = getreg(q->b, out, false);
		ASM_BLT(REG(ra), REG(rb), "__label_" << q->r.value.temp);
		resetreg();
	}
	else if (q->op == lessthan) {
		out << "#lessthan" << endl;
		ra = getreg(q->a, out, false);
		rb = getreg(q->b, out, false);
		ASM_BGE(REG(ra), REG(rb), "__label_" << q->r.value.temp);
		resetreg();
	}
	else if (q->op == lessequal) {
		out << "#lessthan" << endl;
		ra = getreg(q->a, out, false);
		rb = getreg(q->b, out, false);
		ASM_BGT(REG(ra), REG(rb), "__label_" << q->r.value.temp);
		resetreg();
	}
}
unsigned char getarray(Operand a, ostream &out) {
	assert(a.optype == OPENTRY);
	assert(a.value.entry->type == INTARRAY || a.value.entry->type == CHARARRAY);
	if (a.value.entry->isglobal) {
		out << "\tla $9,__glbl_array_" << a.value.entry->iden << endl;
	}
	else {
		//TODO ???
		ASM_LI(REG(9), a.value.entry->varoffset);
		ASM_ADDU(REG(9), REG(9), REG_SP);
	}
	return 9;
}
void dumpfuncend(ostream &out) {
	out << "$end_" << dump_curfunc->iden << ":" << endl;
	ASM_LW(REG_RA, dump_curfunc->stackinfo.ra, REG_SP);
	if (dump_curfunc->iden != "main") {
		for (unsigned int i = 0; i < 8; i++) {
			ASM_LW("$" << (16 + i), dump_curfunc->stackinfo.regs + i * 4, REG_SP);
		}
	}
	ASM_ADDIU(REG_SP, REG_SP, dump_curfunc->stackinfo.size);
	ASM_JR(REG_RA);
}
unsigned int getoffset(Operand op) {
	if (op.optype == OPENTRY) {
		return op.value.entry->varoffset;
	}
	else if (op.optype == OPTEMP) {
		return dump_curfunc->stackinfo.temp + op.value.temp * 4;
	}
	cout << "Internal error, Assembly.cpp " << __LINE__ << endl;
	return 0;
}
unsigned char getreg(Operand op, ostream &out, bool NoNeedValue) {
	static unsigned int tempconst = 24;// use t8 t9
	unsigned int offset;
	if (op.optype == OPCCHAR || op.optype == OPCINT) {
		if (op.value.constint == 0) {
			return 0;
		}
		tempconst = tempconst == 24 ? 25 : 24;
		if (op.optype == OPCCHAR) {
			ASM_LI(REG(tempconst), (int)op.value.constchar);
		}
		else {
			ASM_LI(REG(tempconst), op.value.constint);
		}
		return tempconst;
	}
	//TODO PARAINT PARACHAR
	if (op.optype == OPENTRY) {
		if (op.type == PARACHAR || op.type == PARAINT) {
			ParaEntry* pe = getpara(op.value.entry->iden);
			assert(pe != NULL);
			offset = pe->offset;
			goto getreg_from_offset;
		}
		if (op.value.constint == 0) return 0;
		tempconst = tempconst == 24 ? 25 : 24;
		if (op.type == CONSTINT) {
			ASM_LI(REG(tempconst), op.value.constint);
			return tempconst;
		}
		if (op.type == CONSTCHAR) {
			ASM_LI(REG(tempconst), (int)((char)op.value.constchar));
			return tempconst;
		}
		if (op.value.entry->isglobal) {
			if (!NoNeedValue)
				out << "\tlw $" << tempconst << ",__glbl_var_" << op.value.entry->iden << endl;
			return tempconst;
		}
	}
	offset = getoffset(op);
getreg_from_offset:
	unsigned char r = findreg(offset);
	if (r == 0)
		r = applyreg(out, offset, NoNeedValue);
	return r;
}
unsigned char findreg(unsigned int offset) {
	for (unsigned char i = 0; i < 8; i++) {
		if (offset == regcontent[i]) {
			return 16 + i;
		}
	}
	return 0;
}
unsigned char applyreg(ostream &out, unsigned int offset, bool NoNeedValue) {
	if (regnum == 8) regnum = 0;
	if (regcontent[regnum] != -1) {
		ASM_SW("$" << (16 + regnum), (unsigned int)regcontent[regnum], REG_SP);
	}
	if (!NoNeedValue)
		ASM_LW("$" << (16 + regnum), offset, REG_SP);
	regcontent[regnum] = offset;
	return 16 + regnum++;
}
void resetreg() {
	for (unsigned char i = 0; i < 8; i++) {
		regcontent[i] = -1;
	}
	regnum = 0;
}
void dumpasm(ostream &out) {
	dumpdata(out);
	out << ".text" << endl;
	out << "\t j main" << endl;
	for (unsigned int i = 0; i < glbl_symtable.size(); i++) {
		if (glbl_symtable[i]->type == VOID || glbl_symtable[i]->type == INTFUNC || glbl_symtable[i]->type == CHARFUNC) {
			dump_curfunc = glbl_symtable[i];
			resetreg();
			dumpfunchead(out);
			dumpfuncbody(out);
			dumpfuncend(out);
		}
	}
}
unsigned int calcstack() {
	auto sinfo = &(dump_curfunc->stackinfo);
	unsigned int maxargs = getmaxargs();
	//TODO how many args?
	unsigned int result = 0;
	sinfo->arguments = 0;
	result += maxargs * 4;
	sinfo->regs = result;
	result += 8 * 4;
	sinfo->ra = result;
	result += 4;
	auto psym = *(dump_curfunc->partial_symtable);
	//array
	sinfo->array = result;
	for (unsigned int i = 0; i < psym.size(); i++) {
		if (psym[i]->type == INTARRAY) {
			psym[i]->varoffset = result;
			result += psym[i]->length * 4;
		}
		else if (psym[i]->type == CHARARRAY) {
			psym[i]->varoffset = result;
			result += psym[i]->length + 4 - psym[i]->length % 4;
		}
	}
	sinfo->temp = result;
	//temp
	result += dump_curfunc->tmpindex * 4;
	sinfo->var = result;
	for (unsigned int i = 0; i < psym.size(); i++) {
		if (psym[i]->type == CHAR || psym[i]->type == INT) {
			psym[i]->varoffset = result;
			result += 4;
		}
	}
	for (unsigned int i = 0; i < dump_curfunc->paralist.size(); i++) {
		dump_curfunc->paralist[i].offset = result + 4 * i;
	}
	sinfo->size = result;
	return result;
}
unsigned int getmaxargs() {
	unsigned int result = 0;
	auto qlist = *(dump_curfunc->qlist);
	for (unsigned int i = 0; i < dump_curfunc->qlist->size(); i++) {
		if (qlist[i]->op == call) {
			if (qlist[i]->a.value.entry->paralist.size() > result) {
				result = qlist[i]->a.value.entry->paralist.size();
			}
		}
	}
	if (result < 4) return 4;
	else return result;
}
ParaEntry* getpara(string iden) {
	for (unsigned int i = 0; i < dump_curfunc->paralist.size(); i++) {
		if (dump_curfunc->paralist[i].iden == iden) {
			auto p = new ParaEntry(dump_curfunc->paralist[i]);
			return p;
		}
	}
	return NULL;
}