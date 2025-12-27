#include "BlockRegistry.h"

std::unordered_map<uint8_t, Block> BlockRegistry::blocks;

void BlockRegistry::Init() {
	// Air
	blocks[0] = Block{
		0,
		false,
		{0, 0, 0, 0, 0, 0}
	};

	// Grass Block
	blocks[1] = Block{
		1,
		true,
		{2, 2, 1, 1, 3, 0} // front, back, left, right, top, bottom
	}; 

	// Dirt
	blocks[2] = Block{
		2,
		true,
		{0, 0, 0, 0, 0, 0}
	};

	// Stone
	blocks[3] = Block{
		3,
		true,
		{3, 3, 3, 3, 3, 3}
	};

	// Oak Planks
	blocks[4] = Block{
		4,
		true,
		{4, 4, 4, 4, 4, 4}
	};
}

const Block& BlockRegistry::Get(uint8_t id) {
	return blocks.at(id);
}