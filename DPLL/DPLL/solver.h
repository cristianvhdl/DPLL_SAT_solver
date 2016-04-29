#pragma once

#include "CNF_function_representation.h"

class solver {
public:
	solver();
	~solver();
	static void solver::DPLL_init(CNF_function &f);
	static bool solver::DPLL_recursively(CNF_function &f);
};

