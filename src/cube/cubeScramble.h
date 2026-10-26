#ifndef CUBE_SCRAMBLE_H
#define CUBE_SCRAMBLE_H

#include "cube/cubeIndexModel.h"
#include "cube/moveStore.h"

#include <queue>

class CubeScrambler {
public:
	CubeScrambler(unsigned int seed);

	void scrambleCube100(std::queue<Move> *moveQueue);
	void scrambleCube(CubeIndexModel& cube, int moves);
	void scrambleCubeWithRandomMoveNr(std::queue<Move>* moveQueue, int upperBound, int lowerBound, float animationDuration);
	int getAxis(int targetSide);
};

#endif // !CUBE_SCRAMBLE_H
