#ifndef PATTERN_DATABASE_H
#define PATTERN_DATABASE_H

#include "cube/cubeIndexModel.h"

#include <cstdint>
#include <string>
#include <map>
#include <vector>
#include <thread>

class PatternDatabase {
public:
	std::vector<int> database;
	std::map<int64_t, int> hashDatabase;
	std::thread readThread;

	void readHashData(std::string fileName);
	void readLehmerData(std::string fileName);

	virtual int64_t getDatabaseIndex(const CubeIndexModel& cube);
	virtual int getNumMoves(const CubeIndexModel& cube);
};

#endif // !PATTERN_DATABASE_H
