#pragma once
#include "GameState.h"
#include "GameApp.h"

#define TILE_SIZE 16
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

class GameState{
	friend class GameApp;
public:
	GameState(const std::string& name, GameApp *app);
	std::string name;
	void render();
	virtual void setView();
	void update(float elapsed);
	ShaderProgram *shader;
protected:
	GameApp* app;
	std::vector<Entity*> entities;
	std::vector<Entity*> staticEntities;
	std::vector<ParticleEmitter*> emitters;
};

class MapState: public GameState{

public:
	MapState(const  std::string& name, GameApp *app);
	void render();
private:
	int mapWidth, mapHeight;
	unsigned char** mapData;

	virtual void setView();
	void loadMap();
	bool readHeader(std::ifstream &stream);
	bool readLayerData(std::ifstream &stream);
	bool readEntityData(std::ifstream &stream);
	void placeEntity(std::string& type, float x, float y);
};