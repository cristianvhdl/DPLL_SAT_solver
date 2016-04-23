#include "stdafx.h"
#include "CNF_function_representation.h"

using namespace std;

/*
Variable
*/
CNF_variable::CNF_variable(t_blif_signal *s, string n) {
	name = n;
	index = s->data.index;
	type = s->type;
}

CNF_variable::~CNF_variable() {
}

/*
clause
*/
clause::clause(t_blif_cube* c, int input_count) {
	for (int i = 0; i < input_count; ++i) {
		literals.push_back(int_2_literal(read_cube_literal(c, i)));
	}
	is_DC = c->is_DC;
}

clause::~clause() {
}

literal_type clause::int_2_literal(int i) {
	switch (i) {
		case 1: return literal_type::LITERAL0;
		case 2: return literal_type::LITERAL1;
		case 3: return literal_type::LITERALX;
	default: return literal_type::LITERAL1;
	}
}

void clause::print() {
	for (auto it = literals.begin(); it < literals.end(); it++) {
		if (*it == literal_type::LITERAL0) {
			cout << "0  ";
		} else if(*it == literal_type::LITERAL1) {
			cout << "1  ";
		} else if(*it == literal_type::LITERALX) {
			cout << "-  ";
		} else {}
	}

	cout << (is_DC ? " -" : " 1") << endl;
}

/*
CNF Function
*/
CNF_function::CNF_function() {
}

CNF_function::CNF_function(char * filename) {
	t_blif_logic_circuit *blif_circuit = NULL;
	blif_circuit = ReadBLIFCircuit(filename);
	if (blif_circuit != NULL) {
		s_blif_cubical_function* function = blif_circuit->list_of_functions[0];
		for (int i1 = 0; i1 < function->input_count; ++i1) {
			inputs.push_back(new CNF_variable((function->inputs)[i1], string(blif_circuit->primary_inputs[i1]->data.name)));
		}
		output = new CNF_variable(function->output, string(blif_circuit->primary_outputs[0]->data.name));
		for (int i2 = 0; i2 < function->cube_count; ++i2) {
			set_of_cubes.push_back(new clause(function->set_of_cubes[i2], function->input_count));
		}
		value = function->value;
	}
	DeleteBLIFCircuit(blif_circuit);
}

CNF_function::~CNF_function() {
}

void CNF_function::print() {
	for (auto it = inputs.begin(); it < inputs.end(); it++) {
		cout << (*it)->name << " ";
	}
	cout << " " << output->name << endl;
	for (auto it = set_of_cubes.begin(); it < set_of_cubes.end(); it++) {
		(*it)->print();
	}
}