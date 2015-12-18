#pragma once
#include "Tools.h"
#include "SheetSprite.h"
#include "Entity.h"
#include "Particle.h"
#include "GameState.h"

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

#define TILE_SIZE 16


class Entity;
class ParticleEmitter;
class GameState;

class GameApp{
	friend class SheetSprite;
	friend class Entity;
	friend class GameState;
	friend class MapState;
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
	Mix_Music* music;
	Mix_Chunk* sound;
	Matrix projectionMatrix;
	Matrix modelMatrix;
	Matrix viewMatrix;

	std::map<std::string, GLuint> textures;
	std::map<std::string, GameState*> gameStates;
	GameState* currentState;
	ShaderProgram *shader;
	Entity* grid;
};



