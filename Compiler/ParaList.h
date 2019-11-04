#pragma once
#include <string>
using namespace std;
class ParaEntry {
public:
	enum TYPE type;
	string iden;
	unsigned int offset;
	ParaEntry(enum TYPE type, string iden) {
		this->type = type;
		this->iden = iden;
	};
};