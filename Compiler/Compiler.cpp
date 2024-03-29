#include "pch.h"
#include <iostream>
#include <fstream>
#include <vector>
#include "Compiler.h"
#include "Operand.h"
#include "error.h"
#include "SymbolTable.h"
#include "Quaterntion.h"
#include "Assembly.h"
#include <assert.h>
using namespace std;
string curiden;
bool curarray;
unsigned int curlength;
enum TYPE curfunctype;
enum TYPE curvartype;
vector<TableEntry*> glbl_symtable;
vector<TableEntry*> *cur_symtable;
vector<Quaterntion*>* cur_qlist;
vector<string> string_table;
STRINGID _string_index = 1;
TableEntry *cur_funcentry;
bool force_int = false;
Operand curop;
const char* operators[] =  { (const char*)"index", (const char*)"push", (const char*)"pop", (const char*)"call",
(const char*)"result",(const char*)"add",(const char*)"sub",(const char*)"multiple",(const char*)"divide",(const char*)"lessthan",
(const char*)"lessequal",(const char*)"greaterthan",(const char*)"greaterequal",(const char*)"equalto",(const char*)"notequal",
(const char*)"label",(const char*)"jmp",(const char*)"nop",(const char*)"print",(const char*)"printstring",
(const char*)"printformat",(const char*)"readi",(const char*)"readc",(const char*)"ret",(const char*)"nret",
(const char*)"bne",(const char*)"move",(const char*)"assigni",(const char*)"assignc" };
int main() {
	file_init();
	//puts(buffer);
	program();
	printq();
	ofstream out;
	out.open("out.asm");
	dumpasm(out);
}
void printtable() {
	/*
	for (unsigned int i = 0; i < glbl_symtable.size(); i++) {
		cout << glbl_symtable[i].iden << glbl_symtable[i].type << glbl_symtable[i].value << endl;
	}
	*/
}
bool pushentry(TableEntry entry) {

	for (unsigned int i = 0; i < (*cur_symtable).size(); i++) {
		if ((*cur_symtable)[i]->iden == entry.iden) {
			printf("entry.iden %s ", entry.iden.c_str());
			semantic_error(E_DUPLICATE_DECLARATION);
			return false;
		}
	}
	if (entry.iden == "") {
		cout << "DEBUG:NULL IDEN" << endl;
	}
	TableEntry *e = new TableEntry(entry);
	cur_symtable->push_back(e);
	return true;
}
bool pushvar(enum TYPE type, string iden) {
	TableEntry e(type, iden, 0, 0);
	return pushentry(e);
}
STRINGID pushstring(string s) {
	string_table.push_back(s);
	return _string_index++;
}
void program() {
	bool func_start = false;
	bool main_start = false;
	getsym();
	cur_symtable = &glbl_symtable;
	constblock();
	printtable();
	enum SYMBOL cursym;
	while (!main_start) {
		if (symbol == CHARSY || symbol == INTSY) {
			cursym = symbol;
			getsym();
			if (symbol == IDEN) {
				curiden = token;
				getsym();
				if (symbol == LPARSY) {
					func_start = true;
					curfunctype = cursym == INTSY ? INTFUNC : CHARFUNC;
					cur_symtable = &glbl_symtable;
					TableEntry e(curfunctype, curiden, 0, 0);
					vector<Quaterntion*>* _qlist = new vector<Quaterntion*>();
					e.setqlist(_qlist);
					cur_qlist = _qlist;
					pushentry(e);
					cur_funcentry = getEntry(curiden);
					rfunc();
					continue;
				}
				else {
					if (!func_start) {
						curvartype = cursym == INTSY ? INT : CHAR;
						glbl_vardef();
					}
					else {
						error(E_VAR_DEF_IN_FUNC_AREA);
					}
				}
			}
			else if (symbol == EOFSY) {
				printf("file ended\n");
				exit(0);
			}
			else {
				error(E_EXPECTED_IDEN); printf("line:%d\n", __LINE__);
				break;
			}
		}
		else if (symbol == VOIDSY) {
			curfunctype = VOID;
			getsym();
			if (symbol == MAINSY) {
				cur_symtable = &glbl_symtable;
				TableEntry e(curfunctype, "main", 0, 0);
				vector<Quaterntion*>* _qlist = new vector<Quaterntion*>();
				cur_qlist = _qlist;
				e.setqlist(_qlist);
				pushentry(e);
				cur_funcentry = getEntry("main");
				main_start = true;
				getsym();
				if (symbol == LPARSY) {
					getsym();
					if (symbol == RPARSY) {
						getsym();
						vector<TableEntry*>* functable = new vector<TableEntry*>();
						cur_symtable = functable;
						cur_funcentry->partial_symtable = functable;
						funcbody();
						printf("This is a main func\n");
					}
					else {
						error(E_MAIN_EXPECTED_RPAR);
					}
				}
				else {
					error(E_EXPECTED_LPAR);
				}
			}
			else if (symbol == IDEN) {
				//curfunc = token;
				curiden = token;
				cur_symtable = &glbl_symtable;
				TableEntry e(VOID, curiden, 0, 0);
				vector<Quaterntion*>* _qlist = new vector<Quaterntion*>();
				e.setqlist(_qlist);
				pushentry(e);
				cur_qlist = _qlist;
				cur_funcentry = getEntry(curiden);
				getsym();
				nfunc();
			}
			else {
				error(E_VOIDFUNC_INVALID_IDEN);
			}
		}
		else {
			if (symbol != EOFSY) printf("next symbol can't recognize\n");
			break;
		}
	}

}
//tested
void constblock() {
	while (1) {
		if (symbol == CONSTSY) {
			getsym();
			constdef();
		}
		else {
			break;
		}
	}
}
//tested
void constdef() {
	string curiden;
	int curvalue;
	if (symbol == INTSY) {
	next_const_int_assignment:
		getsym();
		if (symbol == IDEN) {
			curiden = token;
			getsym();
			if (symbol == ASSIGNSY) {
				getsym();
				if (all_integer()) {
					curvalue = num;
					if (symbol == COMMASY) {
						TableEntry entry(CONSTINT, curiden, curvalue, 0);
						pushentry(entry);
						goto next_const_int_assignment;
					}
					else if (symbol == SEMISY) {
						TableEntry entry(CONSTINT, curiden, curvalue, 0);
						pushentry(entry);
						printf("This is a const int definition\n");
						getsym();
						return;
					}
					else {
						error(E_CONSTDEF_NOT_END);
					}
				}
				else {
					error(E_CONST_INT_ASSIGN_TYPE);
				}
			}
			else {
				error(E_CONST_NOT_ASSIGNSY);
			}
		}
		else {
			error(E_EXPECTED_IDEN); printf("line:%d\n", __LINE__);
		}
	}
	else if (symbol == CHARSY) {
	next_const_char_assignment:
		getsym();
		if (symbol == IDEN) {
			curiden = token;
			getsym();
			if (symbol == ASSIGNSY) {
				getsym();
				if (symbol == CHARCONST) {
					curvalue = constchar;
					getsym();
					if (symbol == COMMASY) {
						TableEntry entry(CONSTCHAR, curiden, curvalue, 0);
						pushentry(entry);
						goto next_const_char_assignment;
					}
					else if (symbol == SEMISY) {
						TableEntry entry(CONSTCHAR, curiden, curvalue, 0);
						pushentry(entry);
						printf("This is a const char definition\n");
						getsym();
						return;
					}
					else {
						error(E_CONSTDEF_NOT_END);
					}
				}
				else {
					error(E_CONST_CHAR_ASSIGN_TYPE);
				}
			}
			else {
				error(E_CONST_NOT_ASSIGNSY);
			}
		}
		else {
			error(E_EXPECTED_IDEN); printf("line:%d\n", __LINE__);
		}
	}
	else {
		error(E_NO_CONST_TYPE);
	}
}
//finished
bool parameterlist() {
	getsym();
	if (symbol == RPARSY) {
		printf("This is a parameter list\n");
		getsym();
		return true;
	}
parameterlist_start:
	if (symbol == CHARSY || symbol == INTSY) {
		enum TYPE curtype = symbol == INTSY ? PARAINT : PARACHAR;
		getsym();
		if (symbol == IDEN) {
			string iden = token;
			cur_funcentry->pushpara(curtype, iden);
			pushvar(curtype, iden);
			getsym();
			if (symbol == COMMASY) {
				getsym();
				goto parameterlist_start;
			}
			else if (symbol == RPARSY) {
				printf("This is a parameter list\n");
				getsym();
				return true;
			}
			else {
				error(E_PARAMETER_NOT_END);
				return false;
			}
		}
		else {
			error(E_EXPECTED_IDEN); printf("line:%d\n", __LINE__);
			return false;
		}
	}
	else {
		error(E_INVALID_PARAMETER_TYPE);
		return false;
	}
}
//finished
bool all_integer() {
	if (symbol == INTEGER) {
		getsym();
		return true;
	}
	else if (symbol == ADDSY) {
		getsym();
		if (symbol == INTEGER) {
			getsym();
			return true;
		}
		else {
			error(E_EXPECTED_INTEGER);
			return false;
		}
	}
	else if (symbol == SUBSY) {
		getsym();
		if (symbol == INTEGER) {
			num = -num;
			getsym();
			return true;
		}
		else {
			error(E_EXPECTED_INTEGER);
			return false;
		}
	}
	else {
		return false;
	}
	return false;
}
//finished
bool var_iden() {
	curarray = false;
	if (symbol == IDEN) {
		curiden = token;
		getsym();
		if (symbol == LBRACKSY) {
			getsym();
			if (symbol == INTEGER) {
				curlength = num;
				getsym();
				if (symbol == RBRACKSY) {
					curarray = true;
					getsym();
					return true;
				}
				else {
					error(E_EXPECTED_RBRACKSY);
					return false;
				}
			}
			else {
				error(E_EXPECTED_UNSIGNED);
				return false;
			}
		}
		else {
			return true;
		}
	}
	else {
		error(E_EXPECTED_IDEN);
		return false;
	}
}
//todo
void varblock() {
	while (true) {
		if (symbol == INTSY || symbol == CHARSY) {
			enum TYPE curtype = symbol == INTSY ? INT : CHAR;
			getsym();
			while (true) {
				if (var_iden()) {
					if (curarray) {
						curtype = curtype == INT ? INTARRAY : curtype == CHAR ? CHARARRAY : ERRORTYPE;
					}
					TableEntry e(curtype, curiden, 0, curarray ? curlength : 0);
					pushentry(e);
					if (symbol == COMMASY) {
						getsym();
						continue;
					}
					else if (symbol == SEMISY) {
						printf("This is a var definition in func\n");
						getsym();
						break;
					}
					else {
						error(E_VAR_DEF_NOT_END);
						break;
					}
				}
				else {
					break;
				}
			}
		}
		else {
			return;
		}
	}
}
enum TYPE vartype() {
	if (curarray) {
		return curvartype == INT ? INTARRAY : curvartype == CHAR ? CHARARRAY : ERRORTYPE;
	}
	else return curvartype;
}
void glbl_vardef() {
	assert(cur_symtable == &glbl_symtable);
	bool partial_def = true;
var_def_start:
	if (symbol == COMMASY) {
		if (partial_def) {
			TableEntry e0(curvartype, curiden, 0, 0);
			e0.isglobal = true;
			pushentry(e0);
		}
		getsym();
		while (1) {
			if (var_iden()) {
				if (symbol == COMMASY) {
					TYPE _vartype = vartype();
					TableEntry e0(_vartype, curiden, 0, curarray ? curlength : 0);
					e0.isglobal = true;
					pushentry(e0);
					getsym();
				}
				else if (symbol == SEMISY) {
					TableEntry e0(vartype(), curiden, 0, curarray ? curlength : 0);
					e0.isglobal = true;
					pushentry(e0);
					printf("This is a var definition\n");
					getsym();
					return;
				}
				else {
					error(E_VAR_DEF_NOT_END);
					return;
				}
			}
			else {
				error(E_EXPECTED_IDEN); printf("line:%d\n", __LINE__);
				return;
			}
		}
	}
	else if (symbol == SEMISY) {
		curarray = false;
		TableEntry e0(curvartype, curiden, 0, 0);
		e0.isglobal = true;
		pushentry(e0);
		printf("This is a var definition\n");
		getsym();
		return;
	}
	else if (symbol == LBRACKSY && partial_def) {
		curarray = true;
		getsym();
		if (symbol == INTEGER) {
			getsym();
			if (symbol == RBRACKSY) {
				TYPE _vartype = vartype();
				TableEntry e0(_vartype, curiden, 0, curlength);
				e0.isglobal = true;
				pushentry(e0);
				getsym();
				if (symbol == SEMISY) {
					getsym();
					return;
				}
				partial_def = false;
				goto var_def_start;
			}
			else {
				error(E_EXPECTED_RBRACKSY);
			}
		}
		else {
			error(E_EXPECTED_UNSIGNED);
		}
	}
	else {
		error(E_VAR_DEF_NOT_END);
		return;
	}
}

//finished
bool value_parameters(TableEntry* func) {
	vector<Operand> arglist;
	vector<Quaterntion*> qlist;
	while (true) {
		if (expression()) {
		value_parameters_expression:
			arglist.push_back(curop);
			
			if (curop.optype == OPENTRY && (curop.type == INT || curop.type == CHAR || curop.type == PARACHAR || curop.type == PARAINT)) {
				Operand tmpvar = genTemp();
				emit(Operator::move, curop, genNULL(), tmpvar);
				curop = tmpvar;
			}
			{
				auto q = new Quaterntion;
				q->op = push;
				q->a = curop;
				q->b = genNULL();
				q->r = genNULL();
				qlist.push_back(q);
			}
			//emit(push, curop, genNULL(), genNULL());
			if (symbol == COMMASY) {
				getsym();
				if (expression()) {
					goto value_parameters_expression;
				}
				else {
					error(E_EXPECTED_EXPRESSION);
					return false;
				}
			}
			else if (symbol == RPARSY) {
				if (arglist.size() != func->paralist.size()) {
					semantic_error(E_FUNCTION_PARA_MISMATCH);
					cout << arglist.size() << func->paralist.size() << endl;
					return false;
				}
				for (unsigned int i = 0; i < arglist.size(); i++) {
					TYPE para_type = func->paralist[i].type == PARAINT ? INT : func->paralist[i].type == PARACHAR ? CHAR : VOID;
					if (arglist[i].type != para_type) {
						if ((arglist[i].type == INTFUNC || arglist[i].type == CONSTINT || arglist[i].type == PARAINT) && para_type == INT ||
							(arglist[i].type == CHARFUNC || arglist[i].type == CONSTCHAR || arglist[i].type == PARACHAR) && para_type == CHAR) continue;
						else
							semantic_error(E_TYPE_MISMATCH);
					}
				}
				for (unsigned int i = 0; i < qlist.size(); i++) {
					addQuaterntion(qlist[i]);
				}
				printf("This is a value parameter list\n");
				getsym();
				return true;
			}
			else {
				error(E_INVALID_VALUE_PARAMETERS);
				return false;
			}
		}
		else if (symbol == RPARSY) {
			if (func->paralist.empty()) {
				printf("This is a value parameter list\n");
				getsym();
				return true;
			}
			else {
				semantic_error(E_FUNCTION_PARA_MISMATCH);
				return false;
			}
		}
		else {
			error(E_INVALID_VALUE_PARAMETERS);
			return false;
		}
	}
}
void statementblock() {
	constblock();
	varblock();
	allstatements();
}
//finished
bool factor() {
	if (all_integer()) {
		Operand op = genConstInt(num);
		curop = op;
		return true;
	}
	else if (symbol == CHARCONST) {
		Operand op = genConstChar(constchar);
		curop = op;
		getsym();
		return true;
	}
	else if (symbol == LPARSY) {
		getsym();
		if (expression()) {
			if (symbol == RPARSY) {
				printf("This is an expression factor\n");
				getsym();
				return true;
				force_int = true;
			}
			else {
				error(E_EXPECTED_RPAR);
				return false;
			}
		}
		else {
			error(E_EXPECTED_EXPRESSION);
			return false;
		}
	}
	else if (symbol == IDEN) {
		string _id = token;
		getsym();
		if (symbol == LBRACKSY) {
			getsym();
			if (expression()) {
				if (symbol == RBRACKSY) {
					getsym();
					Operand _ar_result = genTemp();
					TableEntry* ar_entry = getEntry(_id);
					if (ar_entry->type != INTARRAY && ar_entry->type != CHARARRAY) {
						semantic_error(E_IDEN_NOT_ARRAY);
					}
					Operand _ar = genEntry(ar_entry);
					Operand _ar_index = curop;
					emit(index, _ar, _ar_index, _ar_result);
					curop = _ar_result;
					if (ar_entry->type == CHARARRAY) curop.type = CHAR;
					return true;
				}
				else {
					error(E_EXPECTED_RBRACKSY);
					return false;
				}
			}
			else {
				error(E_EXPECTED_EXPRESSION);
				return false;
			}
		}
		else if (symbol == LPARSY) {
			TableEntry* e = getEntry(_id);
			if (e->type != INTFUNC && e->type != CHARFUNC && e->type != VOID) {
				semantic_error(E_IDEN_NOT_FUNC);
			}
			else if (e->type == VOID) {
				warning(E_FUNC_NO_RETURN);
			}
			getsym();
			bool value_valid = value_parameters(e);
			if (value_valid) {
				Operand _function = genEntry(e);
				emit(call, _function, genNULL(), genNULL());
				Operand _function_result = genTemp();
				_function_result.type = e->type;
				emit(result, genNULL(), genNULL(), _function_result);
				curop = _function_result;
			}
			return value_valid;
		}
		else {
			TableEntry* e = getEntry(_id);
			if (e->type != INT && e->type != CHAR && e->type != PARAINT && e->type != PARACHAR && e->type != CONSTINT && e->type != CONSTCHAR) {
				semantic_error(E_IDEN_FUNC_OR_ARRAY);
			}
			curop = genEntry(e);
			return true;
		}
	}
	else {
		return false;
	}
}
//finished
bool term() {
	if (factor()) {
	term_mulsy:
		Operand _factor0 = curop;
		if (symbol == MULSY || symbol == DIVSY) {
			Operator _op = symbol == MULSY ? multiple : divide;
			getsym();
			if (factor()) {
				Operand _factor1 = curop;
				Operand _result = genTemp();
				_result.type = INT;
				curop = _result;
				emit(_op, _factor0, _factor1, _result);
				goto term_mulsy;
			}
			else {
				error(E_EXPECTED_FACTOR);
				return false;
			}
		}
		else {
			printf("This is a term\n");
			return true;
		}
	}
	else {
		return false;
	}
}
//finished
bool expression() {
	bool first_negative = false;
	force_int = false;
	if (symbol == ADDSY || symbol == SUBSY) {
		force_int = true;
		if (symbol == SUBSY) {
			first_negative = true;
		}
		getsym();
	}
	if (term()) {
		Operand _term0 = curop;
		if (first_negative) {
			Operand _tmp = genTemp();
			_tmp.type = INT;
			emit(sub, genConstInt(0), _term0, _tmp);
			curop = _tmp;
		}
	expression_addsy:
		_term0 = curop;
		if (symbol == ADDSY || symbol == SUBSY) {
			Operator _op = symbol == ADDSY ? add : sub;
			getsym();
			if (term()) {
				Operand _term1 = curop;
				Operand _result = genTemp();
				_result.type = INT;
				emit(_op, _term0, _term1, _result);
				curop = _result;
				goto expression_addsy;
			}
			else {
				error(E_EXPECTED_TERM);
				return false;
			}
		}
		else {
			printf("This is an expression\n");
			return true;
		}
	}
	else {
		return false;
	}
}
//finished
bool condition() {
	if (expression()) {
		Operand _a = curop;
		TYPE type_a = _a.type;
		if (force_int) type_a = INT;
		if (type_a == PARAINT || INTFUNC || CONSTINT)
			type_a = INT;
		if (type_a == PARACHAR || CHARFUNC || CONSTCHAR)
			type_a = CHAR;
		if (isRelationSymbol()) {
			Operator _op;
			if (symbol == LTSY) {
				_op = lessthan;
			}
			else if (symbol == LEQUSY) {
				_op = lessequal;
			}
			else if (symbol == GTSY) {
				_op = greaterthan;
			}
			else if (symbol == GEQUSY) {
				_op = greaterequal;
			}
			else if (symbol == EQUSY) {
				_op = equalto;
			}
			else if (symbol == NOTEQUSY) {
				_op = notequal;
			}
			getsym();
			if (expression()) {
				Operand _b = curop;
				TYPE type_b = _b.type;
				if (force_int)
					type_b = INT;
				if (type_b == PARAINT || INTFUNC || CONSTINT)
					type_b = INT;
				if (type_b == PARACHAR || CHARFUNC || CONSTCHAR)
					type_b = CHAR;
				if (type_a != type_b)
					semantic_error(E_TYPE_MISMATCH);
				emit(_op, _a, _b, genNULL());
				printf("This is a condition\n");
				return true;
			}
			else {
				error(E_EXPECTED_EXPRESSION);
				return false;
			}
		}
		else {
			if (curop.type == CHAR || curop.type == CONSTCHAR)
				semantic_error(E_CONDITION_NOT_INT);
			emit(notequal, _a, genConstInt(0), genNULL());
			printf("This is a condition\n");
			return true;
		}
	}
	else {
		error(E_EXPECTED_EXPRESSION);
		return false;
	}
}

bool caselist(TYPE switch_type) {
	Operand _exp = curop;
	vector<Quaterntion*> casejumps;
	Operand _const;
	Operand _nextcase = genLabel();
	Operand _end;
	TYPE case_type = VOID;
caselist_start:
	if (symbol == CASESY) {
		getsym();
		if (symbol == CHARCONST) {
			_const = genConstChar(constchar);
			getsym();
			case_type = CHAR;
		}
		else if (all_integer()) {
			_const = genConstInt(num);
			case_type = INT;
		}
		else {
			error(E_CASE_NO_CONST);
			return false;
		}
		if (switch_type != case_type) {
			semantic_error(E_TYPE_MISMATCH);
		}
		if (symbol == COLONSY) {
			emit(bne, _exp, _const, _nextcase);
			//emit(bnz, genNULL(), _nextcase, genNULL());
			getsym();
			if (symbol == LBRACESY) {
				getsym();
				if (allstatements()) {
					printf("This is one case{}\n");
					casejumps.push_back(emit(jmp, genNULL(), genNULL(), genNULL()));
					setLabel(_nextcase);
					_end = _nextcase;
					_nextcase = genLabel();
					goto caselist_start;
				}
				else {
					error(E_EXPECTED_ALLSTATEMENTS);
					return false;
				}
			}
			else if (statement()) {
				printf("This is one case \n");
				casejumps.push_back(emit(jmp, genNULL(), genNULL(), genNULL()));
				setLabel(_nextcase);
				_end = _nextcase;
				_nextcase = genLabel();
				goto caselist_start;
			}
			else {
				error(E_EXPECTED_ALLSTATEMENTS);
				return false;
			}
		}
		else {
			error(E_CASE_EXPECTED_COLON);
			return false;
		}
	}
	else if (symbol == DEFAULTSY) {
		getsym();
		if (symbol == COLONSY) {
			getsym();
			if (symbol == LBRACESY) {
				getsym();
				if (allstatements()) {
					if (symbol == RBRACESY) {
						getsym();
						_end = genLabel();
						setLabel(_end);
						for (unsigned int i = 0; i < casejumps.size(); i++) {
							casejumps[i]->a = _end;
						}
						printf("This is a case list\n");
						return true;
					}
					else {
						error(E_EXPECTED_RBRACE);
						return false;
					}
				}
				else {
					error(E_EXPECTED_ALLSTATEMENTS);
					return false;
				}
			}
			else if (symbol == RBRACESY) {
				_end = genLabel();
				setLabel(_end);
				for (unsigned int i = 0; i < casejumps.size(); i++) {
					casejumps[i]->a = _end;
				}
				getsym();
				printf("This is a case list\n");
				return true;
			}
			else {
				if (statement()) {
					if (symbol == RBRACESY) {
						_end = genLabel();
						setLabel(_end);
						for (unsigned int i = 0; i < casejumps.size(); i++) {
							casejumps[i]->a = _end;
						}
						getsym();
						printf("This is a case list\n");
						return true;
					}
					else {
						error(E_EXPECTED_RBRACE);
						return false;
					}
				}
				else {
					error(E_CASE_STATEMENT);
					return false;
				}
			}
		}
		else {
			error(E_CASE_EXPECTED_COLON);
			return false;
		}
	}
	else {
		error(E_INVALID_CASELIST);
		return false;
	}
}
bool statement() {
	if (symbol == IFSY) {
		getsym();
		if (symbol == LPARSY) {
			getsym();
			if (condition()) {
				Operand _label = genLabel();
				auto lastq = getlastquat();
				lastq->r = _label;
				if (symbol == RPARSY) {
					getsym();
					if (symbol == LBRACESY) {
						getsym();
						bool statements_valid = allstatements();
						if (statements_valid) {
							setLabel(_label);
						}
						return statements_valid;
					}
					else {
						bool statements_valid = statement();
						if (statements_valid) {
							setLabel(_label);
						}
						return statements_valid;
					}
				}
				else {
					error(E_IF_CONDITION_NOT_END);
					return false;
				}
			}
			else {
				error(E_IF_NO_CONDITION);
				return false;
			}
		}
		else {
			error(E_EXPECTED_LPAR);
			return  false;
		}

	}
	else if (symbol == WHILESY) {
		Operand _whilestart = genLabel();
		setLabel(_whilestart);
		getsym();
		if (symbol == LPARSY) {
			getsym();
			if (condition()) {
				Operand _whileend = genLabel();
				auto lastq = getlastquat();
				lastq->r = _whileend;
				if (symbol == RPARSY) {
					getsym();
					if (symbol == LBRACESY) {
						getsym();
						bool statements_valid = allstatements();
						if (statements_valid) {
							emit(jmp, _whilestart, genNULL(), genNULL());
							setLabel(_whileend);
						}
						return statements_valid;
					}
					else {
						bool statements_valid = statement();
						if (statements_valid) {
							emit(jmp, _whilestart, genNULL(), genNULL());
							setLabel(_whileend);
						}
						return statements_valid;
					}
				}
				else {
					error(E_WHILE_CONDITION_NOT_END);
					return false;
				}
			}
			else {
				error(E_WHILE_NO_CONDITION);
				return false;
			}
		}
		else {
			error(E_EXPECTED_LPAR);
			return false;
		}

	}
	else if (symbol == SWITCHSY) {
		getsym();
		if (symbol == LPARSY) {
			getsym();
			if (expression()) {
				TYPE switch_type = INT;
				if (force_int);
				else if (curop.type == CHAR || curop.type == CONSTCHAR) switch_type = CHAR;
				if (symbol == RPARSY) {
					getsym();
					if (symbol == LBRACESY) {
						getsym();
						if (caselist(switch_type)) {
							printf("This is a switch case\n");
							return true;
						}
						else {
							error(E_INVALID_CASE_LIST);
							return false;
						}
					}
					else {
						error(E_SWITCH_NO_LBRACE);
						return false;
					}
				}
				else {
					error(E_SWITCH_EXPRESSION_NOT_END);
					return false;
				}
			}
			else {
				error(E_SWITCH_NO_EXPRESSION);
				return false;
			}
		}
		else {
			error(E_SWITCH_NO_LPAR);
			return false;
		}

	}
	else if (symbol == SCANFSY) {
		getsym();
		if (symbol == LPARSY) {
		scanf_iden:
			getsym();
			if (symbol == IDEN) {
				TableEntry* e = getEntry(token);
				if (e == NULL) {
					critical(E_UNDEFINED_IDEN);
				}
				if (!(e->type == INT || e->type == CHAR)) {
					semantic_error(E_EXPECTED_VAR);
				}
				getsym();
				if (symbol == COMMASY) {
					Operand _var = genEntry(e);
					if (e->type == INT) {
						emit(readi, _var, genNULL(), genNULL());
					}
					else if (e->type == CHAR) {
						emit(readc, _var, genNULL(), genNULL());
					}
					goto scanf_iden;
				}
				else if (symbol == RPARSY) {
					Operand _var = genEntry(e);
					if (e->type == INT) {
						emit(readi, _var, genNULL(), genNULL());
					}
					else if (e->type == CHAR) {
						emit(readc, _var, genNULL(), genNULL());
					}
					getsym();
					if (symbol == SEMISY) {
						printf("This is a scanf statement\n");
						getsym();
						return true;
					}
					else {
						error(E_EXPECTED_SEMI); printf("line%d\n", __LINE__); printf("line%d\n", __LINE__);
						return false;
					}
				}
				else {
					error(E_UNEXPECPTED_SCANF_PARA);
					return false;
				}
			}
			else {
				error(E_EXPECTED_IDEN);
				return false;
			}
		}
		else {
			error(E_EXPECTED_LPAR);
			return false;
		}

	}
	else if (symbol == PRINTFSY) {
		getsym();
		if (symbol == LPARSY) {
			getsym();
			if (expression()) {
				Operand _op = curop;
				if (symbol == RPARSY) {
					getsym();
					if (symbol == SEMISY) {
						emit(print, _op, genNULL(), genNULL());
						printf("This is a printf(expression)\n");
						getsym();
						return true;
					}
					else {
						error(E_EXPECTED_SEMI); printf("line%d\n", __LINE__);
						return false;
					}
				}
				else {
					error(E_PRINTF_NOT_END);
					return false;
				}
			}
			else if (symbol == STRING) {
				string str = token;
				getsym();
				if (symbol == RPARSY) {
					getsym();
					if (symbol == SEMISY) {
						STRINGID _id = pushstring(str);
						Operand _string = genTemp();
						_string.value.temp = _id;
						emit(printstring, _string, genNULL(), genNULL());
						printf("This is a printf(STRING)\n");
						getsym();
						return true;
					}
					else {
						error(E_EXPECTED_SEMI); printf("line%d\n", __LINE__);
						return false;
					}
				}
				else if (symbol == COMMASY) {
					getsym();
					if (expression()) {
						Operand _exp = curop;
						if (symbol == RPARSY) {
							getsym();
							if (symbol == SEMISY) {
								STRINGID _id = pushstring(str);
								Operand _string = genTemp();
								_string.value.temp = _id;
								//emit(printformat, _string, _exp, genNULL());
								emit(printstring, _string, genNULL(), genNULL());
								emit(print, _exp, genNULL(), genNULL());
								printf("This is a printf(string,expression)\n");
								getsym();
								return true;
							}
							else {
								error(E_EXPECTED_SEMI); printf("line%d\n", __LINE__);
								return false;
							}
						}
						else {
							error(E_PRINTF_NOT_END);
							return false;
						}
					}
					else {
						error(E_EXPECTED_EXPRESSION);
						return false;
					}
				}
				else {
					error(E_PRINTF_NOT_END);
					return false;
				}
			}
			else {
				error(E_UNEXPECTED_PRINTF_PARA);
				return false;
			}
		}
		else {
			error(E_EXPECTED_LPAR);
			return false;
		}
	}
	else if (symbol == SEMISY) {
		getsym();
		printf("This is a void statement\n");
		return true;
	}
	else if (symbol == IDEN) {
		string _id = token;
		TableEntry *e = getEntry(_id);
		getsym();
		if (symbol == ASSIGNSY) {
			if (!(e->type == INT || e->type == CHAR || e-> type == PARAINT || e->type == PARACHAR)) {
				semantic_error(E_EXPECTED_VAR);
			}
			getsym();
			if (expression()) {
				if (symbol == SEMISY) {
					Operand _var = genEntry(e);
					Operand _result = curop;
					type_check(_var, _result);
					auto lastq = getlastquat();
					if (lastq) {
						if (lastq->r.optype == OPTEMP) {
							lastq->r = _var;
						}
						else {
							emit(Operator::move, _result, genNULL(), _var);
						}
					}
					else {
						emit(Operator::move, _result, genNULL(), _var);
					}
					printf("This is a iden=expression statement\n");
					getsym();
					return true;
				}
				else {
					error(E_EXPECTED_SEMI); printf("line%d\n", __LINE__);
					return false;
				}
			}
			else {
				error(E_EXPECTED_EXPRESSION);
				return false;
			}
		}
		else if (symbol == LBRACKSY) {
			if (!(e->type == INTARRAY || e->type == CHARARRAY)) {
				semantic_error(E_IDEN_NOT_ARRAY);
				cout << e->iden << " " << e->type << endl;
			}
			Operand _array = genEntry(e);
			getsym();
			if (expression()) {
				Operand _index = curop;
				if (symbol == RBRACKSY) {
					getsym();
					if (symbol == ASSIGNSY) {
						getsym();
						if (expression()) {
							Operand _res = curop;
							if (symbol == SEMISY){
								assert(e->type == INTARRAY || e->type == CHARARRAY);
								if (e->type == INTARRAY) {
									if (_res.type != TYPE::INT && _res.type != TYPE::CONSTINT && _res.type != INTFUNC && _res.type != PARAINT)
										semantic_error(E_TYPE_MISMATCH);
									emit(assigni, _index, _res, _array);
								}
								else if (e->type == CHARARRAY) {
									if (_res.type != TYPE::CHAR && _res.type != TYPE::CONSTCHAR && _res.type != CHARFUNC && _res.type != PARACHAR)
										semantic_error(E_TYPE_MISMATCH);
									emit(assignc, _index, _res, _array);
								}
								printf("This is a iden[exp]=exp statement\n");
								getsym();
								return true;
							}
							else {
								error(E_EXPECTED_SEMI); printf("line%d\n", __LINE__);
								return false;
							}
						}
						else {
							error(E_EXPECTED_EXPRESSION);
							return false;
						}
					}
					else {
						error(E_EXPECTED_ASSIGNSY);
						return false;
					}
				}
				else {
					error(E_EXPECTED_RBRACKSY);
					return false;
				}
			}
			else {
				error(E_EXPECTED_EXPRESSION);
				return false;
			}
		}
		else if (symbol == LPARSY) {
			if (!(e->type == INTFUNC || e->type == CHARFUNC || e->type == VOID)) {
				semantic_error(E_IDEN_NOT_FUNC);
			}
			getsym();
			if (value_parameters(e)) {
				if (symbol == SEMISY) {
					Operand _func = genEntry(e);
					emit(call, _func, genNULL(), genNULL());
					printf("This is a function call statement\n");
					getsym();
					return true;
				}
				else {
					error(E_EXPECTED_SEMI); printf("line%d\n", __LINE__);
					return false;
				}
			}
			else {
				return false;
			}
		}
		else {
			error(E_UNEXPECTED_AFTER_IDEN);
			return false;
		}
	}
	else if (symbol == RETURNSY) {
		getsym();
		if (symbol == LPARSY) {
			getsym();
			if (expression()) {
				Operand _val = curop;
				if (symbol == RPARSY) {
					getsym();
					if (symbol == SEMISY) {
						emit(ret, _val, genNULL(), genNULL());
						printf("This is a return statement\n");
						getsym();
						return true;
					}
					else {
						error(E_EXPECTED_SEMI); printf("line%d\n", __LINE__);
						return false;
					}
				}
				else {
					error(E_RETURN_NOT_END);
					return false;
				}
			}
			else {

				error(E_EXPECTED_EXPRESSION);
				return false;
			}
		}
		else if (symbol == SEMISY) {
			emit(nret, genNULL(), genNULL(), genNULL());
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
}
bool allstatements() {
	while (true) {
		if (symbol == LBRACESY) {
			getsym();
			allstatements();
		}
		else if (symbol == RBRACESY) {
			printf("This is a {statements}\n");
			getsym();
			return true;
		}
		else if (symbol == EOFSY) {
			error(E_UNEXPECTED_FILE_ENDED);
			return false;
		}
		else {
			if (!statement()) {
				if (symbol == RBRACESY) {
					printf("This is a {statements}\n");
					getsym();
					return true;
				}
				error(E_INVALID_STATEMENT);
				return false;
			}
		}
	}
	return false;
}

void funcbody() {
	if (symbol == LBRACESY) {
		getsym();
		statementblock();
		printf("This is a func body\n");
	}
	else {
		error(E_EXPECTED_LBRACE);
	}
	emit(label, genNULL(), genNULL(), genNULL());
}
void type_check(Operand dest, Operand source) {
	if (force_int) {
		if (dest.type != INT && dest.type != PARAINT) {
			semantic_error(E_TYPE_MISMATCH);
		}
	}
	else if (source.type == CONSTCHAR || source.type == CHAR) {
		if (dest.type != CHAR && dest.type != PARACHAR) {
			semantic_error(E_TYPE_MISMATCH);
		}
	}
	else {
		if (dest.type != INT && dest.type != PARAINT) {
			semantic_error(E_TYPE_MISMATCH);
		}
	}
}
void nfunc() {
	vector<TableEntry*>* functable = new vector<TableEntry*>();
	cur_symtable = functable;
	cur_funcentry->partial_symtable = functable;
	if (symbol == LPARSY) {
		if (parameterlist()) {
			funcbody();
			printf("This is a void func\n");
		}
		else {
			error(E_FUNC_PARAMETER_LIST);
		}
	}
	else {
		error(E_EXPECTED_LPAR);
	}
}

void rfunc() {
	vector<TableEntry*>* functable = new vector<TableEntry*>();
	cur_symtable = functable;
	cur_funcentry->partial_symtable = functable;
	if (parameterlist()) {
		funcbody();
		printf("This is a int or char func\n");
	}
	else {
		error(E_FUNC_PARAMETER_LIST);
	}
}

void printq() {
	for (unsigned int i = 0; i < glbl_symtable.size(); i++) {
		if (glbl_symtable[i]->type == VOID || glbl_symtable[i]->type == INTFUNC || glbl_symtable[i]->type == CHARFUNC) {
			cout << glbl_symtable[i]->iden << endl;
			for (unsigned int j = 0; j < glbl_symtable[i]->qlist->size(); j++) {
				cout << operators[(*(glbl_symtable[i]->qlist))[j]->op] <<  (*(glbl_symtable[i]->qlist))[j]->a.optype << (*(glbl_symtable[i]->qlist))[j]->b.optype << (*(glbl_symtable[i]->qlist))[j]->r.optype << endl;
			}
		}
	}
}