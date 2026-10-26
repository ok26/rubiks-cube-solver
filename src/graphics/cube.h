#ifndef CUBE_H
#define CUBE_H

#include <glm/glm.hpp>

#include "graphics/shader.h"
#include "graphics/animation.h"

#include <vector>
#include <array>

enum cubeletFaces { CORNER, EDGEU, EDGED, EDGER, EDGEL, CENTER, NONE };

class Cubelet {
public:
	std::vector<glm::vec3> colors;
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 model = glm::mat4(1.0f);
	float lastRotationAngle = 0.0f;
	std::array<int, 6> cubeType = {};

	Cubelet(std::vector<glm::vec3> colors, glm::vec3 position, std::array<int, 6> cubeType);

	void DrawFace(int face, float diffuseMultiplier, float ambientMultiplier, Shader &shader);
	void Draw(Shader &shader);
};

class Cube {
public:
	int cubeletIndices[3][3][3];
	std::vector<Cubelet*> cubelets;
	glm::vec3 position;
	unsigned int cubeVAO, cubeVBO;
	int moveX, moveY, moveZ, all;

	Cube(glm::vec3 position);

	void initCube();
	void LoadVertices();
	void LoadTextures(Shader &shader);
	void Draw(Shader &shader, Animation &animation);
	void setUpMoveAxis(int targetFace);
	void RotateCubelet(float angle, int axis, bool inverse, Cubelet* cubelet);
	void FinishMove(Animation& animation);
};

#endif
