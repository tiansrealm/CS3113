#pragma once
#include "Tools.h"
#include "SheetSprite.h"



enum EntityShape{RECTANGLE, CIRCLE};
class Entity {
	friend class GameApp;
	friend class GameState;
	friend class MapState;
public:
	Entity();
	Entity(ShaderProgram *shader, SheetSprite* sprite, float x = 0.0f, float y = 0.0f, bool is_static = true);
	//~Entity();
	void setWidthHeight(float w, float h);
	void setPos(float x, float y);
	void draw();
	void update(float elapsed);
	bool collidesWith(const Entity& other, bool applyShift = false);
	bool collidesWith(int x, int y);
	void move(float xShift, float yShift);
protected:
	ShaderProgram *shader;
	SheetSprite* sprite;
	Matrix matrix;  //stores the transformation for it's position in the world
	Vector pos;
	Vector vel;
	Vector accel;
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
	float hp;
	std::string name;
	float aniTime;
};


class circleEntity: public Entity{
	circleEntity(ShaderProgram *shader, SheetSprite* sprite, float x = 0.0f, float y = 0.0f);

private:
	float radius;
};