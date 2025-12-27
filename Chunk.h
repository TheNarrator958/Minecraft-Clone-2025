#pragma once
#include <vector>
#include "glm.hpp"

class Chunk {
public:
	static constexpr int SIZE = 16;

	void GenerateMesh();
	void Draw();

private:
	uint8_t blocks[SIZE][SIZE][SIZE];

	unsigned int VAO, VBO;
	std::vector<float> vertices;
};