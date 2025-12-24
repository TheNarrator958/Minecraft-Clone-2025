#pragma once
#include "Camera.h"
#include "SDL3/SDL.h"

extern int windowWidth;
extern int windowHeight;

extern Camera camera;
extern SDL_Window* window;

extern unsigned int VAO, VBO;
extern bool running;

/*
#pragma once

// single variables
int windowWidth = 1920;
int windowHeight = 1080;

Camera camera;

SDL_Window* window;

// The Triangle

// Shaders
unsigned int VAO, VBO;

// Game Loop */#pragma once
