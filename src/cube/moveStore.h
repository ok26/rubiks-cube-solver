#ifndef MOVE_STORE_H
#define MOVE_STORE_H

#include <array>
#include <string>
#include <vector>

struct Move {
	int target;
	float duration;
	int axis;
	bool inverse;
};

class MoveStore {
public:
	std::array<std::string, 18> cubeMoveNames = { "R", "R'", "R2", "L", "L'", "L2",
												"U", "U'", "U2", "D", "D'", "D2",
												"F", "F'", "F2", "B", "B'", "B2" };
	std::vector<int> targetedFace = { 2, 3, 0, 5, 1, 4 };

	void printMoveSequence(std::vector<int> sequence);

	//Only supports symCoord 0 and 1 for triple search
	std::vector<int> translateSymmetryMoves(std::vector<int> moves, int symmetryCoord);

	std::vector<Move> indexToAnimationMove(std::vector<int> moveSequence, float duration);
	int animationMoveToIndex(Move move);
};

#endif // !MOVE_STORE_H
