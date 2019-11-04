#include "pch.h"
#include "SymbolTable.h"
#include "ParaList.h"
#include <string>
#include <vector>
using namespace std;
extern vector<TableEntry*> glbl_symtable;
extern vector<TableEntry*> *cur_symtable;

TableEntry* getEntry(string id){
	for (unsigned int i = 0; i < cur_symtable->size(); i++) {
		if ((*cur_symtable)[i]->iden == id) {
			return (*cur_symtable)[i];
		}
	}
	for (unsigned int i = 0; i < glbl_symtable.size(); i++) {
		if (glbl_symtable[i]->iden == id) {
			return glbl_symtable[i];
		}
	}
	cout << "entry not found" << endl;
	return NULL;
}
