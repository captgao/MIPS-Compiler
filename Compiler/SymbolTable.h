#pragma once
#include "pch.h"
#include <string>
#include <iostream>
#include <vector>
#include <map>
#include "ParaList.h"
#include "Quaterntion.h"
using namespace std;

class TableEntry {

public:
	struct StackInfo {
		unsigned int size;
		unsigned int var;
		unsigned int temp;
		unsigned int array;
		unsigned int ra;
		unsigned int regs;
		unsigned int arguments;
	};
	enum TYPE type;
	string iden;
	int value;
	unsigned int length;
	bool isglobal = false;
	vector<Quaterntion*>* qlist;
	vector<ParaEntry> paralist;
	unsigned int tmpindex = 0;
	unsigned int varoffset;
	StackInfo stackinfo;
	vector<TableEntry*>* partial_symtable;

	TableEntry(enum TYPE type, string iden, int value, unsigned int length) {
		this->type = type;
		this->iden = iden;
		this->value = value;
		this->length = length;
	};
	bool pushpara(enum TYPE type, string iden) {
		ParaEntry p(type, iden);
		for (unsigned int i = 0; i < paralist.size(); i++) {
			if (p.iden == paralist[i].iden) {
				cout << "duplicate para" << endl;
				return false;
			}
		}
		//cout << type << iden << endl;
		paralist.push_back(p);
		return true;
	};
	void setqlist(vector<Quaterntion*>* qlist) {
		this->qlist = qlist;
	};
};
//TableEntry::TableEntry( enum TYPE type, string iden, int value, unsigned int length) 

enum TYPE {
	CONSTCHAR, CONSTINT, CONSTSTRING, CHAR, INT, VOID, INTFUNC, CHARFUNC, CHARARRAY, INTARRAY, PARACHAR, PARAINT, ERRORTYPE
};

TableEntry *getEntry(string id);
