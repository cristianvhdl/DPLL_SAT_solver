#pragma once

#include<iostream>
#include<string>
#include<vector>

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
public:
	clause() {}
	clause(t_blif_cube* c, int input_count);
	~clause();
	literal_type int_2_literal(int i);
	//int read_variable();
	//void write_variable();
	void print();
};

//enum signal_type { SIGNAL_LOCAL = 0, SIGNAL_EXTERNAL_IN, SIGNAL_EXTERNAL_OUT, SIGNAL_LOCAL_OTHER };

class CNF_variable {
	friend class clause;
	friend class CNF_function;
	friend class logic_circuit;
private:
	string name;
	int index;
	t_signal_type type;
public:
	CNF_variable() {}
	CNF_variable(t_blif_signal *s, string n);
	~CNF_variable();
};

class CNF_function {
	friend class logic_circuit;
private:
	vector<CNF_variable*> inputs;	/* List of input signals */

	CNF_variable* output;	/* Logic function output */

	vector<clause*> set_of_cubes; /* Set of cubes */

	int value;	/* If there are no cubes then this stores the constant value the function evaluates to. Otherwise, should be < 0 */
public:
	CNF_function();
	CNF_function(char * filename);
	~CNF_function();
	void print();
};