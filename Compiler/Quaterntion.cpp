#include "pch.h"
#include "Quaterntion.h"
#include "Operand.h"
using namespace std;
extern vector<Quaterntion*>* cur_qlist;
vector<Operand> labels;
Quaterntion* getlastquat() {
	if (cur_qlist->empty()) {
		return NULL;
	}
	else{
		return (*cur_qlist)[cur_qlist->size() - 1];
	}
}
Quaterntion* emit(Operator op, Operand a, Operand b, Operand r) {
	Quaterntion* q = new Quaterntion;
	q->op = op;
	q->a = a;
	q->b = b;
	q->r = r;
	q->labels = labels;
	addQuaterntion(q);
	labels.clear();
	return q;
}
void addQuaterntion(Quaterntion* q) {
	cur_qlist->push_back(q);
}
void setLabel(Operand op) {
	labels.push_back(op);
}
