#include "generator/tableVerifier.h"

#include "cube/cubeIndexModel.h"
#include "solver/kociemba/g1DatabaseController.h"
#include "solver/kociemba/g2DatabaseController.h"
#include "solver/kociemba/patternIndexer.h"
#include "solver/kociemba/symmetryIndexer.h"

#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <vector>

using namespace std;

static const int availableMovesG2 = 151524;

struct VerifyResult {
	int64_t checked = 0;
	int64_t unknown = 0;
	int64_t errors = 0;
};

// A correct distance table has distance 0 only at the goal, and every other position has a neighbour
// exactly one move closer and no neighbour more than one move away. Positions where the position or one
// of its neighbours has no distance (255) are counted as unknown; a completely generated table has none.
static void checkPosition(int distance, const vector<int>& neighbourDistances, bool goal, VerifyResult& result) {
	if (distance == 255) {
		result.unknown++;
		return;
	}
	int closest = 255;
	bool consistent = true;
	for (int neighbourDistance : neighbourDistances) {
		if (neighbourDistance == 255) {
			result.unknown++;
			return;
		}
		closest = min(closest, neighbourDistance);
		if (abs(neighbourDistance - distance) > 1)
			consistent = false;
	}
	if (goal ? distance != 0 : distance == 0 || closest != distance - 1)
		consistent = false;

	result.checked++;
	if (!consistent)
		result.errors++;
}

static bool report(string name, VerifyResult& result) {
	cout << name << ": " << result.checked << " positions checked, " << result.errors << " errors, " << result.unknown << " unknown\n";
	return result.errors == 0 && result.unknown == 0;
}

// Random positions plus positions a few random moves from the goal, so that every depth gets checked
static vector<pair<int, int>> samplePositions(int rows, int columns, pair<int, int> goal, const vector<int>& moves,
	function<int(int, int)> rowMove, function<int(int, int)> columnMove, int count) {

	mt19937 rng(2024);
	vector<pair<int, int>> positions;
	for (int i = 0; i < count; i++)
		positions.push_back({ (int)(rng() % rows), (int)(rng() % columns) });
	for (int i = 0; i < count; i++) {
		pair<int, int> position = goal;
		int length = rng() % 16;
		for (int j = 0; j < length; j++) {
			int move = moves[rng() % moves.size()];
			position = { rowMove(position.first, move), columnMove(position.second, move) };
		}
		positions.push_back(position);
	}
	return positions;
}

bool verifyTables(MoveController& moveController) {
	CubeIndexer cubeIndexer;
	ReverseCubeIndexer reverseCubeIndexer;
	SymmetryIndexer symmetryIndexer;
	bool allCorrect = true;

	vector<int> allMoves, phase2Moves;
	for (int move = 0; move < 18; move++) {
		allMoves.push_back(move);
		if ((1 << move) & availableMovesG2)
			phase2Moves.push_back(move);
	}
	CubeIndexModel solved;

	auto databaseG1 = make_unique<G1DatabaseController>();
	databaseG1->initializeTables();

	auto flipUDSliceSymmetry = [&](int flipUDSlice, int symmetry) {
		CubeIndexModel cube;
		reverseCubeIndexer.updateCubeEdgeMComb(cube, flipUDSlice / 2048);
		reverseCubeIndexer.updateCubeEdgeOrientaion(cube, flipUDSlice % 2048);
		CubeIndexModel symmetric = symmetryIndexer.overlaySymmetry(cube, symmetry);
		return cubeIndexer.getEdgeOrientationIndex(symmetric) + cubeIndexer.getUDSliceIndex(symmetric) * 2048;
	};
	auto cornerOriSymmetry = [&](int cornerOri, int symmetry) {
		CubeIndexModel cube;
		reverseCubeIndexer.updateCubeCornerOrientation(cube, cornerOri);
		CubeIndexModel symmetric = symmetryIndexer.overlaySymmetry(cube, symmetry);
		return cubeIndexer.getCornerOrientationIndex(symmetric);
	};

	pair<int, int> goalG1 = { cubeIndexer.getEdgeOrientationIndex(solved) + cubeIndexer.getUDSliceIndex(solved) * 2048, cubeIndexer.getCornerOrientationIndex(solved) };
	VerifyResult resultG1, symmetryG1;
	for (auto position : samplePositions(1013760, 2187, goalG1, allMoves,
		[&](int row, int move) { return moveController.updateFlipUDSlice(row, move); },
		[&](int column, int move) { return moveController.updateCornerOrientaion(column, move); }, 200000)) {

		int distance = databaseG1->getNumMoves(position.first, position.second);
		vector<int> neighbourDistances;
		for (int move : allMoves) {
			int flipUDSlice = moveController.updateFlipUDSlice(position.first, move);
			int cornerOri = moveController.updateCornerOrientaion(position.second, move);
			neighbourDistances.push_back(databaseG1->getNumMoves(flipUDSlice, cornerOri));
		}
		checkPosition(distance, neighbourDistances, position == goalG1, resultG1);

		for (int symmetry = 0; symmetry < 16; symmetry++) {
			int flipUDSlice = flipUDSliceSymmetry(position.first, symmetry);
			int cornerOri = cornerOriSymmetry(position.second, symmetry);
			symmetryG1.checked++;
			if (databaseG1->getNumMoves(flipUDSlice, cornerOri) != distance)
				symmetryG1.errors++;
		}
	}
	allCorrect &= report("KociembaG1", resultG1);
	allCorrect &= report("KociembaG1 symmetric positions", symmetryG1);
	databaseG1.reset();

	auto databaseG2 = make_unique<G2DatabaseController>();
	databaseG2->initializeTables();

	int goalCornerPerm = cubeIndexer.getCornerPermIndex(solved);
	int goalEdgeRPerm = cubeIndexer.getEdgeRPermIndex(solved);
	int goalEdgeMPerm = cubeIndexer.getEdgeMPermIndex(solved);

	VerifyResult resultCorner;
	for (int cornerPerm = 0; cornerPerm < 40320; cornerPerm++) {
		vector<int> neighbourDistances;
		for (int move : phase2Moves) {
			int neighbour = moveController.updateCornerPerm(cornerPerm, move);
			neighbourDistances.push_back(databaseG2->getNumCornPermMoves(neighbour));
		}
		checkPosition(databaseG2->getNumCornPermMoves(cornerPerm), neighbourDistances, cornerPerm == goalCornerPerm, resultCorner);
	}
	allCorrect &= report("KociembaG2Corner", resultCorner);

	VerifyResult resultEdge;
	for (int edgeMPerm = 0; edgeMPerm < 24; edgeMPerm++) {
		for (int edgeRPerm = 0; edgeRPerm < 40320; edgeRPerm++) {
			vector<int> neighbourDistances;
			for (int move : phase2Moves) {
				int neighbourR = moveController.updateEdgeRPerm(edgeRPerm, move);
				int neighbourM = moveController.updateEdgeMPerm(edgeMPerm, move);
				neighbourDistances.push_back(databaseG2->getNumEdgePermMoves(neighbourR, neighbourM));
			}
			bool goal = edgeRPerm == goalEdgeRPerm && edgeMPerm == goalEdgeMPerm;
			checkPosition(databaseG2->getNumEdgePermMoves(edgeRPerm, edgeMPerm), neighbourDistances, goal, resultEdge);
		}
	}
	allCorrect &= report("KociembaG2Edge", resultEdge);

	auto cornerPermSymmetry = [&](int cornerPerm, int symmetry) {
		CubeIndexModel cube;
		reverseCubeIndexer.updateCubeCornerPerm(cube, cornerPerm);
		CubeIndexModel symmetric = symmetryIndexer.overlaySymmetry(cube, symmetry);
		return cubeIndexer.getCornerPermIndex(symmetric);
	};
	auto edgeRPermSymmetry = [&](int edgeRPerm, int symmetry) {
		CubeIndexModel cube;
		reverseCubeIndexer.updateCubeEdgeRPerm(cube, edgeRPerm);
		CubeIndexModel symmetric = symmetryIndexer.overlaySymmetry(cube, symmetry);
		return cubeIndexer.getEdgeRPermIndex(symmetric);
	};

	pair<int, int> goalCubie = { goalCornerPerm, goalEdgeRPerm };
	VerifyResult resultCubie, symmetryCubie;
	for (auto position : samplePositions(40320, 40320, goalCubie, phase2Moves,
		[&](int row, int move) { return moveController.updateCornerPerm(row, move); },
		[&](int column, int move) { return moveController.updateEdgeRPerm(column, move); }, 200000)) {

		int distance = databaseG2->getNumCubiePermMoves(position.first, position.second);
		vector<int> neighbourDistances;
		for (int move : phase2Moves) {
			int cornerPerm = moveController.updateCornerPerm(position.first, move);
			int edgeRPerm = moveController.updateEdgeRPerm(position.second, move);
			neighbourDistances.push_back(databaseG2->getNumCubiePermMoves(cornerPerm, edgeRPerm));
		}
		checkPosition(distance, neighbourDistances, position == goalCubie, resultCubie);

		for (int symmetry = 0; symmetry < 16; symmetry++) {
			int cornerPerm = cornerPermSymmetry(position.first, symmetry);
			int edgeRPerm = edgeRPermSymmetry(position.second, symmetry);
			symmetryCubie.checked++;
			if (databaseG2->getNumCubiePermMoves(cornerPerm, edgeRPerm) != distance)
				symmetryCubie.errors++;
		}
	}
	allCorrect &= report("KociembaG2Cubie", resultCubie);
	allCorrect &= report("KociembaG2Cubie symmetric positions", symmetryCubie);

	cout << (allCorrect ? "All tables are correct\n" : "SOME TABLES ARE WRONG OR INCOMPLETE\n");
	return allCorrect;
}
