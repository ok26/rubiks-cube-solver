#ifndef IMPRV_KOCIMEBA_SOLVER_H
#define IMPRV_KOCIMEBA_SOLVER_H

#include "cube/cubeIndexModel.h"

#include "solver/kociemba/g1DatabaseController.h"
#include "solver/kociemba/moveController.h"
#include "solver/kociemba/g2DatabaseController.h"
#include "solver/kociemba/patternIndexer.h"

#include <vector>

struct CubeStateG1 {
	int flipUDSlice;
	int cornerOri;
	int cornerPerm;
	int UDSliceSort;
	int USliceSort;
	int DSliceSort;
};

struct CubeStateG2 {
	int cornerPerm;
	int edgeRPerm;
	int edgeMPerm;
};

struct NodeG1 {
	CubeStateG1 cube;
	int move;
	int depth;
};

struct NodeG2 {
	CubeStateG2 cube;
	int move;
	int depth;
};

struct PrioritizedMoveG1 {
	CubeStateG1 cube;
	int move;
	int estMoves;
	bool operator>(const PrioritizedMoveG1 rhs) const {
		return this->estMoves > rhs.estMoves;
	}
};

struct PrioritizedMoveG2 {
	CubeStateG2 cube;
	int move;
	int estMoves;
	bool operator>(const PrioritizedMoveG2 rhs) const {
		return this->estMoves > rhs.estMoves;
	}
};


class KociembaSolver {

	CubeIndexer cubeIndexer;
	G1DatabaseController databaseG1Controller;
	G2DatabaseController databaseG2Controller;
	MoveController moveController;

	int availableMovesG2 = 151524;

public:
	bool tablesInitialized = false;

	KociembaSolver(bool initializeTablesOnCreate = true);

	void initializeTables();
	CubeStateG1 getStateFromCube(CubeIndexModel& cube);
	CubeStateG2 getG2StateFromG1State(CubeStateG1& stateG1);
	std::vector<int> tripleSearch(CubeIndexModel cube1, int timeBlock, bool print = true);
	std::vector<int> kociembaTwoPhase(CubeIndexModel cube, int timeBlock, bool print = true, int solutionDepthblock = 1e9, int adds = 0);
	std::vector<int> solveG2WithBlock(CubeStateG2& startingNode, int depthBlock);
};

#endif
