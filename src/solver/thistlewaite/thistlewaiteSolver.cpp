#include "solver/thistlewaite/thistlewaiteSolver.h"
#include "solver/thistlewaite/pDatabaseG1.h"
#include "solver/thistlewaite/pDatabaseG2.h"
#include "solver/thistlewaite/pDatabaseG3.h"
#include "solver/thistlewaite/pDatabaseG4.h"

#include "cube/moveStore.h"
#include "solver/simplifySolution.h"
#include "solver/cubeSolver.h"

#include <iostream>

using namespace std;

ThistlewaiteSolver::ThistlewaiteSolver(bool initializeTablesOnCreate) {

	if (initializeTablesOnCreate)
		initializeTables();
}

void ThistlewaiteSolver::initializeTables() {
	PatternDatabaseG1 dDatabaseG1 = PatternDatabaseG1();
	PatternDatabaseG2 dDatabaseG2 = PatternDatabaseG2();
	PatternDatabaseG3 dDatabaseG3 = PatternDatabaseG3();
	PatternDatabaseG4 dDatabaseG4 = PatternDatabaseG4();

	dDatabaseG1.readThread.join();
	dDatabaseG2.readThread.join();
	dDatabaseG3.readThread.join();
	dDatabaseG4.readThread.join();

	pDatabaseG1 = &dDatabaseG1;
	pDatabaseG2 = &dDatabaseG2;
	pDatabaseG3 = &dDatabaseG3;
	pDatabaseG4 = &dDatabaseG4;

	tablesInitialized = true;
}

vector<int> ThistlewaiteSolver::solveCubeBFS(CubeIndexModel cube) {
	CubeSolver cubeSolver;
	SolutionSimplifier solutionSimplifier;
	MoveStore moveStore;
	vector<int> solution;

	vector<int> solutionG1 = cubeSolver.solveBFS(pDatabaseG1, cube, availableMoves[0]);
	for (int move : solutionG1)
		cube.doMove(move);
	solution.insert(solution.end(), solutionG1.begin(), solutionG1.end());

	vector<int> solutionG2 = cubeSolver.solveBFS(pDatabaseG2, cube, availableMoves[1]);
	for (int move : solutionG2)
		cube.doMove(move);
	solution.insert(solution.end(), solutionG2.begin(), solutionG2.end());

	vector<int> solutionG3 = cubeSolver.solveBFS(pDatabaseG3, cube, availableMoves[2]);
	for (int move : solutionG3)
		cube.doMove(move);
	solution.insert(solution.end(), solutionG3.begin(), solutionG3.end());

	vector<int> solutionG4 = cubeSolver.solveBFS(pDatabaseG4, cube, availableMoves[3]);
	for (int move : solutionG4)
		cube.doMove(move);
	solution.insert(solution.end(), solutionG4.begin(), solutionG4.end());

	solutionSimplifier.simplifySolution(solution);
	cout << "Found solution with " << solution.size() << " moves!\n";
	moveStore.printMoveSequence(solution);
	cout << "\n";
	return solution;
}


vector<int> ThistlewaiteSolver::solveCubeIDA(CubeIndexModel cube) {
	CubeSolver cubeSolver;
	SolutionSimplifier solutionSimplifier;
	MoveStore moveStore;
	vector<int> solution;

	vector<int> solutionG1 = cubeSolver.solveIDA(pDatabaseG1, cube, availableMoves[0]);
	for (int move : solutionG1)
		cube.doMove(move);
	solution.insert(solution.end(), solutionG1.begin(), solutionG1.end());

	vector<int> solutionG2 = cubeSolver.solveIDA(pDatabaseG2, cube, availableMoves[1]);
	for (int move : solutionG2)
		cube.doMove(move);
	solution.insert(solution.end(), solutionG2.begin(), solutionG2.end());

	vector<int> solutionG3 = cubeSolver.solveIDA(pDatabaseG3, cube, availableMoves[2]);
	for (int move : solutionG3)
		cube.doMove(move);
	solution.insert(solution.end(), solutionG3.begin(), solutionG3.end());

	vector<int> solutionG4 = cubeSolver.solveIDA(pDatabaseG4, cube, availableMoves[3]);
	for (int move : solutionG4)
		cube.doMove(move);
	solution.insert(solution.end(), solutionG4.begin(), solutionG4.end());

	solutionSimplifier.simplifySolution(solution);
	cout << "Found solution with " << solution.size() << " moves!\n";
	moveStore.printMoveSequence(solution);
	cout << "\n";
	return solution;
}
