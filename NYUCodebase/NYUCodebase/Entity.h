#pragma once
#include "GameApp.h"
#include "string.h"
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
	Entity();
	Entity(GameApp * app, SheetSprite& sprite, float x = 0.0f, float y = 0.0f);
	//~Entity();
	void draw();
	void update(float elapsed);
	bool collidesWith(const Entity& other, bool applyShift = false);
	void move(float xShift, float yShift);
protected:
	GameApp* app;
	SheetSprite sprite;
	Matrix matrix;  //stores the transformation for it's position in the world
	float x, y;
	float velocity_x, velocity_y;
	float accel_x, accel_y;
	float rotation;
	int textureID;
	float width;
	float height;
	float speed;
	std::string shape;
};


class circleEntity :public Entity{
	circleEntity(GameApp * app, SheetSprite& sprite, float x = 0.0f, float y = 0.0f);

private:
	float radius;
};