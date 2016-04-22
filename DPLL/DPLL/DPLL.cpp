// DPLL.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "new_cubical_function.h"

using namespace std;

int main(int argc, char* argv[])
{	
	logic_circuit* circuit = NULL;
	
	if (argc != 2) {
		cout << "Usage: " << argv[0] << " <source BLIF file>" << endl;
		return 0;
	}
	cout << "DPLL SAT SOLVER START!" << endl;

	/*	READ CIRCUIT FROM BLIF FILE	*/
	cout << "READING FILE " << argv[1] << endl;
	circuit = new logic_circuit(argv[1]);

	if (circuit) {
		//cout << "NOT NULL" << endl;
		circuit->list_of_functions[0]->print();
	}

	return 0;
}

