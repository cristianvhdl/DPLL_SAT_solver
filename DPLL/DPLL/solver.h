#pragma once

#include "CNF_function_representation.h"

class solver {
public:
	solver();
	~solver();
	void solver::DPLL_init();
	void solver::DPLL_recursively(CNF_function *f);
};

