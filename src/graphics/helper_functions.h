#ifndef HELPER_H
#define HELPER_H

#include <vector>
#include <string>

std::vector<float> LoadVertexData(std::string filepath);
unsigned int loadTexture(char const* path);
bool floatEquals(float x, float y, float diff);

#endif
