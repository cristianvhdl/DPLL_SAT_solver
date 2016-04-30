#include "stdafx.h"
#include "solver.h"

void solver::DPLL_init(CNF_function &f) {
	// Do some operations before the recursive algorithm
}

bool solver::DPLL_recursively(CNF_function &prev_f) {

	/*
	the set of clauses is empty
	-> all clauses are satisfiable
	-> solution found
	*/
	if (prev_f.clauses_is_empty()) {
		return true;
	}

	CNF_function curr_f = prev_f;
	curr_f.find_and_resolve_unit_clauses();		// find and resolve unit clauses
	curr_f.find_and_resolve_pure_literals();		// find and reolve pure literals
	if (curr_f.clauses_is_empty()) {				// Check its satisfiablity again after the above operation
		return true;
	}

	if (curr_f.inputs_is_empty()) {	// the set of clauses is not empty AND there are no varable left to be set -> need to back track
		cout << "Back Track" << endl;
		return false;
	}else{	// Branching
		CNF_function f_branch1 = curr_f;
		cout << f_branch1.getVarName(0) << " Branch 1";
		f_branch1.resolve(0, true);	// The first input variable is set to 1
		if (DPLL_recursively(f_branch1)) {	// solution found
			return true;
		} else {	// solution not found, keep looking
			CNF_function f_branch2 = curr_f;
			cout << f_branch2.getVarName(0) << " Branch 0";
			f_branch2.resolve(0, false);	// The variable is set to 0
			return DPLL_recursively(f_branch2);
		}
	}
	
}