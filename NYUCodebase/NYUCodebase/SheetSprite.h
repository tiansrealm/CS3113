#pragma once
#include "ShaderProgram.h"


class SheetSprite{
public:
	SheetSprite();
	SheetSprite(unsigned int textureID, float u, float v, float u_w, float v_h, float w, float h);

	void draw(ShaderProgram *shader);

	unsigned int textureID;
	float u;
	float v;
	float u_width;
	float v_height;
	float width;
	float height;
	float size;
};