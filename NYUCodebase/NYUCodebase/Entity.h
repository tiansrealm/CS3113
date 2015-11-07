#pragma once
#include "GameApp.h"
class GameApp;
class SheetSprite{
public:
	SheetSprite();
	SheetSprite(unsigned int textureID, float u, float v, float u_w, float v_h, float w, float h);

	void draw(GameApp* app);

	unsigned int textureID;
	float u;
	float v;
	float u_width;
	float v_height;
	float width;
	float height;
	float size;
};

enum EntityShape{RECTANGLE, CIRCLE};
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
	EntityShape shape;
	bool is_static;
	bool collideTop;
	bool collideBot;
	bool collideLeft;
	bool collideRight;
};


class circleEntity :public Entity{
	circleEntity(GameApp * app, SheetSprite& sprite, float x = 0.0f, float y = 0.0f);

private:
	float radius;
};