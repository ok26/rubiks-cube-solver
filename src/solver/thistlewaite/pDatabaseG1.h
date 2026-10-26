#ifndef THISTLEWAITE_G1_H
#define THISTLEWAITE_G1_H

#include "solver/patternDatabase.h"

class PatternDatabaseG1 : public PatternDatabase {
public:

	PatternDatabaseG1();

	int64_t getDatabaseIndex(const CubeIndexModel& cube) override;
	int getNumMoves(const CubeIndexModel& cube) override;
};

#endif
