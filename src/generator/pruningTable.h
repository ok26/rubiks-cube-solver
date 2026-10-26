#ifndef PRUNING_TABLE_H
#define PRUNING_TABLE_H

#include "generator/symmetryClasses.h"

#include <cstdint>
#include <string>
#include <vector>

// Distances from the goal for every (row, column) coordinate pair, found with a breadth-first search.
// Rows can be reduced by symmetry; the distance of a pair is then stored at
// column' + columns * classOf[row], where column' is the column transformed the same way the solver
// does it when it looks the distance up.
class PruningTable {
public:
	std::string name;
	SymmetryClasses rowClasses;
	int columns = 1;
	int moveCount = 0;
	std::vector<int> rowMoveTable;
	std::vector<int> columnMoveTable;
	std::vector<int> columnSymmetryTable;
	std::vector<uint8_t> distances;

	PruningTable(std::string name);

	void generate(int goalRow, int goalColumn, int maxDepth = 255);
	void writeFile();

private:
	int64_t filled = 0;

	int transformColumn(int column, int symmetry);
	int64_t entryIndex(int row, int column);
	void setDistance(int64_t index, uint8_t distance);
	int64_t estimateBackwardLookups(int depth);
};

#endif
