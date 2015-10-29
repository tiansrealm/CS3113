#pragma once
#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

#include "ShaderProgram.h"
#include <vector>
#include "Matrix.h"
#include "Entity.h"

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif
class Entity;
class GameApp{
	friend class SheetSprite;
	friend class Entity;
public:
	GameApp();
	~GameApp();
	void setup();
	void ProcessEvents();
	bool updateAndRender();
	void Render();
	void Update(float elapsed);
	Entity* player; 

private:
	int screenWidth, screenHeight;
	bool done;
	float lastFrameTicks;
	SDL_Window*	displayWindow;

	Matrix projectionMatrix;
	Matrix modelMatrix;
	Matrix viewMatrix;

	ShaderProgram *program;
	std::vector<Entity*> entities;
};



