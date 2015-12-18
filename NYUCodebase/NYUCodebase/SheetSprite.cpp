
#include "SheetSprite.h"
SheetSprite::SheetSprite(){}

SheetSprite::SheetSprite(unsigned int textureID, float u, float v, float u_w, float v_h, float w, float h) :
textureID(textureID), u(u), v(v), u_width(u_w), v_height(v_h), width(w), height(h)
{
}
void SheetSprite::draw(ShaderProgram *shader){
	GLfloat texCoords[] = {
		u, v + v_height,
		u + u_width, v,
		u, v,
		u + u_width, v,
		u, v + v_height,
		u + u_width, v + v_height
	};
	//height = size;
	//width = size * width / height;
	//float aspect = width/height
	float vertices[] = {
		//top left corner = 0,0
		0.0f, -1.0f * height,
		1.0f * width, 0.0f,
		0.0f, 0.0 * height,
		1.0f * width, 0.0f,
		0.0f, -1.0f * height,
		1.0f * width, -1.0f * height

		//centered around 0,0
		/*
		-0.5f * size * aspect, -0.5f * size,
		0.5f * size * aspect, 0.5f * size,
		-0.5f * size * aspect, 0.5f * size,
		0.5f * size * aspect, 0.5f * size,
		-0.5f * size * aspect, -0.5f * size,
		0.5f * size * aspect, -0.5f * size
		*/
	};
	glVertexAttribPointer(shader->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
	glEnableVertexAttribArray(shader->positionAttribute);
	glVertexAttribPointer(shader->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
	glEnableVertexAttribArray(shader->texCoordAttribute);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(shader->positionAttribute);
	glDisableVertexAttribArray(shader->texCoordAttribute);

}

