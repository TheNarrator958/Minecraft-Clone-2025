#pragma once
#include <string>
#include <unordered_map>
#include <vector>

struct AtlasEntry {
    float u0, v0;
    float u1, v1;
};

struct TextureAtlas {
    unsigned int textureID = 0;
    int atlasWidth = 0;
    int atlasHeight = 0;
    int tileSize = 16;

    std::unordered_map<std::string, AtlasEntry> entries;
};

TextureAtlas BuildAtlas(const std::vector<std::string>& files);