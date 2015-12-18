#pragma once

#include "Tools.h"
#include "Matrix.h"
#include "SheetSprite.h"
#include "Entity.h"
#include "Particle.h"

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

#define TILE_SIZE 16


class Entity;
class ParticleEmitter;

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

	bool readHeader(std::ifstream &stream);
	bool readLayerData(std::ifstream &stream);
	bool readEntityData(std::ifstream &stream);
	void loadMap();
	void placeEntity(std::string& type, float x, float y);
private:
	int screenWidth, screenHeight;
	int mapWidth, mapHeight;
	unsigned char** mapData;
	bool done;
	float lastFrameTicks;
	SDL_Window*	displayWindow;
	Mix_Music* music;
	Mix_Chunk* sound;
	Matrix projectionMatrix;
	Matrix modelMatrix;
	Matrix viewMatrix;
	std::map<std::string, GLuint> textures;
	ShaderProgram *shader;
	std::vector<Entity*> entities;
	std::vector<Entity*> staticEntities;
	ParticleEmitter* emitter;
	Entity* grid;
};



