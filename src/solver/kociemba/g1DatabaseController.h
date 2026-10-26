#ifndef DATABASE_G1_CONTROLLER_H
#define DATABASE_G1_CONTROLLER_H

#include "solver/kociemba/patternIndexer.h"
#include "solver/kociemba/symmetryIndexer.h"

#include <array>
#include <cstdint>
#include <string>
#include <utility>

class G1DatabaseController {

	std::array<uint8_t, 140908410>				reducedDatabase;
	std::array<std::pair<int, int>, 1013760>	symmetryDatabase;
	std::array<std::array<int, 16>, 2187>		cornerSymmetryIndexEquivalence;

	ReverseCubeIndexer reverseCubeIndexer;
	SymmetryIndexer symmetryIndexer;
	CubeIndexer cubeIndexer;

public:

	void initializeTables();
	void generateCornerSymIndexEquivalences();
	void readSymmetryData(std::string fileName);
	void readReducedData(std::string fileName);
	int getNumMoves(int& flipUDSlice, int& cornerOrientation);
};

#endif
