#include "solver/patternDatabase.h"

#include <fstream>
#include <iostream>

using namespace std;

void PatternDatabase::readHashData(string fileName) {
	ifstream input(fileName);
	int64_t hash;
	int numMoves;
	while (input >> hash >> numMoves)
		hashDatabase[hash] = numMoves;
	input.close();
	std::cout << "Finished reading from: " << fileName << "\n";
}

void PatternDatabase::readLehmerData(string fileName) {
	ifstream input(fileName);
	string data;
	input >> data;
	for (int i = 0; i < data.length(); i++)
		database[i] = (int)(data[i] - 'a');
	input.close();
	std::cout << "Finished reading from: " << fileName << "\n";
}

int64_t PatternDatabase::getDatabaseIndex(const CubeIndexModel& cube) {
	cout << "Failed to initialize getDatabaseIndex\n";
	return 0;
}

int PatternDatabase::getNumMoves(const CubeIndexModel& cube) {
	int64_t ID = this->getDatabaseIndex(cube);
	return database[ID];
}
