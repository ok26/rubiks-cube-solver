#include "solver/kociemba/g2DatabaseController.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

void G2DatabaseController::initializeTables() {
	generateEdgeMPermEquivalenceTable();
	generateEdgeRPermEquivalenceTable();
	generateUAndDSliceSortReductionTable();
	generateEdgeRPermSymEquivalences();

	readSymmetryData("resources/data/KociembaG2Symmetry");
	readCornerPermDatabase("resources/data/KociembaG2Corner");
	readEdgePermDatabase("resources/data/KociembaG2Edge");
	readCubiePermDatabase("resources/data/KociembaG2Cubie");
}

void G2DatabaseController::readSymmetryData(string fileName) {
	ifstream input(fileName);
	string data;
	input >> data;
	int current = 0;
	for (int i = 0; i < data.length(); i) {
		int ID = (int)(data[i] - 'a') + (int)(data[i + 1] - 'a') * 24 + (int)(data[i + 2] - 'a') * 576;
		cornerPermSymmetry[current] = { ID, (int)(data[i + 3] - 'A') };

		int j = i + 3;
		while ((int)(data[j] - 'A') < 16 && j < data.length()) {
			j++;
		}
		i = j;
		current++;
	}
	cout << "Finished reading from: " << fileName << "\n";
}

void G2DatabaseController::readCornerPermDatabase(string fileName) {
	ifstream input(fileName);
	string data;
	input >> data;
	for (int i = 0; i < data.length(); i++)
		cornerPermDatabase[i] = (uint8_t)(data[i] - 'a');
	input.close();
	std::cout << "Finished reading from: " << fileName << "\n";
}

void G2DatabaseController::readEdgePermDatabase(string fileName) {
	ifstream input(fileName);
	string data;
	input >> data;
	for (int i = 0; i < data.length(); i++)
		edgePermDatabase[i] = (uint8_t)(data[i] - 'a');
	input.close();
	std::cout << "Finished reading from: " << fileName << "\n";
}

void G2DatabaseController::readCubiePermDatabase(string fileName) {
	ifstream input(fileName);
	string data;
	input >> data;
	for (int i = 0; i < data.length(); i++)
		cubiePermDatabase[i] = (uint8_t)(data[i] - 'a');
	input.close();
	std::cout << "Finished reading from: " << fileName << "\n";
}

void G2DatabaseController::generateEdgeRPermSymEquivalences() {
	for (int i = 0; i < 40320; i++) {
		CubeIndexModel cube;
		reverseCubeIndexer.updateCubeEdgeRPerm(cube, i);
		for (int sym = 0; sym < 16; sym++) {
			CubeIndexModel cubeSymmetry = symmetryIndexer.overlaySymmetry(cube, sym);
			edgeRPermSymmetryEquivalence[i][sym] = cubeIndexer.getEdgeRPermIndex(cubeSymmetry);
		}
	}
	std::cout << "Finished generating EdgeRPermSymEquivalences\n";
}

void G2DatabaseController::generateEdgeMPermEquivalenceTable() {
	std::array<int, 4> posPerm, indexPerm;
	std::vector<int> permutationV = { 4, 5, 6, 7 };

	do {
		for (int i = 0; i < 4; i++) {
			posPerm[i] = permutationV[i];
			indexPerm[permutationV[i] - 4] = i;
		}
		edgeMPermFromUDSliceSort[perm12Indexer.lehmerIndex(posPerm)] = fullPerm4Indexer.lehmerIndex(indexPerm);

	} while (next_permutation(permutationV.begin(), permutationV.end()));

	std::cout << "Finished generating EdgeMPerm equivalence table\n";
}

void G2DatabaseController::generateUAndDSliceSortReductionTable() {
	std::vector<int> combination(4, 1);
	combination.resize(8, 0);

	do {
		std::array<int, 4> permutation;
		std::vector<int> permutationV(4);
		int cur = 0;
		for (int i = 0; i < 8; i++)
			if (combination[i])
				permutationV[cur++] = i;

		for (int i = 0; i < 24; i++) {

			for (int j = 0; j < 4; j++)
				permutation[j] = permutationV[j];

			int reducedIndex = perm8Indexer.lehmerIndex(permutation);

			for (int j = 0; j < 4; j++)
				if (permutation[j] > 3)
					permutation[j] += 4;

			int fullIndex = perm12Indexer.lehmerIndex(permutation);
			reduceUAndDSliceSort[fullIndex] = reducedIndex;

			next_permutation(permutationV.begin(), permutationV.end());
		}
	} while (prev_permutation(combination.begin(), combination.end()));

	std::cout << "Finished generating EdgeUAndD reduction table\n";
}

void G2DatabaseController::generateEdgeRPermEquivalenceTable() {
	std::array<int, 4> permU, permD;
	std::array<int, 8> permutation;
	std::vector<int> permutationV = { 0, 1, 2, 3, 4, 5, 6, 7 };

	do {

		for (int i = 0; i < 8; i++) {

			if (permutationV[i] < 4)
				permU[permutationV[i]] = i;
			else
				permD[permutationV[i] - 4] = i;

			permutation[i] = permutationV[i];
		}

		int USliceIndex = perm8Indexer.lehmerIndex(permU);
		int DSliceIndex = perm8Indexer.lehmerIndex(permD);
		int edgeRPermIndex = fullPerm8Indexer.lehmerIndex(permutation);

		edgeRPermFromUAndDSliceSort[USliceIndex][DSliceIndex] = edgeRPermIndex;

	} while (next_permutation(permutationV.begin(), permutationV.end()));

	std::cout << "Finished generating EdgeRPerm equivalence table\n";
}

int G2DatabaseController::getEdgeMPerm(int& UDSliceSorted) {
	return edgeMPermFromUDSliceSort[UDSliceSorted];
}

int G2DatabaseController::getEdgeRPerm(int& USliceSorted, int& DSliceSorted) {
	int reducedUSliceIndex = reduceUAndDSliceSort[USliceSorted];
	int reducedDSliceIndex = reduceUAndDSliceSort[DSliceSorted];
	return edgeRPermFromUAndDSliceSort[reducedUSliceIndex][reducedDSliceIndex];
}

int G2DatabaseController::getNumCornPermMoves(int& cornerPermutaionIndex) {
	return cornerPermDatabase[cornerPermutaionIndex];
}

int G2DatabaseController::getNumEdgePermMoves(int& edgeRPermIndex, int& edgeMPermIndex) {
	return edgePermDatabase[edgeRPermIndex + edgeMPermIndex * 40320];
}

int G2DatabaseController::getNumCubiePermMoves(int& cornerPermIndex, int& edgeRPermIndex) {
	std::pair<int, int> symmetryIndex = cornerPermSymmetry[cornerPermIndex];
	int edgeRPermSymmetryIndex = edgeRPermSymmetryEquivalence[edgeRPermIndex][reverseEquivalences[symmetryIndex.second]];

	return cubiePermDatabase[edgeRPermSymmetryIndex + symmetryIndex.first * 40320];
}

int G2DatabaseController::getNumMoves(int& cornerPermutation, int& edgeMPermutation, int& edgeRPermutaion) {
	int edgePermEstimation = getNumEdgePermMoves(edgeRPermutaion, edgeMPermutation);
	int cubiePermEstimation = getNumCubiePermMoves(cornerPermutation, edgeRPermutaion);
	return std::max(edgePermEstimation, cubiePermEstimation);
}
