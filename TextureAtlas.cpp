#include "TextureAtlas.h"

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <glad/glad.h>
#include <iostream>

TextureAtlas BuildAtlas(const std::vector<std::string>& imagePaths)
{
    const int TILE_SIZE = 16;
    int tilesPerRow = (int)ceil(sqrt(imagePaths.size()));
    int atlasSize = tilesPerRow * TILE_SIZE;

    SDL_Surface* atlasSurface = SDL_CreateSurface(
        atlasSize,
        atlasSize,
        SDL_PIXELFORMAT_RGBA32
    );

    TextureAtlas atlas{};
    atlas.atlasWidth = atlasSize;
    atlas.atlasHeight = atlasSize;

    for (size_t i = 0; i < imagePaths.size(); i++)
    {
        SDL_Surface* tile = IMG_Load(imagePaths[i].c_str());
        if (!tile) {
            SDL_Log("Failed to load %s", imagePaths[i].c_str());
            continue;
        }

        int x = (i % tilesPerRow) * TILE_SIZE;
        int y = (i / tilesPerRow) * TILE_SIZE;

        SDL_Rect dst{ x, y, TILE_SIZE, TILE_SIZE };
        SDL_BlitSurface(tile, nullptr, atlasSurface, &dst);

        AtlasEntry entry;
        entry.u0 = (float)x / atlasSize;
        entry.v0 = (float)y / atlasSize;
        entry.u1 = (float)(x + TILE_SIZE) / atlasSize;
        entry.v1 = (float)(y + TILE_SIZE) / atlasSize;

        atlas.entries[imagePaths[i]] = entry;

        SDL_DestroySurface(tile);
    }

    glCreateTextures(GL_TEXTURE_2D, 1, &atlas.textureID);
    glTextureStorage2D(atlas.textureID, 1, GL_RGBA8, atlasSize, atlasSize);

    glTextureSubImage2D(
        atlas.textureID,
        0,
        0, 0,
        atlasSize,
        atlasSize,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        atlasSurface->pixels
    );

    glTextureParameteri(atlas.textureID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(atlas.textureID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    SDL_DestroySurface(atlasSurface);

    return atlas;
}