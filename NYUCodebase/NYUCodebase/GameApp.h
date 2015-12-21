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
	bool done;
	int orthoHeight, orthoWidth;
	float lastFrameTicks;
	float aniTime;
	SDL_Window*	displayWindow;

	Matrix projectionMatrix;
	Matrix modelMatrix;
	Matrix viewMatrix;

	std::map<std::string, GLuint> textures;
	std::map<std::string, SheetSprite*> sprites;
	std::map<std::string, GameState*> gameStates;
	std::map<std::string, Mix_Chunk*> sounds;
	std::map<std::string, Mix_Music*> musics;
	GameState* currentState;
	ShaderProgram *shader;
	Entity* grid;
	Entity* cursor;
	ParticleEmitter* starEmitter;
	ParticleEmitter* fireballEmitter;
	void displayText(const std::string s, float x, float y, float w, float h, float spacing);
	void loadSprites();
	void loadStates();
	
};



