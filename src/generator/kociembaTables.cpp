#include "generator/kociembaTables.h"
#include "generator/pruningTable.h"

#include "cube/cubeIndexModel.h"
#include "solver/kociemba/patternIndexer.h"
#include "solver/kociemba/symmetryIndexer.h"

#include <filesystem>
#include <iostream>
#include <utility>
#include <vector>

using namespace std;

static const int availableMovesG2 = 151524;

void generatePhase1Tables(MoveController& moveController, int maxDepth) {
	CubeIndexer cubeIndexer;
	ReverseCubeIndexer reverseCubeIndexer;
	SymmetryIndexer symmetryIndexer;
	filesystem::create_directories("resources/data");

	PruningTable table("KociembaG1");
	table.rowClasses.generate(1013760, [&](int flipUDSlice, int symmetry) {
		CubeIndexModel cube;
		reverseCubeIndexer.updateCubeEdgeMComb(cube, flipUDSlice / 2048);
		reverseCubeIndexer.updateCubeEdgeOrientaion(cube, flipUDSlice % 2048);
		CubeIndexModel symmetric = symmetryIndexer.overlaySymmetry(cube, symmetry);
		return cubeIndexer.getEdgeOrientationIndex(symmetric) + cubeIndexer.getUDSliceIndex(symmetric) * 2048;
	});
	table.rowClasses.writeFile("resources/data/KociembaG1Symmetry", 4);
	cout << "Wrote resources/data/KociembaG1Symmetry (" << table.rowClasses.representatives.size() << " classes)\n";

	table.columns = 2187;
	table.moveCount = 18;

	table.rowMoveTable.resize(1013760 * 18);
	for (int flipUDSlice = 0; flipUDSlice < 1013760; flipUDSlice++)
		for (int move = 0; move < 18; move++)
			table.rowMoveTable[flipUDSlice * 18 + move] = moveController.updateFlipUDSlice(flipUDSlice, move);

	table.columnMoveTable.resize(2187 * 18);
	for (int cornerOri = 0; cornerOri < 2187; cornerOri++)
		for (int move = 0; move < 18; move++)
			table.columnMoveTable[cornerOri * 18 + move] = moveController.updateCornerOrientaion(cornerOri, move);

	table.columnSymmetryTable.resize(2187 * 16);
	for (int cornerOri = 0; cornerOri < 2187; cornerOri++) {
		CubeIndexModel cube;
		reverseCubeIndexer.updateCubeCornerOrientation(cube, cornerOri);
		for (int symmetry = 0; symmetry < 16; symmetry++) {
			CubeIndexModel symmetric = symmetryIndexer.overlaySymmetry(cube, symmetry);
			table.columnSymmetryTable[cornerOri * 16 + symmetry] = cubeIndexer.getCornerOrientationIndex(symmetric);
		}
	}

	CubeIndexModel solved;
	int goalFlipUDSlice = cubeIndexer.getEdgeOrientationIndex(solved) + cubeIndexer.getUDSliceIndex(solved) * 2048;
	table.generate(goalFlipUDSlice, cubeIndexer.getCornerOrientationIndex(solved), maxDepth);
	table.writeFile();
}

void generatePhase2Tables(MoveController& moveController, int maxDepth) {
	CubeIndexer cubeIndexer;
	ReverseCubeIndexer reverseCubeIndexer;
	SymmetryIndexer symmetryIndexer;
	filesystem::create_directories("resources/data");

	vector<int> moves;
	for (int move = 0; move < 18; move++)
		if ((1 << move) & availableMovesG2)
			moves.push_back(move);
	int moveCount = moves.size();

	vector<int> cornerPermMoves(40320 * moveCount), edgeRPermMoves(40320 * moveCount), edgeMPermMoves(24 * moveCount);
	for (int i = 0; i < moveCount; i++) {
		for (int perm = 0; perm < 40320; perm++) {
			cornerPermMoves[perm * moveCount + i] = moveController.updateCornerPerm(perm, moves[i]);
			edgeRPermMoves[perm * moveCount + i] = moveController.updateEdgeRPerm(perm, moves[i]);
		}
		for (int perm = 0; perm < 24; perm++)
			edgeMPermMoves[perm * moveCount + i] = moveController.updateEdgeMPerm(perm, moves[i]);
	}

	CubeIndexModel solved;
	int goalCornerPerm = cubeIndexer.getCornerPermIndex(solved);
	int goalEdgeRPerm = cubeIndexer.getEdgeRPermIndex(solved);
	int goalEdgeMPerm = cubeIndexer.getEdgeMPermIndex(solved);

	PruningTable corner("KociembaG2Corner");
	corner.rowClasses.generateWithoutSymmetry(40320);
	corner.moveCount = moveCount;
	corner.rowMoveTable = cornerPermMoves;
	corner.columnMoveTable.assign(moveCount, 0);
	corner.generate(goalCornerPerm, 0, maxDepth);
	corner.writeFile();

	PruningTable edge("KociembaG2Edge");
	edge.rowClasses.generateWithoutSymmetry(24);
	edge.columns = 40320;
	edge.moveCount = moveCount;
	edge.rowMoveTable = edgeMPermMoves;
	edge.columnMoveTable = edgeRPermMoves;
	edge.generate(goalEdgeMPerm, goalEdgeRPerm, maxDepth);
	edge.writeFile();

	PruningTable cubie("KociembaG2Cubie");
	cubie.rowClasses.generate(40320, [&](int cornerPerm, int symmetry) {
		CubeIndexModel cube;
		reverseCubeIndexer.updateCubeCornerPerm(cube, cornerPerm);
		CubeIndexModel symmetric = symmetryIndexer.overlaySymmetry(cube, symmetry);
		return cubeIndexer.getCornerPermIndex(symmetric);
	});
	cubie.rowClasses.writeFile("resources/data/KociembaG2Symmetry", 3);
	cout << "Wrote resources/data/KociembaG2Symmetry (" << cubie.rowClasses.representatives.size() << " classes)\n";

	cubie.columns = 40320;
	cubie.moveCount = moveCount;
	cubie.rowMoveTable = std::move(cornerPermMoves);
	cubie.columnMoveTable = std::move(edgeRPermMoves);

	cubie.columnSymmetryTable.resize(40320 * 16);
	for (int edgeRPerm = 0; edgeRPerm < 40320; edgeRPerm++) {
		CubeIndexModel cube;
		reverseCubeIndexer.updateCubeEdgeRPerm(cube, edgeRPerm);
		for (int symmetry = 0; symmetry < 16; symmetry++) {
			CubeIndexModel symmetric = symmetryIndexer.overlaySymmetry(cube, symmetry);
			cubie.columnSymmetryTable[edgeRPerm * 16 + symmetry] = cubeIndexer.getEdgeRPermIndex(symmetric);
		}
	}

	cubie.generate(goalCornerPerm, goalEdgeRPerm, maxDepth);
	cubie.writeFile();
}
