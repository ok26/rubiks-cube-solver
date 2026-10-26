#include "solver/thistlewaite/pDatabaseG1.h"

using namespace std;

PatternDatabaseG1::PatternDatabaseG1() : PatternDatabase() {
	readThread = thread(&PatternDatabase::readHashData, this, "resources/data/ThistlewaiteG1");
}

int64_t PatternDatabaseG1::getDatabaseIndex(const CubeIndexModel& cube) {
	int64_t ID = 1;

	for (int i = 0; i < 12; i++) {
		ID = ID << 1;
		ID += cube.edges[i].orientation;
	}

	return ID;
}

int PatternDatabaseG1::getNumMoves(const CubeIndexModel& cube) {
	int64_t ID = this->getDatabaseIndex(cube);
	return hashDatabase[ID];
}
