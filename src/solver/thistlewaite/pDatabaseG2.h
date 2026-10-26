#ifndef THISTLEWAITE_G2_H
#define THISTLEWAITE_G2_H

#include "solver/patternDatabase.h"

class PatternDatabaseG2 : public PatternDatabase {
public:

	PatternDatabaseG2();

	int64_t getDatabaseIndex(const CubeIndexModel& cube) override;
	int getNumMoves(const CubeIndexModel& cube) override;
};

#endif
