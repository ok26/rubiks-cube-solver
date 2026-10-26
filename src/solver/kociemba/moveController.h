#ifndef G1_MOVE_CONTROLLER
#define G1_MOVE_CONTROLLER

#include "solver/kociemba/patternIndexer.h"

#include <array>

class MoveController {

	std::array<std::array<int, 18>, 2187>		cornerOriMoveTable;
	std::array<std::array<int, 18>, 1013760>	edgeUDMoveTable;
	std::array<std::array<int, 18>, 40320>		cornerPermMoveTable;
	std::array<std::array<int, 18>, 40320>		edgeRPermMoveTable;
	std::array<std::array<int, 18>, 24>			edgeMPermMoveTable;
	std::array<std::array<int, 18>, 11880>		UDSliceSortMoveTable;
	std::array<std::array<int, 18>, 11880>		USliceSortMoveTable;
	std::array<std::array<int, 18>, 11880>		DSliceSortMoveTable;

	int availableMovesG2 = 151524;

	ReverseCubeIndexer reverseCubeIndexer;
	CubeIndexer cubeIndexer;

public:

	void initializeTables();

	void generateCornerOriTable();
	void generateEdgeUDTable();
	void generateCornerPermTable();
	void generateEdgeRPermTable();
	void generateEdgeMPermTable();
	void generateUDSliceSortTable();
	void generateUSliceSortTable();
	void generateDSliceSortTable();

	int updateFlipUDSlice(int flipUDSlice, int move);
	int updateCornerOrientaion(int cornerOrientaion, int move);
	int updateUDSliceSorted(int UDSliceSort, int move);
	int updateUSliceSorted(int USliceSort, int move);
	int updateDSliceSorted(int DSliceSort, int move);
	int updateCornerPerm(int cornerPerm, int move);
	int updateEdgeRPerm(int edgeRPerm, int move);
	int updateEdgeMPerm(int edgeMPerm, int move);
};

#endif
