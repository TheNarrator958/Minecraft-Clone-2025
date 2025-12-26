#include "TextureAtlas.h"
#include <iostream>

#include "glad/glad.h"
#include "SDL3_image/SDL_image.h"

TextureAtlas BuildAtlas(const std::vector<std::string>& files)
{
    TextureAtlas atlas;
    atlas.tileSize = 16;

    const int tilesPerRow = (int)files.size();
    atlas.atlasWidth = tilesPerRow * atlas.tileSize;
    atlas.atlasHeight = atlas.tileSize;

    // Create pixel buffer
    std::vector<unsigned char> pixels(
        atlas.atlasWidth * atlas.atlasHeight * 4,
        0
    );

    for (int i = 0; i < files.size(); i++)
    {
        SDL_Surface* surface = IMG_Load(files[i].c_str());
        if (!surface) {
            std::cerr << "Failed to load: " << files[i] << "\n";
            continue;
        }

        SDL_Surface* rgba = SDL_ConvertSurface(surface, SDL_PIXELFORMAT_RGBA32);
        SDL_DestroySurface(surface);

        int xOffset = i * atlas.tileSize;

        // Copy pixels into atlas buffer
        for (int y = 0; y < atlas.tileSize; y++) {
            for (int x = 0; x < atlas.tileSize; x++) {
                int src = (y * rgba->w + x) * 4;
                int dst = ((y * atlas.atlasWidth + (x + xOffset)) * 4);

                memcpy(&pixels[dst], &((unsigned char*)rgba->pixels)[src], 4);
            }
        }

        // Calculate UVs
        float u0 = (float)xOffset / atlas.atlasWidth;
        float u1 = (float)(xOffset + atlas.tileSize) / atlas.atlasWidth;
        float v0 = 0.0f;
        float v1 = 1.0f;

        atlas.entries[files[i]] = { u0, v0, u1, v1 };

        SDL_DestroySurface(rgba);
    }

    // Create OpenGL texture
    glCreateTextures(GL_TEXTURE_2D, 1, &atlas.textureID);
    glTextureStorage2D(
        atlas.textureID,
        1,
        GL_RGBA8,
        atlas.atlasWidth,
        atlas.atlasHeight
    );

    glTextureSubImage2D(
        atlas.textureID,
        0,
        0,
        0,
        atlas.atlasWidth,
        atlas.atlasHeight,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        pixels.data()
    );

    glTextureParameteri(atlas.textureID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(atlas.textureID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameteri(atlas.textureID, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(atlas.textureID, GL_TEXTURE_WRAP_T, GL_REPEAT);

    return atlas;
}