#ifndef THISTLEWAITE_G4_H
#define THISTLEWAITE_G4_H

#include "solver/patternDatabase.h"

class PatternDatabaseG4 : public PatternDatabase {
public:

	PatternDatabaseG4();

	int64_t getDatabaseIndex(const CubeIndexModel& cube) override;
	int getNumMoves(const CubeIndexModel& cube) override;
};

#endif
