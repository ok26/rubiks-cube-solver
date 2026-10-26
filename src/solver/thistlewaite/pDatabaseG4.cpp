#include "solver/thistlewaite/pDatabaseG4.h"

using namespace std;

PatternDatabaseG4::PatternDatabaseG4() : PatternDatabase() {
	readThread = thread(&PatternDatabase::readHashData, this, "resources/data/ThistlewaiteG4");
}

int64_t PatternDatabaseG4::getDatabaseIndex(const CubeIndexModel& cube) {
	int64_t ID = 1;

	for (int i = 0; i < 12; i++) {
		if (cube.edges[i].index < 4)
			ID = ID << 2;
		else if (cube.edges[i].index < 8)
			ID = ID << 3;
		else
			ID = ID << 4;
		ID += cube.edges[i].index;
	}
	for (int i = 0; i < 8; i++) {
		ID = ID << 3;
		ID += cube.corners[i].index;
	}

	return ID;
}

int PatternDatabaseG4::getNumMoves(const CubeIndexModel& cube) {
	int64_t ID = this->getDatabaseIndex(cube);
	return hashDatabase[ID];
}
