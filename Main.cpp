#include "Main.h"
#include <iostream>
#include "glad/glad.h"
#include "glm.hpp"
#include "gtc/type_ptr.hpp"
#include "Shader.h"
#include "SDL3_image/SDL_image.h"
#include "TextureAtlas.h"

int windowWidth = 1920;
int windowHeight = 1080;

Camera camera;
SDL_Window* window;

unsigned int VAO = 0;
unsigned int VBO = 0;

bool running = true;

float cubeVertices[] = {
    // Front
    -0.5f,-0.5f, 0.5f,  0,0,
     0.5f,-0.5f, 0.5f,  1,0,
     0.5f, 0.5f, 0.5f,  1,1,
    -0.5f,-0.5f, 0.5f,  0,0,
     0.5f, 0.5f, 0.5f,  1,1,
    -0.5f, 0.5f, 0.5f,  0,1,

    // Back
     0.5f,-0.5f,-0.5f,  0,0,
    -0.5f,-0.5f,-0.5f,  1,0,
    -0.5f, 0.5f,-0.5f,  1,1,
     0.5f,-0.5f,-0.5f,  0,0,
    -0.5f, 0.5f,-0.5f,  1,1,
     0.5f, 0.5f,-0.5f,  0,1,

     // Left
     -0.5f,-0.5f,-0.5f,  0,0,
     -0.5f,-0.5f, 0.5f,  1,0,
     -0.5f, 0.5f, 0.5f,  1,1,
     -0.5f,-0.5f,-0.5f,  0,0,
     -0.5f, 0.5f, 0.5f,  1,1,
     -0.5f, 0.5f,-0.5f,  0,1,

     // Right
      0.5f,-0.5f, 0.5f,  0,0,
      0.5f,-0.5f,-0.5f,  1,0,
      0.5f, 0.5f,-0.5f,  1,1,
      0.5f,-0.5f, 0.5f,  0,0,
      0.5f, 0.5f,-0.5f,  1,1,
      0.5f, 0.5f, 0.5f,  0,1,

      // Top
      -0.5f, 0.5f, 0.5f,  0,0,
       0.5f, 0.5f, 0.5f,  1,0,
       0.5f, 0.5f,-0.5f,  1,1,
      -0.5f, 0.5f, 0.5f,  0,0,
       0.5f, 0.5f,-0.5f,  1,1,
      -0.5f, 0.5f,-0.5f,  0,1,

      // Bottom
      -0.5f,-0.5f,-0.5f,  0,0,
       0.5f,-0.5f,-0.5f,  1,0,
       0.5f,-0.5f, 0.5f,  1,1,
      -0.5f,-0.5f,-0.5f,  0,0,
       0.5f,-0.5f, 0.5f,  1,1,
      -0.5f,-0.5f, 0.5f,  0,1,
};

unsigned int cubeVAO;
unsigned int cubeVBO;

unsigned int LoadTexture(const char* path)
{
    SDL_Surface* surface = IMG_Load(path);

    // Convert to RGBA32 for OpenGL
    SDL_Surface* rgbaSurface = SDL_ConvertSurface(
        surface,
        SDL_PIXELFORMAT_RGBA32
    );
    SDL_DestroySurface(surface);

    unsigned int texture;
    glCreateTextures(GL_TEXTURE_2D, 1, &texture);

    glTextureStorage2D(
        texture,
        1,
        GL_RGBA8,
        rgbaSurface->w,
        rgbaSurface->h
    );

    glTextureSubImage2D(
        texture,
        0,
        0,
        0,
        rgbaSurface->w,
        rgbaSurface->h,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        rgbaSurface->pixels
    );

    // Minecraft-style filtering
    glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_REPEAT);

    SDL_DestroySurface(rgbaSurface);

    return texture;
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_GLContext glContext;

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    window = SDL_CreateWindow("Minecraft Clone", windowWidth, windowHeight, SDL_WINDOW_OPENGL);

    glContext = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, glContext);
    //SDL_GL_SetSwapInterval(1); // Enable VSync

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    int w, h;
    SDL_GetWindowSize(window, &w, &h);
    glViewport(0, 0, w, h);

    Shader shader("shaders/basic.vert", "shaders/basic.frag");

    glEnable(GL_DEPTH_TEST);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // cube setup
    glCreateVertexArrays(1, &cubeVAO);
	glCreateBuffers(1, &cubeVBO);

    // Vertex Buffer
    glNamedBufferData(
        cubeVBO,
        sizeof(cubeVertices),
        cubeVertices,
        GL_STATIC_DRAW
	);

    // Attach Buffers
    glVertexArrayVertexBuffer(
        cubeVAO,
        0,
        cubeVBO,
        0,
        5 * sizeof(float)
    );

    /*unsigned int dirtBlockTexture = LoadTexture("textures/dirt_block.png");
    unsigned int grassSideBlockTexture = LoadTexture("textures/grass_block_side.png");
    unsigned int grassTopBlockTexture = LoadTexture("textures/grass_block_top.png");

    constexpr int STRIDE = 5 * sizeof(float);*/

    // Attribute 0 -> Position
    glEnableVertexArrayAttrib(cubeVAO, 0);
    glVertexArrayAttribFormat(cubeVAO, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(cubeVAO, 0, 0);

	// Attribute 1 -> UV
    glEnableVertexArrayAttrib(cubeVAO, 1);
    glVertexArrayAttribFormat(cubeVAO, 1, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float));
    glVertexArrayAttribBinding(cubeVAO, 1, 0);

    // end of cube setup

    // texture atlas setup
    TextureAtlas atlas = BuildAtlas({
    "textures/dirt_block.png",
    "textures/grass_block_side.png",
    "textures/grass_block_top.png",
    "textures/stone.png",
    "textures/oak_planks.png"
        });
	// end of texture atlas setup
    AtlasEntry grassTop = atlas.entries["textures/grass_block_top.png"];
    AtlasEntry grassSide = atlas.entries["textures/grass_block_side.png"];
    AtlasEntry dirt = atlas.entries["textures/dirt_block.png"];

    SDL_ShowWindow(window);

    SDL_SetWindowRelativeMouseMode(window, true);

    // GAME LOOP
    double lastTime = SDL_GetTicks() / 1000.0;
    bool running = true;
    SDL_Event event;

    while (running) {
        double currentTime = SDL_GetTicks() / 1000.0;
        double deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) running = false;

            if (event.type == SDL_EVENT_MOUSE_MOTION) {
                camera.ProcessMouseMovement(
                    (float)event.motion.xrel,
                    (float)-event.motion.yrel // inverted Y for FPS feel
                );
            }
        }

        const bool* keys = SDL_GetKeyboardState(NULL);

        camera.ProcessKeyboard(
            (float)deltaTime,
            keys[SDL_SCANCODE_W],
            keys[SDL_SCANCODE_S],
            keys[SDL_SCANCODE_A],
            keys[SDL_SCANCODE_D]
        );

        // drawing
        glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.Use();

        glBindTextureUnit(0, atlas.textureID);
        shader.SetInt("uTexture", 0);

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(
            glm::radians(camera.FOV),
            (float)windowWidth / (float)windowHeight,
            0.1f,
            100.0f
        );

        shader.SetMat4("model", model);
        shader.SetMat4("view", view);
        shader.SetMat4("projection", projection);

        glBindVertexArray(cubeVAO); 
        glDrawArrays(GL_TRIANGLES, 0, 36);

        SDL_GL_SwapWindow(window);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}