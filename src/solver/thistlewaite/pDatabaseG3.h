#ifndef THISTLEWAITE_G3_H
#define THISTLEWAITE_G3_H

#include "solver/patternDatabase.h"

class PatternDatabaseG3 : public PatternDatabase {
public:

	PatternDatabaseG3();

	int64_t getDatabaseIndex(const CubeIndexModel& cube) override;
	int getNumMoves(const CubeIndexModel& cube) override;
};

#endif
