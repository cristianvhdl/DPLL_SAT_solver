#include "stdafx.h"
#include "CNF_function_representation.h"
#include "solver.h"

using namespace std;

int main(int argc, char* argv[])
{	
	CNF_function* CNF = NULL;
	bool satisfiable = false;
	
	if (argc != 2) {
		cout << "Usage: " << argv[0] << " <source BLIF file>" << endl;
		return 0;
	}
	cout << "DPLL SAT SOLVER START!" << endl;

	// READ function FROM BLIF FILE
	cout << "READING FILE " << argv[1] << endl;
	CNF = new CNF_function(argv[1]);

	if (CNF) {
		CNF->print();

		// Solve the CNF function
		solver::DPLL_init(*CNF);
		satisfiable = solver::DPLL_recursively(*CNF);

		// Print Result
		CNF->print_result(satisfiable);

		// Release Memory
		CNF->clear();
	}

	return 0;
}

