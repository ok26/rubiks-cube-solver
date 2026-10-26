#include "graphics/cube.h"
#include "graphics/helper_functions.h"

#include <glad/glad.h>

#include <glm/gtc/matrix_transform.hpp>

#include <string>

#define YELLOW	1.00f, 	0.84f, 	0.00f
#define BLUE	0.00f, 	0.27f, 	0.78f
#define RED		0.72f, 	0.07f, 	0.10f
#define GREEN	0.00f, 	0.61f, 	0.12f
#define ORANGE	1.00f, 	0.35f, 	0.00f
#define WHITE	1.00f, 	1.00f, 	1.00f
#define BLACK	0.00f, 	0.00f, 	0.00f

static unsigned int corner, edgeU, edgeR, edgeL, edgeD, center, blank;

Cubelet::Cubelet(std::vector<glm::vec3> colors, glm::vec3 position, std::array<int, 6> cubeType) {
	this->colors = colors;
	this->position = position;
	this->cubeType = cubeType;
	this->model = glm::translate(this->model, position);
}

void Cubelet::DrawFace(int face, float diffuseMultiplier, float ambientMultiplier, Shader &shader) {
	if (cubeType[face] == CORNER)
		glBindTexture(GL_TEXTURE_2D, corner);
	else if (cubeType[face] == CENTER)
		glBindTexture(GL_TEXTURE_2D, center);
	else if (cubeType[face] == EDGEU)
		glBindTexture(GL_TEXTURE_2D, edgeU);
	else if (cubeType[face] == EDGED)
		glBindTexture(GL_TEXTURE_2D, edgeD);
	else if (cubeType[face] == EDGER)
		glBindTexture(GL_TEXTURE_2D, edgeR);
	else if (cubeType[face] == EDGEL)
		glBindTexture(GL_TEXTURE_2D, edgeL);
	else if (cubeType[face] == NONE)
		glBindTexture(GL_TEXTURE_2D, blank);

	shader.setVec3("material.ambient", colors[face] * ambientMultiplier);
	shader.setVec3("material.diffuse", colors[face] * diffuseMultiplier);
	glDrawArrays(GL_TRIANGLES, 6 * face, 6);
}

void Cubelet::Draw(Shader &shader) {
	float diffuseMultiplier = 2.0f;
	float ambientMultiplier = 1.0f * diffuseMultiplier;

	shader.use();
	shader.setMat4("model", model);

	for (int i = 0; i < 6; i++)
		DrawFace(i, diffuseMultiplier, ambientMultiplier, shader);
}

Cube::Cube(glm::vec3 position) {
	this->position = position;
	initCube();
	LoadVertices();
}

void Cube::initCube() {
	for (int x = 0; x < 3; x++)
		for (int y = 0; y < 3; y++)
			for (int z = 0; z < 3; z++)
				cubeletIndices[x][y][z] = x * 9 + y * 3 + z - (x * 9 + y * 3 + z > 12);


	cubelets.push_back(new Cubelet({ {BLACK}, {BLACK}, {BLACK}, {BLUE}, {ORANGE}, {WHITE} }, glm::vec3(-2.1f, -2.1f, -2.1f), { NONE, NONE, NONE, CORNER, CORNER, CORNER }));
	cubelets.push_back(new Cubelet({ {BLACK}, {BLACK}, {BLACK}, {BLUE}, {BLACK}, {WHITE} }, glm::vec3(-2.1f, -2.1f, 0.0f), { NONE, NONE, NONE, EDGEL, NONE, EDGEL }));
	cubelets.push_back(new Cubelet({ {BLACK}, {BLACK}, {RED}, {BLUE}, {BLACK}, {WHITE} }, glm::vec3(-2.1f, -2.1f, 2.1f), { NONE, NONE, CORNER, CORNER, NONE, CORNER }));
	cubelets.push_back(new Cubelet({ {BLACK}, {BLACK}, {BLACK}, {BLUE}, {ORANGE}, {BLACK} }, glm::vec3(-2.1f, 0.0f, -2.1f), { NONE, NONE, NONE, EDGED, EDGEL, NONE }));
	cubelets.push_back(new Cubelet({ {BLACK}, {BLACK}, {BLACK}, {BLUE}, {BLACK}, {BLACK} }, glm::vec3(-2.1f, 0.0f, 0.0f), { NONE, NONE, NONE, CENTER, NONE, NONE }));
	cubelets.push_back(new Cubelet({ {BLACK}, {BLACK}, {RED}, {BLUE}, {BLACK}, {BLACK} }, glm::vec3(-2.1f, 0.0f, 2.1f), { NONE, NONE, EDGEL, EDGEU, NONE, NONE }));
	cubelets.push_back(new Cubelet({ {YELLOW}, {BLACK}, {BLACK}, {BLUE}, {ORANGE}, {BLACK} }, glm::vec3(-2.1f, 2.1f, -2.1f), { CORNER, NONE, NONE, CORNER, CORNER, NONE }));
	cubelets.push_back(new Cubelet({ {YELLOW}, {BLACK}, {BLACK}, {BLUE}, {BLACK}, {BLACK} }, glm::vec3(-2.1f, 2.1f, 0.0f), { EDGEL, NONE, NONE, EDGER, NONE, NONE }));
	cubelets.push_back(new Cubelet({ {YELLOW}, {BLACK}, {RED}, {BLUE}, {BLACK}, {BLACK} }, glm::vec3(-2.1f, 2.1f, 2.1f), { CORNER, NONE, CORNER, CORNER, NONE, NONE }));
	cubelets.push_back(new Cubelet({ {BLACK}, {BLACK}, {BLACK}, {BLACK}, {ORANGE}, {WHITE} }, glm::vec3(0.0f, -2.1f, -2.1f), { NONE, NONE, NONE, NONE, EDGEU, EDGED }));
	cubelets.push_back(new Cubelet({ {BLACK}, {BLACK}, {BLACK}, {BLACK}, {BLACK}, {WHITE} }, glm::vec3(0.0f, -2.1f, 0.0f), { NONE, NONE, NONE, NONE, NONE, CENTER })); // WHITE
	cubelets.push_back(new Cubelet({ {BLACK}, {BLACK}, {RED}, {BLACK}, {BLACK}, {WHITE} }, glm::vec3(0.0f, -2.1f, 2.1f), { NONE, NONE, EDGEU, NONE, NONE, EDGEU }));
	cubelets.push_back(new Cubelet({ {BLACK}, {BLACK}, {BLACK}, {BLACK}, {ORANGE}, {BLACK} }, glm::vec3(0.0f, 0.0f, -2.1f), { NONE, NONE, NONE, NONE, CENTER, NONE })); // ORANGE
	cubelets.push_back(new Cubelet({ {BLACK}, {BLACK}, {RED}, {BLACK}, {BLACK}, {BLACK} }, glm::vec3(0.0f, 0.0f, 2.1f), { NONE, NONE, CENTER, NONE, NONE, NONE })); // RED
	cubelets.push_back(new Cubelet({ {YELLOW}, {BLACK}, {BLACK}, {BLACK}, {ORANGE}, {BLACK} }, glm::vec3(0.0f, 2.1f, -2.1f), { EDGED, NONE, NONE, NONE, EDGED, NONE }));
	cubelets.push_back(new Cubelet({ {YELLOW}, {BLACK}, {BLACK}, {BLACK}, {BLACK}, {BLACK} }, glm::vec3(0.0f, 2.1f, 0.0f), { CENTER, NONE, NONE, NONE, NONE, NONE })); // YELLOW
	cubelets.push_back(new Cubelet({ {YELLOW}, {BLACK}, {RED}, {BLACK}, {BLACK}, {BLACK} }, glm::vec3(0.0f, 2.1f, 2.1f), { EDGEU, NONE, EDGED, NONE, NONE, NONE }));
	cubelets.push_back(new Cubelet({ {BLACK}, {GREEN}, {BLACK}, {BLACK}, {ORANGE}, {WHITE} }, glm::vec3(2.1f, -2.1f, -2.1f), { NONE, CORNER, NONE, NONE, CORNER, CORNER }));
	cubelets.push_back(new Cubelet({ {BLACK}, {GREEN}, {BLACK}, {BLACK}, {BLACK}, {WHITE} }, glm::vec3(2.1f, -2.1f, 0.0f), { NONE, EDGEL, NONE, NONE, NONE, EDGER }));
	cubelets.push_back(new Cubelet({ {BLACK}, {GREEN}, {RED}, {BLACK}, {BLACK}, {WHITE} }, glm::vec3(2.1f, -2.1f, 2.1f), { NONE, CORNER, CORNER, NONE, NONE, CORNER }));
	cubelets.push_back(new Cubelet({ {BLACK}, {GREEN}, {BLACK}, {BLACK}, {ORANGE}, {BLACK} }, glm::vec3(2.1f, 0.0f, -2.1f), { NONE, EDGED, NONE, NONE, EDGER, NONE }));
	cubelets.push_back(new Cubelet({ {BLACK}, {GREEN}, {BLACK}, {BLACK}, {BLACK}, {BLACK} }, glm::vec3(2.1f, 0.0f, 0.0f), { NONE, CENTER, NONE, NONE, NONE, NONE })); // GREEN
	cubelets.push_back(new Cubelet({ {BLACK}, {GREEN}, {RED}, {BLACK}, {BLACK}, {BLACK} }, glm::vec3(2.1f, 0.0f, 2.1f), { NONE, EDGEU, EDGER, NONE, NONE, NONE }));
	cubelets.push_back(new Cubelet({ {YELLOW}, {GREEN}, {BLACK}, {BLACK}, {ORANGE}, {BLACK} }, glm::vec3(2.1f, 2.1f, -2.1f), { CORNER, CORNER, NONE, NONE, CORNER, NONE }));
	cubelets.push_back(new Cubelet({ {YELLOW}, {GREEN}, {BLACK}, {BLACK}, {BLACK}, {BLACK} }, glm::vec3(2.1f, 2.1f, 0.0f), { EDGER, EDGER, NONE, NONE, NONE }));
	cubelets.push_back(new Cubelet({ {YELLOW}, {GREEN}, {RED}, {BLACK}, {BLACK}, {BLACK} }, glm::vec3(2.1f, 2.1f, 2.1f), { CORNER, CORNER, CORNER, NONE, NONE, NONE }));
}

void Cube::LoadVertices() {
	std::vector<float> vertices = LoadVertexData("resources/vertexData/cube.txt");

	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);

	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
}

void Cube::LoadTextures(Shader &shader) {
	corner = loadTexture("resources/textures/stickerCorner.png");
	edgeU = loadTexture("resources/textures/stickerEdgeU.png");
	edgeR = loadTexture("resources/textures/stickerEdgeR.png");
	edgeL = loadTexture("resources/textures/stickerEdgeL.png");
	edgeD = loadTexture("resources/textures/stickerEdgeD.png");
	center = loadTexture("resources/textures/stickerCenter.png");
	blank = loadTexture("resources/textures/stickerBlank.png");

	shader.use();
	shader.setInt("material.diffuseMap", 0);
}

void Cube::Draw(Shader &shader, Animation &animation) {
	glBindVertexArray(cubeVAO);
	glActiveTexture(GL_TEXTURE0);

	if (animation.running)
		setUpMoveAxis(animation.targetSide);

	for (int x = 0; x < 3; x++)
		for (int y = 0; y < 3; y++)
			for (int z = 0; z < 3; z++) {
				if (x == y && y == z && z == 1)
					continue;

				if (animation.running && (moveX == x || moveY == y || moveZ == z || all != -1)) {
					glm::vec3 axisRot = glm::vec3(0.0f);
					axisRot[ animation.axis ] = 1.0f;
					float angleRot = 90.0f * animation.animationStep -cubelets[cubeletIndices[x][y][z]]->lastRotationAngle;
					cubelets[cubeletIndices[x][y][z]]->lastRotationAngle += angleRot;
					if (animation.inverseMove)
						angleRot *= -1;

					glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(angleRot), axisRot);
					cubelets[cubeletIndices[x][y][z]]->model = rotationMatrix * cubelets[cubeletIndices[x][y][z]]->model;
				}
				cubelets[cubeletIndices[x][y][z]]->Draw(shader);
			}
}

void Cube::setUpMoveAxis(int targetFace) {
	switch (targetFace) {
		case 0:
			moveX = moveZ = all = -1;
			moveY = 2;
			break;
		case 1:
			moveX = moveY = all = -1;
			moveZ = 2;
			break;
		case 2:
			moveY = moveZ = all = -1;
			moveX = 2;
			break;
		case 3:
			moveY = moveZ = all = -1;
			moveX = 0;
			break;
		case 4:
			moveX = moveY = all = -1;
			moveZ = 0;
			break;
		case 5:
			moveX = moveZ = all = -1;
			moveY = 0;
			break;
		case 6:
			moveX = moveZ = moveY = -1;
			all = 1;
	}
}

void Cube::RotateCubelet(float angle, int axis, bool inverse, Cubelet* cubelet) {
	glm::vec3 axisRot = glm::vec3(0.0f);
	axisRot[axis] = 1.0f;
	float angleRot = angle - cubelet->lastRotationAngle;
	cubelet->lastRotationAngle += angleRot;
	if (inverse)
		angleRot *= -1;

	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(angleRot), axisRot);
	cubelet->model = rotationMatrix * cubelet->model;
}

void Cube::FinishMove(Animation& animation) {
	setUpMoveAxis(animation.targetSide);

	glm::vec3 axisRot = glm::vec3(0.0f);
	axisRot[animation.axis] = 1.0f;
	float angleRot = 90.0f;
	if (animation.inverseMove)
		angleRot *= -1;

	std::vector<std::vector<int>> newIndices;

	for (int x = 0; x < 3; x++)
		for (int y = 0; y < 3; y++)
			for (int z = 0; z < 3; z++)
				if (moveX == x || moveY == y || moveZ == z || all != -1 && !(x == y && y == z && z == 1)) {
					int index = cubeletIndices[x][y][z];
					RotateCubelet(90.0f, animation.axis, animation.inverseMove, cubelets[index]);
					cubelets[index]->lastRotationAngle = 0.0f;

					glm::vec3 newPos = glm::vec3(cubelets[index]->model[3]);
					int indices[3] = {0};
					for (int i = 0; i < 3; i++)
						if (floatEquals(newPos[i], -2.1f, 0.05f))
							indices[i] = 0;
						else if (floatEquals(newPos[i], 0.0f, 0.05f))
							indices[i] = 1;
						else if (floatEquals(newPos[i], 2.1f, 0.05f))
							indices[i] = 2;

					if (indices[0] == x && indices[1] == y && indices[2] == z)
						continue;

					newIndices.push_back({ indices[0], indices[1], indices[2], cubeletIndices[x][y][z] });
				}

	for (int i = 0; i < newIndices.size(); i++)
		cubeletIndices[newIndices[i][0]][newIndices[i][1]][newIndices[i][2]] = newIndices[i][3];
}
