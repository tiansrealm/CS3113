#pragma once
#include "GameApp.h"
class GameApp;
class SheetSprite{
public:
	SheetSprite();
	SheetSprite(unsigned int textureID, float u, float v, float width, float height, float size);

	void draw(GameApp* app);

	unsigned int textureID;
	float u;
	float v;
	float width;
	float height;
	float size;
};

class Entity {
	friend class GameApp;
public:
	Entity(GameApp *app);
	Entity(GameApp * app, SheetSprite& sprite, float x, float y);
	//~Entity();
	void draw();
	void update(float elapsed);
private:
	GameApp* app;
	SheetSprite sprite;
	float x, y;
	float velocity_x, velocity_y;
	float accel_x, accel_y;
	float rotation;
	int textureID;
	float width;
	float height;
	float speed;
	Matrix transMatrix;
};