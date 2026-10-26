#include "solver/kociemba/g1DatabaseController.h"

#include <fstream>
#include <iostream>

using namespace std;

void G1DatabaseController::initializeTables() {
	readSymmetryData("resources/data/KociembaG1Symmetry");
	readReducedData("resources/data/KociembaG1");
	generateCornerSymIndexEquivalences();
}

void G1DatabaseController::generateCornerSymIndexEquivalences() {
	for (int i = 0; i < 2187; i++) {
		CubeIndexModel cube;
		reverseCubeIndexer.updateCubeCornerOrientation(cube, i);
		for (int sym = 0; sym < 16; sym++) {
			CubeIndexModel cubeSymmetry = symmetryIndexer.overlaySymmetry(cube, sym);
			cornerSymmetryIndexEquivalence[i][sym] = cubeIndexer.getCornerOrientationIndex(cubeSymmetry);
		}
	}
	std::cout << "Finished generating CornerSymEquivalences\n";
}

void G1DatabaseController::readSymmetryData(string fileName) {
	ifstream input(fileName);
	string data;
	input >> data;
	int current = 0;
	for (int i = 0; i < data.length(); i) {
		int ID = (int)(data[i] - 'a') + (int)(data[i + 1] - 'a') * 24 + (int)(data[i + 2] - 'a') * 576 + (int)(data[i + 3] - 'a') * 13824;
		symmetryDatabase[current] = { ID, (int)(data[i + 4] - 'A') };

		int j = i + 4;
		while ((int)(data[j] - 'A') < 16 && j < data.length()) {
			j++;
		}
		i = j;
		current++;
	}
	cout << "Finished reading from: " << fileName << "\n";
}

void G1DatabaseController::readReducedData(string fileName) {
	ifstream input(fileName);
	string data;
	input >> data;
	for (int i = 0; i < data.length(); i++)
		reducedDatabase[i] = (uint8_t)(data[i] - 'a');
	input.close();
	std::cout << "Finished reading from: " << fileName << "\n";
}

int G1DatabaseController::getNumMoves(int& flipUDSlice, int& cornerOrientation) {

	std::pair<int, int> symmetryIndex = symmetryDatabase[flipUDSlice];
	int cornerSymmetryIndex = cornerSymmetryIndexEquivalence[cornerOrientation][reverseEquivalences[symmetryIndex.second]];

	return reducedDatabase[cornerSymmetryIndex + symmetryIndex.first * 2187];
}
