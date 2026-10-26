#include "generator/pruningTable.h"
#include "solver/kociemba/symmetryIndexer.h"

#include <chrono>
#include <fstream>
#include <iostream>
#include <random>

using namespace std;

static const uint8_t EMPTY = 0xFF;

PruningTable::PruningTable(string name) {
	this->name = name;
}

int PruningTable::transformColumn(int column, int symmetry) {
	if (columnSymmetryTable.empty())
		return column;
	return columnSymmetryTable[column * 16 + symmetry];
}

int64_t PruningTable::entryIndex(int row, int column) {
	int symmetry = reverseEquivalences[rowClasses.symmetryOf[row]];
	return transformColumn(column, symmetry) + (int64_t)columns * rowClasses.classOf[row];
}

// When the row representative is mapped to itself by more symmetries than the identity, several columns
// in its row describe the same cube position, so they must all get the same distance
void PruningTable::setDistance(int64_t index, uint8_t distance) {
	int classIndex = index / columns;
	int column = index % columns;
	uint16_t stabilizer = rowClasses.stabilizers[classIndex];

	for (int symmetry = 0; symmetry < 16; symmetry++) {
		if (!(stabilizer >> symmetry & 1))
			continue;
		int64_t symmetricIndex = transformColumn(column, symmetry) + (int64_t)columns * classIndex;
		if (distances[symmetricIndex] == EMPTY) {
			distances[symmetricIndex] = distance;
			filled++;
		}
	}
}

// Estimates the number of neighbour lookups a backward pass would need by trying it on a sample of the empty entries
int64_t PruningTable::estimateBackwardLookups(int depth) {
	mt19937 rng(depth);
	int64_t size = distances.size(), sampled = 0, lookups = 0;
	for (int tries = 0; sampled < 1000 && tries < 1000000; tries++) {
		int64_t index = rng() % size;
		if (distances[index] != EMPTY)
			continue;
		sampled++;
		const int* rowMoves = &rowMoveTable[(int64_t)rowClasses.representatives[index / columns] * moveCount];
		const int* columnMoves = &columnMoveTable[(int64_t)(index % columns) * moveCount];
		for (int move = 0; move < moveCount; move++) {
			lookups++;
			if (distances[entryIndex(rowMoves[move], columnMoves[move])] == depth)
				break;
		}
	}
	return sampled == 0 ? 0 : lookups * (size - filled) / sampled;
}

// Fills the table one depth at a time. A forward pass expands every entry at the current depth, a backward
// pass lets every still empty entry look for a neighbour at the current depth. Forward is cheaper while the
// table is mostly empty and backward once it is mostly full, so each pass uses whichever needs fewer lookups.
void PruningTable::generate(int goalRow, int goalColumn, int maxDepth) {
	int classes = rowClasses.representatives.size();
	int64_t size = (int64_t)classes * columns;
	distances.assign(size, EMPTY);
	filled = 0;
	setDistance(entryIndex(goalRow, goalColumn), 0);

	auto totalStart = chrono::steady_clock::now();
	int64_t layerSize = filled;
	cout << name << ": " << size << " entries\n";
	cout << "  depth 0: " << layerSize << "\n";

	for (int depth = 0; depth < maxDepth && filled < size && layerSize > 0; depth++) {
		auto start = chrono::steady_clock::now();
		int64_t filledBefore = filled;
		bool backward = estimateBackwardLookups(depth) < layerSize * moveCount;

		for (int classIndex = 0; classIndex < classes; classIndex++) {
			const int* rowMoves = &rowMoveTable[(int64_t)rowClasses.representatives[classIndex] * moveCount];

			for (int column = 0; column < columns; column++) {
				int64_t index = column + (int64_t)columns * classIndex;
				if (backward ? distances[index] != EMPTY : distances[index] != depth)
					continue;

				const int* columnMoves = &columnMoveTable[(int64_t)column * moveCount];
				for (int move = 0; move < moveCount; move++) {
					int64_t neighbour = entryIndex(rowMoves[move], columnMoves[move]);
					if (backward) {
						if (distances[neighbour] == depth) {
							setDistance(index, depth + 1);
							break;
						}
					}
					else if (distances[neighbour] == EMPTY)
						setDistance(neighbour, depth + 1);
				}
			}
		}

		layerSize = filled - filledBefore;
		double seconds = chrono::duration<double>(chrono::steady_clock::now() - start).count();
		cout << "  depth " << depth + 1 << ": " << layerSize << " (" << (backward ? "backward" : "forward") << ", " << seconds << " s)\n";
	}

	double totalSeconds = chrono::duration<double>(chrono::steady_clock::now() - totalStart).count();
	if (filled < size)
		cout << "  " << size - filled << " entries were not reached\n";
	cout << "  finished in " << totalSeconds << " s\n";
}

void PruningTable::writeFile() {
	string data(distances.size(), 'a');
	for (int64_t i = 0; i < (int64_t)distances.size(); i++)
		data[i] = 'a' + distances[i];

	ofstream output("resources/data/" + name, ios::binary);
	output.write(data.data(), data.size());
	cout << "Wrote resources/data/" << name << "\n";
}
