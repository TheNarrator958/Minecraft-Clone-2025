#pragma once
#include <cstdint>

enum class BlockFace : uint8_t {
	Front,
	Back,
	Left,
	Right,
	Top,
	Bottom
};

struct Block {
	uint8_t id;
	bool solid;

	// index into texture atlas per face
	uint16_t textures[6];
};