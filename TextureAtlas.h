#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <cstdint>

struct AtlasEntry {
    float u0, v0;
    float u1, v1;
};

struct TextureAtlas {
    unsigned int textureID; // OpenGL texture
    int atlasWidth;
    int atlasHeight;

    std::unordered_map<std::string, AtlasEntry> entries;
};

// Builds an atlas and uploads it to OpenGL
TextureAtlas BuildAtlas(const std::vector<std::string>& imagePaths);