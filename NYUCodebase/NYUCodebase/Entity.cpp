#include "GameApp.h"

Entity::Entity(GameApp *app) : app(app)
{}
Entity::Entity(GameApp *app, SheetSprite* sprite) : app(app), sprite(sprite)
{}
void Entity::draw(){
	sprite->draw(app, x, y);
}

void Entity::update(float elapsed){

}

SheetSprite::SheetSprite(unsigned int textureID, float u, float v, float width, float height, float size) :
textureID(textureID), u(u), v(v), width(width), height(height), size(size)
{}
void SheetSprite::draw(GameApp* app, float x, float y){
	glBindTexture(GL_TEXTURE_2D, textureID);
	u = 0;
	v = 0;
	width = 1;
	height = 1;
	size = 1;
	GLfloat texCoords[] = {
		u, v + height,
		u + width, v,
		u, v,
		u + width, v,
		u, v + height,
		u + width, v + height
	};

	float aspect = width / height;
	float vertices[] = {
		-0.5f * size * aspect, -0.5f * size,
		0.5f * size * aspect, 0.5f * size,
		-0.5f * size * aspect, 0.5f * size,
		0.5f * size * aspect, 0.5f * size,
		-0.5f * size * aspect, -0.5f * size,
		0.5f * size * aspect, -0.5f * size
	};
	glVertexAttribPointer(app->program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
	glEnableVertexAttribArray(app->program->positionAttribute);
	glVertexAttribPointer(app->program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
	glEnableVertexAttribArray(app->program->texCoordAttribute);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(app->program->texCoordAttribute);
}
