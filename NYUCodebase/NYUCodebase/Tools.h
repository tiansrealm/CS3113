#pragma once

#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "ShaderProgram.h"
#include <SDL_mixer.h>

#include "Matrix.h"
#include <vector>
#include <set>
#include <functional>
#include <string>
#include <map>
#include <fstream>
#include <iostream>
#include <sstream>


#define PI 3.1415926f

struct Xywh {
	Xywh(float x = 0.0f, float y = 0.0f, float w = 0.0f, float h = 0.0f);
	float x;
	float y;
	float w;
	float h;
};

struct Vector {
	Vector();
	Vector(float x, float y);
	float x;
	float y;
};
struct Color {
	Color();
	Color(float r, float g, float b, float a);
	float r;
	float g;
	float b;
	float a;
};

float lerp(float v0, float v1, float t);
bool circleCollison(float x1, float x2, float y1, float y2, float r1, float r2);

GLuint LoadTexture(const char *image_path);
GLuint LoadTextureAlpha(const char *image_path);
