#pragma once
#include "Operand.h"
#include <vector>
enum Operator {
	index, //
	push, //
	pop, // not used
	call, //
	result, //
	add, //
	sub, //
	multiple, //
	divide,  //
	lessthan, //
	lessequal, //
	greaterthan, //
	greaterequal, //
	equalto, //
	notequal, //
	label, // not used
	jmp, //
	nop, // not used
	print, //
	printstring, //
	printformat, // not used
	readi, //
	readc, //
	ret, //
	nret, //
	bne, //
	move, //
	assigni, //
	assignc  //
};
typedef struct {
	Operator op;
	Operand a;
	Operand b;
	Operand r;
	vector<Operand> labels;
}Quaterntion;

Quaterntion * getlastquat();

Quaterntion* emit(Operator op, Operand a, Operand b, Operand r);

void addQuaterntion(Quaterntion* q);

void setLabel(Operand op);

void printq();

