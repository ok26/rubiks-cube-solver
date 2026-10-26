#include "cube/cubeScramble.h"

#include <cstdlib>

using namespace std;

CubeScrambler::CubeScrambler(unsigned int seed) {
	srand(seed);
}

void CubeScrambler::scrambleCube100(queue<Move> *moveQueue) {
	for (int i = 0; i < 100; i++) {
		Move move;
		move.target = rand() % 6;
		while (!moveQueue->empty() && move.target == moveQueue->back().target)
			move.target = rand() % 6;
		move.duration = 0.01f;
		move.axis = getAxis(move.target);
		move.inverse = rand() % 2;
		moveQueue->push(move);
	}
}

void CubeScrambler::scrambleCube(CubeIndexModel& cube, int moves) {
	int lastMove = -1;
	for (int i = 0; i < moves; i++) {
		int move = rand() % 18;
		while ((move % 3) == (lastMove % 3)) move = rand() % 18;
		cube.doMove(move);
	}
}

void CubeScrambler::scrambleCubeWithRandomMoveNr(queue<Move>* moveQueue, int upperBound, int lowerBound, float animationDuration) {
	int moveNr = (rand() % (upperBound - lowerBound + 1)) + lowerBound;
	for (int i = 0; i < moveNr; i++) {
		Move move;
		move.target = rand() % 6;
		while (!moveQueue->empty() && move.target == moveQueue->back().target)
			move.target = rand() % 6;
		move.duration = animationDuration;
		move.axis = getAxis(move.target);
		move.inverse = rand() % 2;
		moveQueue->push(move);
	}
}

int CubeScrambler::getAxis(int targetSide) {
	if (targetSide == 0 || targetSide == 5)
		return 1;
	if (targetSide == 1 || targetSide == 4)
		return 2;
	if (targetSide == 2 || targetSide == 3)
		return 0;
	return 0;
}
