#include "cube/moveStore.h"
#include "cube/cubeIndexModel.h"

#include <algorithm>
#include <iostream>

using namespace std;

void MoveStore::printMoveSequence(vector<int> sequence) {
	for (int move : sequence)
		cout << cubeMoveNames[move] << " ";
	cout << "\n";
}

std::vector<int> MoveStore::translateSymmetryMoves(vector<int> moves, int symmetryCoord) {
	std::array<int, 18> moveTranslations;
	if (symmetryCoord == 0) moveTranslations = { U, Up, U2, D, Dp, D2, L, Lp, L2, R, Rp, R2, F, Fp, F2, B, Bp, B2 };
	else moveTranslations = { R, Rp, R2, L, Lp, L2, F, Fp, F2, B, Bp, B2, D, Dp, D2, U, Up, U2 };

	std::vector<int> translatedMoves;
	for (int i = 0; i < moves.size(); i++)
		translatedMoves.push_back(moveTranslations[moves[i]]);
	return translatedMoves;
}

vector<Move> MoveStore::indexToAnimationMove(vector<int> moveSequence, float duration) {
	vector<Move> converted;

	for (int move : moveSequence) {
		Move convertedMove;
		convertedMove.target = targetedFace[move / 3];
		convertedMove.axis = move / 6;
		convertedMove.duration = duration;

		if (move % 3 == 1)
			convertedMove.inverse = true;
		else
			convertedMove.inverse = false;

		//temporary
		if ((move / 3) % 2 == 0)
			convertedMove.inverse = !convertedMove.inverse;

		//still temporary
		if (move % 3 == 2)
			converted.push_back(convertedMove);

		//not temporary
		converted.push_back(convertedMove);
	}
	return converted;
}

int MoveStore::animationMoveToIndex(Move move) {
	int diff = move.inverse;
	if (move.target < 3)
		diff = 1 - diff;
	return 3 * (find(targetedFace.begin(), targetedFace.end(), move.target) - targetedFace.begin()) + diff;
}
