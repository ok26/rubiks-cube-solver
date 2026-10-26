#ifndef CUBE_SOLVER_H
#define CUBE_SOLVER_H

#include "cube/cubeIndexModel.h"
#include "solver/patternDatabase.h"

#include <vector>

struct Node {
	CubeIndexModel cube;
	int move;
	int depth;
};

struct PrioritizedMove {
	CubeIndexModel cube;
	int move;
	int estMoves;
	bool operator>(const PrioritizedMove rhs) const {
		return this->estMoves > rhs.estMoves;
	}
};


class CubeSolver {
public:
	std::vector<int> solveBFS(PatternDatabase* pDatabase, CubeIndexModel cube, int availableMoves);
	std::vector<int> solveIDA(PatternDatabase* pDatabase, CubeIndexModel cube, int availableMoves, int foundSolutionDepth = 1e9);
};

#endif // !CUBE_SOLVER_H
