#ifndef DATABASE_G2_CONTROLLER_H
#define DATABASE_G2_CONTROLLER_H

#include "solver/kociemba/patternIndexer.h"
#include "solver/kociemba/symmetryIndexer.h"

#include <array>
#include <cstdint>
#include <string>
#include <utility>

class G2DatabaseController {

	std::array<uint8_t, 40320>				cornerPermDatabase;
	std::array<uint8_t, 967680>				edgePermDatabase;
	std::array<uint8_t, 111605760>			cubiePermDatabase;
	std::array<std::pair<int, int>, 40320>	cornerPermSymmetry;
	std::array<std::array<int, 16>, 40320>	edgeRPermSymmetryEquivalence;

	std::array<int, 11880>					edgeMPermFromUDSliceSort;
	std::array<int, 11880>					reduceUAndDSliceSort;
	std::array<std::array<int, 1680>, 1680>	edgeRPermFromUAndDSliceSort;

	PermutationIndexer<12, 4>	perm12Indexer;
	PermutationIndexer<8, 4>	perm8Indexer;
	PermutationIndexer<8>		fullPerm8Indexer;
	PermutationIndexer<4>		fullPerm4Indexer;

	ReverseCubeIndexer reverseCubeIndexer;
	SymmetryIndexer symmetryIndexer;
	CubeIndexer cubeIndexer;

public:

	void initializeTables();

	void readSymmetryData(std::string fileName);
	void readCornerPermDatabase(std::string fileName);
	void readEdgePermDatabase(std::string fileName);
	void readCubiePermDatabase(std::string fileName);

	void generateEdgeRPermSymEquivalences();
	void generateEdgeMPermEquivalenceTable();
	void generateUAndDSliceSortReductionTable();
	void generateEdgeRPermEquivalenceTable();

	int getEdgeMPerm(int& UDSliceSorted);
	int getEdgeRPerm(int& USliceSorted, int& DSliceSorted);
	int getNumCornPermMoves(int& cornerPermutaionIndex);
	int getNumEdgePermMoves(int& edgeRPermIndex, int& edgeMPermIndex);
	int getNumCubiePermMoves(int& cornerPermIndex, int& edgeRPermIndex);
	int getNumMoves(int& cornerPermutation, int& edgeMPermutation, int& edgeRPermutaion);
};

#endif
