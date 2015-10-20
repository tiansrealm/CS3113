#pragma once
#include "GameApp.h"
class GameApp;
class SheetSprite{
public:
	//SheetSprite();
	SheetSprite(unsigned int textureID, float u, float v, float width, float height, float size);

	void draw(GameApp* app, float x, float y);

	unsigned int textureID;
	float u;
	float v;
	float width;
	float height;
	float size;

};

class Entity {
public:
	Entity(GameApp *app);
	Entity(GameApp * app, SheetSprite* sprite);
	//~Entity();
	void draw();
	void update(float elapsed);
private:
	GameApp* app;
	SheetSprite* sprite;
	float x;
	float y;
	float rotation;
	int textureID;
	float width;
	float height;
	float speed;
	float direction_x;
	float direction_y;
};