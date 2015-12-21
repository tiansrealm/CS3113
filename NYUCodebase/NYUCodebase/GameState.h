#pragma once
#include "GameState.h"
#include "GameApp.h"

#define TILE_SIZE 32
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define ORTHO_WIDTH SCREEN_WIDTH
#define ORTHO_HEIGHT SCREEN_HEIGHT
struct TextData{
	TextData(std::string s, float x = 0.0f, float y = 0.0f, float w = 0.0f, float h = 0.0f, float spacing = 0.0f) :
	text(s), x(x), y(y), font_w(w), font_h(h), spacing(spacing){}
	std::string text;
	float x;
	float y;
	float font_w;
	float font_h;
	float spacing;
};

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
	std::vector<TextData> textDatas;
	std::vector<Entity*> entities;
	std::vector<Entity*> staticEntities;

	std::map<std::string, Entity*> ghostEntities;
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