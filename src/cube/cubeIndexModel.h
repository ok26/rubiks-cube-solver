#ifndef CUBE_INDEX_H
#define CUBE_INDEX_H

#include <array>

struct Cubie {
	int index;
	int orientation;
};

enum indexMove {R, Rp, R2, L, Lp, L2, U, Up, U2, D, Dp, D2, F, Fp, F2, B, Bp, B2};

class CubeIndexModel {
public:
	std::array<Cubie, 12> edges;
	std::array<Cubie, 8> corners;
	std::array<int, 6> centers = {0, 1, 2, 3, 4, 5};

	CubeIndexModel(bool blank = false);
	CubeIndexModel(const CubeIndexModel& cubeCopy);

	bool isSolved();
	void printData();
	void updateEdgeOrientation(int affectedFace);
	void updateCubieIndex(int affectedFace, bool inverse, bool doubleMove);
	void fullRotation(int axis, bool inverse);
	void doMove(int indexMove);
	void doReverseMove(int move);
};

#endif
