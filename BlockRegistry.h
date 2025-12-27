#pragma once
#include "Block.h"
#include <unordered_map>

class BlockRegistry {
public:
	static void Init();
	static const Block& Get(uint8_t id);

private:
	static std::unordered_map<uint8_t, Block> blocks;
};