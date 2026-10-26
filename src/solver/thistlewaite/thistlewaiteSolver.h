#ifndef THISTLEWAITE_SOLVER_H
#define THISTLEWAITE_SOLVER_H

#include "cube/cubeIndexModel.h"
#include "solver/patternDatabase.h"

#include <vector>

class ThistlewaiteSolver {

	int availableMoves[4] = { 262143, 151551, 151524, 149796 };
	PatternDatabase* pDatabaseG1;
	PatternDatabase* pDatabaseG2;
	PatternDatabase* pDatabaseG3;
	PatternDatabase* pDatabaseG4;

public:
	bool tablesInitialized = false;

	ThistlewaiteSolver(bool initializeTablesOnCreate = true);

	void initializeTables();
	std::vector<int> solveCubeBFS(CubeIndexModel cube);
	std::vector<int> solveCubeIDA(CubeIndexModel cube);
};

#endif
