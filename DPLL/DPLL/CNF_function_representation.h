#pragma once

#include<iostream>
#include<string>
#include<vector>
#include<list>
#include <algorithm>

extern "C" {
	#include"blif_common.h"
	#include"cubical_function_representation.h"
}

using namespace std;

enum literal_type { LITERAL0, LITERAL1, LITERALX };

class clause {
	friend class CNF_function;
	friend class logic_circuit;
private:
	vector<literal_type> literals;
	bool is_DC;
	literal_type int_2_literal(int i);
public:
	clause() {}
	clause(t_blif_cube* c, int input_count);
	~clause();
	void print();
};

class CNF_variable {
	friend class clause;
	friend class CNF_function;
	friend class logic_circuit;
private:
	string name;
	int index;
	int occurance = 0;
	bool curr_valuation = true;	// current valuation of the variable, default is true
	t_signal_type type;
public:
	CNF_variable() {}
	CNF_variable(t_blif_signal *s, string n);
	~CNF_variable();
};

class CNF_function {
private:
	vector<CNF_variable*> inputs;	// List of input variables

	CNF_variable* output;	// CNF function output

	list<clause*> set_of_clauses; // Set of clauses

	int value;	// If there are no clause then this stores the constant value the function evaluates to. Otherwise, should be < 0
public:
	CNF_function();	// default constructor
	CNF_function(char * filename);	// constructor that read a file
	CNF_function(const CNF_function &f);	// copy constructor, it copies the pointers that points to the variables and set of clause (shallow copy)
	~CNF_function();	// destructor

	int getValue() { return value; }
	string getVarName(int ind) { return inputs[ind]->name; }

	vector<CNF_variable*>::iterator resolve(int var_ind, bool valuation);
	void find_and_resolve_pure_literals();
	void find_and_resolve_unit_clauses();
	bool inputs_is_empty() { return inputs.empty(); }
	bool clauses_is_empty() { return set_of_clauses.empty(); }
	void sort_occurance();

	void print();
	void print_result(bool satisfiable);
	void clear();
};