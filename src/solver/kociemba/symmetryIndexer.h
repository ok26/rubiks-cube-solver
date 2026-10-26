#ifndef SYMMETRY_INDEXER_H
#define SYMMETRY_INDEXER_H

#include "cube/cubeIndexModel.h"

#include <array>

extern const std::array<int, 16> reverseEquivalences;

class SymmetryIndexer {
public:

	CubeIndexModel overlaySymmetry(const CubeIndexModel& cube, int symmetryCoord);
	CubeIndexModel overlayTripleSearchSymmetry(const CubeIndexModel& cube, int symmetryCoord);
	CubeIndexModel backwardsSymmetryOverlay(const CubeIndexModel& cubeSymmetry, int symmetryCoord);
};

#endif
