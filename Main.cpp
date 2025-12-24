#include "Main.h"
#include <iostream>
#include "glad/glad.h"
#include "glm.hpp"
#include "gtc/type_ptr.hpp"
#include "Shader.h"

int windowWidth = 1920;
int windowHeight = 1080;

Camera camera;
SDL_Window* window;

unsigned int VAO = 0;
unsigned int VBO = 0;

bool running = true;

float triangleVertices[] = {
     -0.5f, -0.5f, 0.0f,
      0.5f, -0.5f, 0.0f,
      0.0f,  0.5f, 0.0f
};

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

    glCreateVertexArrays(1, &VAO);
    glCreateBuffers(1, &VBO);

    glNamedBufferData(
        VBO,
        sizeof(triangleVertices),
        triangleVertices,
        GL_STATIC_DRAW
    );

    // Bind VBO to binding point 0 of VAO
    glVertexArrayVertexBuffer(
        VAO,
        0,          // binding index
        VBO,
        0,
        3 * sizeof(float)
    );

    // Describe attribute 0
    glEnableVertexArrayAttrib(VAO, 0);

    glVertexArrayAttribFormat(
        VAO,
        0,          // attribute index
        3,
        GL_FLOAT,
        GL_FALSE,
        0
    );

    // Attach attribute 0 → binding 0
    glVertexArrayAttribBinding(
        VAO,
        0,
        0
    );

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

        glm::mat4 model = glm::translate(
            glm::mat4(1.0f),
            glm::vec3(0.0f, 0.0f, -2.0f)
        );
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(
            glm::radians(camera.FOV),
            (float)windowWidth / (float)windowHeight,
            0.1f,
            1000.0f
        );

        shader.SetMat4("model", model);
        shader.SetMat4("view", view);
        shader.SetMat4("projection", projection);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        SDL_GL_SwapWindow(window);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}