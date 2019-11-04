#pragma once
#define REG_SP "$29"
#define REG_RA "$31"
#define REG_K0 "$26"
#define REG_K1 "$27"
#define REG_V0 "$2"
#define REG_A0 "$a0"
#define ASM_ADDIU(r,a,im) out << "\taddiu " << r << "," <<  a << "," << im << endl
#define ASM_SW(r,of,base) out << "\tsw " << r << "," << of << "(" << base << ")" << endl
#define ASM_SB(r,of,base) out << "\tsb " << r << "," << of << "(" << base << ")" << endl
#define ASM_LW(r,of,base) out << "\tlw " << r << "," << of << "(" << base << ")" << endl
#define ASM_LB(r,of,base) out << "\tlb " << r << "," << of << "(" << base << ")" << endl
#define ASM_SLL(r,a,im) out << "\tsll " << r << "," << a << "," << im << endl
#define ASM_MOVE(r,a) out << "\tmove " << r << "," << a << endl
#define ASM_ANDI(r,a,im) out << "\tandi " << r << "," << a <<"," << im << endl
#define ASM_BNE(a,b,l) out << "\tbne " << a << "," << b <<"," << l<< endl
#define ASM_ADDU(r,a,b) out << "\taddu " << r << "," << a << "," << b << endl
#define ASM_SUBU(r,a,b) out << "\tsubu " << r << "," << a << "," << b << endl
#define ASM_MUL(r,a,b) out << "\tmul " << r << "," << a << "," << b << endl
#define ASM_DIV(r,a,b) out << "\tdiv " << r << "," << a << "," << b << endl
#define ASM_BEQ(a,b,l) out << "\tbeq " << a << "," << b <<"," << l<< endl
#define ASM_BLE(a,b,l) out << "\tble " << a << "," << b <<"," << l<< endl
#define ASM_BLT(a,b,l) out << "\tblt " << a << "," << b <<"," << l<< endl
#define ASM_BGE(a,b,l) out << "\tbge " << a << "," << b <<"," << l<< endl
#define ASM_BGT(a,b,l) out << "\tbgt " << a << "," << b <<"," << l<< endl
#define ASM_SYSCALL out << "\tsyscall" << endl
#define ASM_LI(r,a) out << "\tli " << r << "," << (a) << endl
#define ASM_JAL(_) out << "\tjal " << (_) << endl
#define ASM_NOP out << "\tnop" << endl
#define ASM_JR(r) out << "\tjr " << r << endl
#define ASM_J(_) out << "\tj " << _ << endl
#define REG(_) "$" << (_)
void dumpdata(ostream & out);

void dumpfunchead(ostream & out);

void dumpfuncbody(ostream & out);

void dumpquat(ostream & out, Quaterntion * q);

unsigned char getarray(Operand a, ostream & out);

void dumpfuncend(ostream & out);

unsigned int getoffset(Operand op);

unsigned char getreg(Operand op, ostream & out, bool NoNeedValue);

unsigned char findreg(unsigned int offset);

unsigned char applyreg(ostream & out, unsigned int offset, bool NoNeedValue);

void resetreg();

void dumpasm(ostream & out);

unsigned int calcstack();

unsigned int getmaxargs();

ParaEntry * getpara(string iden);
