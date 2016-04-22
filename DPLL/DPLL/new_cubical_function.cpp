#include "stdafx.h"
#include "new_cubical_function.h"

using namespace std;

/*
Cubical Signal
*/
cubical_signal::cubical_signal(t_blif_signal *s) {
	//name = string(s->data.name);
	index = s->data.index;
	type = s->type;
}

cubical_signal::~cubical_signal() {
}

/*
Cube
*/
cube::cube(t_blif_cube* c, int input_count) {
	for (int i = 0; i < input_count; ++i) {
		literals.push_back(int_2_literal(read_cube_literal(c, i)));
	}
	is_DC = c->is_DC;
}

cube::~cube() {
}

literal_type cube::int_2_literal(int i) {
	switch (i) {
		case 1: return literal_type::LITERAL0;
		case 2: return literal_type::LITERAL1;
		case 3: return literal_type::LITERALX;
	default: return literal_type::LITERAL0;
	}
}

void cube::print() {
	for (auto it = literals.begin(); it < literals.end(); it++) {
		if (*it == literal_type::LITERAL0) {
			cout << "0 ";
		} else if(*it == literal_type::LITERAL1) {
			cout << "1 ";
		} else if(*it == literal_type::LITERALX) {
			cout << "X ";
		} else {}
	}

	cout << (is_DC ? " -" : " 1") << endl;
}

/*
Cubical Function
*/
cubical_function::cubical_function(s_blif_cubical_function* function) {
	for (int i1 = 0; i1 < function->input_count; ++i1) {
		inputs.push_back(new cubical_signal((function->inputs)[i1]));
	}
	output = new cubical_signal(function->output);
	for (int i2 = 0; i2 < function->cube_count; ++i2) {
		set_of_cubes.push_back(new cube(function->set_of_cubes[i2], function->input_count));
	}
	value = function->value;
}

cubical_function::~cubical_function() {
}

void cubical_function::print() {
	for (auto it = set_of_cubes.begin(); it < set_of_cubes.end(); it++) {
		(*it)->print();
	}
}

/*
BLIF Circuit
*/
logic_circuit::logic_circuit(char * filename) {
	read_file(filename);
}

logic_circuit::~logic_circuit() {
}

void logic_circuit::read_file(char* filename) {
	t_blif_logic_circuit *blif_circuit = NULL;
	blif_circuit = ReadBLIFCircuit(filename);
	if (blif_circuit != NULL) {
		for (int i = 0; i < blif_circuit->function_count; ++i) {
			list_of_functions.push_back(new cubical_function(blif_circuit->list_of_functions[i]));
		}
	}
	DeleteBLIFCircuit(blif_circuit);
}