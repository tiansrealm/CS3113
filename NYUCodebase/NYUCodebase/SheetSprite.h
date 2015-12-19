#pragma once
#include "ShaderProgram.h"


class SheetSprite{
public:
	SheetSprite();
	SheetSprite(unsigned int textureID, float u, float v, float u_w, float v_h, float w, float h);
	SheetSprite(unsigned int textureID, int row, int col, float w, float h,
		float totalWidth, float totalHeight, float subWidth, float subHeight);
	void draw(ShaderProgram *shader);
	unsigned int textureID;
	float u;
	float v;
	float u_width;
	float v_height;
	float width;
	float height;
};

