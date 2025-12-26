#pragma once
#include "Camera.h"
#include "SDL3/SDL.h"

extern int windowWidth;
extern int windowHeight;

extern Camera camera;
extern SDL_Window* window;

extern unsigned int VAO, VBO;
extern bool running;