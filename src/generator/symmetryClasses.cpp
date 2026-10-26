#include "generator/symmetryClasses.h"

#include <fstream>

using namespace std;

// Splits the coordinates 0..size-1 into classes of coordinates that are symmetric to each other.
// The smallest coordinate in a class is its representative, and for every coordinate x:
//   transform(representatives[classOf[x]], symmetryOf[x]) == x
// Bit s of stabilizers[c] is set when symmetry s maps the representative of class c to itself,
// which is the case for at least the identity and for more symmetries when the class has fewer than 16 members.
void SymmetryClasses::generate(int size, function<int(int, int)> transform) {
	classOf.assign(size, -1);
	symmetryOf.assign(size, 0);
	representatives.clear();
	stabilizers.clear();

	for (int x = 0; x < size; x++) {
		if (classOf[x] != -1)
			continue;

		int classIndex = representatives.size();
		uint16_t stabilizer = 0;
		for (int symmetry = 0; symmetry < 16; symmetry++) {
			int y = transform(x, symmetry);
			if (y == x)
				stabilizer |= 1 << symmetry;
			if (classOf[y] == -1) {
				classOf[y] = classIndex;
				symmetryOf[y] = symmetry;
			}
		}
		representatives.push_back(x);
		stabilizers.push_back(stabilizer);
	}
}

void SymmetryClasses::generateWithoutSymmetry(int size) {
	classOf.resize(size);
	symmetryOf.assign(size, 0);
	representatives.resize(size);
	stabilizers.assign(size, 1);
	for (int x = 0; x < size; x++)
		classOf[x] = representatives[x] = x;
}

void SymmetryClasses::writeFile(string fileName, int digits) {
	string data;
	data.reserve(classOf.size() * (digits + 1));
	for (int x = 0; x < classOf.size(); x++) {
		int classIndex = classOf[x];
		for (int i = 0; i < digits; i++) {
			data += (char)('a' + classIndex % 24);
			classIndex /= 24;
		}
		data += (char)('A' + symmetryOf[x]);
	}

	ofstream output(fileName, ios::binary);
	output.write(data.data(), data.size());
}
