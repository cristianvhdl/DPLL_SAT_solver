#include "stdafx.h"
#include "CNF_function_representation.h"

using namespace std;

/*
------------ CNF Variable ------------ 
*/
CNF_variable::CNF_variable(t_blif_signal *s, string n) {
	name = n;
	index = s->data.index;
	type = s->type;
}

/*
------------ CNF clause ------------
*/
clause::clause(t_blif_cube* c, int input_count) {
	for (int i = 0; i < input_count; ++i) {
		literals.push_back(int_2_literal(read_cube_literal(c, i)));
	}
	is_DC = c->is_DC;
}

// This function converts integer to the corresponding literal_type
literal_type clause::int_2_literal(int i) {
	switch (i) {
		case 1: return literal_type::LITERAL0;
		case 2: return literal_type::LITERAL1;
		case 3: return literal_type::LITERALX;
	default: return literal_type::LITERAL1;
	}
}

// This function prints the clause
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
------------ CNF Function ------------ 
*/

//This is constructor that reads the blif file
CNF_function::CNF_function(char * filename) {	
	t_blif_logic_circuit *blif_circuit = NULL;
	blif_circuit = ReadBLIFCircuit(filename);	// Read file here and itialize the t_blif_logic_circuit

	//Transfer the information from t_blif_logic_cirtuit to the CNF_function object
	if (blif_circuit != NULL) {
		s_blif_cubical_function* function = blif_circuit->list_of_functions[0];
		
		for (int i1 = 0; i1 < function->input_count; ++i1) {		// All input variables
			inputs.push_back(new CNF_variable((function->inputs)[i1], string(blif_circuit->primary_inputs[i1]->data.name)));
		}
		output = new CNF_variable(function->output, string(blif_circuit->primary_outputs[0]->data.name));	//output variable
		
		for (int i2 = 0; i2 < function->cube_count; ++i2) {	// cubes to clauses
			t_blif_cube* cube= function->set_of_cubes[i2];
			set_of_clauses.push_back(new clause(cube, function->input_count));
			for (int j = 0; j < function->input_count; ++j) {
				int lit = read_cube_literal(cube, j);
				if (lit == 1 || lit == 2) {	//the variable is in the clause
					inputs[j]->occurance++;
				}
			}
		}
		value = function->value;		
	}
	DeleteBLIFCircuit(blif_circuit);		// Release Memory
}

// copy constructor (Shallow copy)
CNF_function::CNF_function(const CNF_function &f) {	
	for (auto it = f.inputs.begin(); it < f.inputs.end(); it++) {
		inputs.push_back(*it);
	}
	output = f.output;
	for (auto it = f.set_of_clauses.begin(); it != f.set_of_clauses.end(); it++) {
		set_of_clauses.push_back(*it);
	}
	value = value;
}

CNF_function::~CNF_function() {	
	//because the copy constructor above uses shallow copy, the memeory doesn't need to be deallocated here, please call clear() to release memory before exit the program
}

// This function deduces one variable on all clauses in the CNF function
vector<CNF_variable*>::iterator CNF_function::deduce(int var_ind, bool valuation) {
	int literal_ind = inputs[var_ind]->index;
	inputs[var_ind]->curr_valuation = valuation;
	cout << " --- Deduce Variable: " << inputs[var_ind]->name << " (" << valuation << ")"<< endl;
	for (auto it = set_of_clauses.begin(); it != set_of_clauses.end();) {
		if (((*it)->literals[literal_ind] == literal_type::LITERAL0 && !valuation) ||
			((*it)->literals[literal_ind] == literal_type::LITERAL1 && valuation)){
			it = set_of_clauses.erase(it);	// remove the clause
		} else {
			++it;
		}
	}
	auto it = inputs.erase(inputs.begin() + var_ind);
	print();
	return it;
}

//This function finds and deduces all pure literals found in the CNF function
void CNF_function::find_and_deduce_pure_literals() {
	int var_ind = 0;
	for (auto it = inputs.begin(); it != inputs.end();) {	//traverse all input variables
		int literal_ind = (*it)->index;
		literal_type first = literal_type::LITERALX;
		bool find_pure_literal = true;
		bool is_first = true;
		for (auto it1 = set_of_clauses.begin(); it1 != set_of_clauses.end();++it1) {	//traverse all clauses
			literal_type curr_literal = (*it1)->literals[literal_ind];
			if (curr_literal != literal_type::LITERALX) {
				if (is_first) {
					first = curr_literal;
					is_first = false;
				} else {
					if (curr_literal != first) {	// if find different literal value for current variable in all clauses
						find_pure_literal = false;	// not a pure literal
						break;
					}
				}
			}
		}
		if (!clauses_is_empty() && find_pure_literal) {	//if found pure literal
			cout << "Pure Literal Found: " << (*it)->name;
			bool valuation = (first == literal_type::LITERAL1 ? true:false);
			it = deduce(var_ind, valuation);	// deduce it
			it = inputs.begin();
			var_ind = 0;
		} else {
			++var_ind;
			++it;
		}
	}
}

// This function finds and deduces all unit clauses in the CNF function
void CNF_function::find_and_deduce_unit_clauses() {
	for (auto it = set_of_clauses.begin(); it != set_of_clauses.end();) {	// traverse all clauses
		int num_inputs_var = inputs.size();
		int num_DC = 0;
		int var_ind = 0;
		int saved_var_ind = 0;
		bool valuation = false;
		for (auto it1 = inputs.begin(); it1 != inputs.end(); ++it1, ++var_ind) {	// traverse all input variables
			int literal_ind = (*it1)->index;
			literal_type curr = (*it)->literals[literal_ind];
			if (curr == literal_type::LITERALX) {
				num_DC++;
			} else {
				saved_var_ind = var_ind;
				valuation = (curr == literal_type::LITERAL1? true:false);
			}
		}
		if (num_inputs_var - num_DC == 1) {	// unit_clause found
			cout << "Unit Clause Found with Variable: " << inputs[saved_var_ind]->name;
			deduce(saved_var_ind, valuation);
			it = set_of_clauses.begin();
		} else {
			++it;
		}
	}
}

// This function sorts the input variables based on their occurance
void CNF_function::sort_occurance() {
	struct larger_than_occurance {
		inline bool operator() (const CNF_variable* variable1, const CNF_variable* variable2) {
			return (variable1->occurance > variable2->occurance);
		}
	};
	sort(inputs.begin(), inputs.end(), larger_than_occurance());
}

// This function prints the CNF function
void CNF_function::print() {
	for (auto it = inputs.begin(); it < inputs.end(); it++) {
		string name = (*it)->name;
		name.append(1, ' ');
		cout << name;
	}
	//cout << " " << output->name << endl;
	if (clauses_is_empty() || inputs_is_empty()) {
		cout << endl;
	} else {
		cout << endl;
		for (auto it = set_of_clauses.begin(); it != set_of_clauses.end(); it++) {
			//(*it)->print();
			for (auto it1 = inputs.begin(); it1 < inputs.end(); it1++) {
				int literal_ind = (*it1)->index;
				literal_type curr = (*it)->literals[literal_ind];
				string output = "";
				if (curr == literal_type::LITERAL0) {
					output = "0";
				} else if (curr == literal_type::LITERAL1) {
					output = "1";
				} else if (curr == literal_type::LITERALX) {
					output = "-";
				} else {}
				output.append((*it1)->name.length(), ' ');
				cout << output;
			}
			cout << endl;

			//cout << (is_DC ? " -" : " 1") << endl;
		}
	}
}

// This function prints the satisfiablilty of the function
void CNF_function::print_result(bool satisfiable) {
	if (satisfiable) {
		cout << endl << "The CNF function is Satisfiable with Solution: " << endl;
		for (auto it = inputs.begin(); it < inputs.end(); it++) {
			cout << (*it)->name << "(" << (*it)->curr_valuation << ") ";
		}
		cout << endl;
	} else {
		cout << endl << "The CNF function is Un-Satisfiable" << endl;
	}
}

// This function release the memory for the CNF function
void CNF_function::clear() {
	for (auto it = inputs.begin(); it < inputs.end(); it++) {
		delete(*it);
	}
	inputs.clear();
	delete(output);
	for (auto it = set_of_clauses.begin(); it != set_of_clauses.end(); it++) {
		delete(*it);
	}
	set_of_clauses.clear();
}