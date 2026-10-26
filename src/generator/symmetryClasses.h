#ifndef SYMMETRY_CLASSES_H
#define SYMMETRY_CLASSES_H

#include <cstdint>
#include <functional>
#include <string>
#include <vector>

class SymmetryClasses {
public:
	std::vector<int> classOf;
	std::vector<uint8_t> symmetryOf;
	std::vector<int> representatives;
	std::vector<uint16_t> stabilizers;

	void generate(int size, std::function<int(int, int)> transform);
	void generateWithoutSymmetry(int size);
	void writeFile(std::string fileName, int digits);
};

#endif
