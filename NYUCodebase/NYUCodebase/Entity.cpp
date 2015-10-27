#include "GameApp.h"

float lerp(float v0, float v1, float t) {
	return (1.0 - t)*v0 + t*v1;
}

Entity::Entity(GameApp *app) : app(app)
{}
Entity::Entity(GameApp *app, SheetSprite& sprite, float x, float y) : 
app(app), sprite(sprite), x(x), y(y), velocity_x(0.0f), velocity_y(0.0f),
accel_x(0.0f), accel_y(0.0f)
{}
void Entity::draw(){
	sprite.draw(app);
}

void Entity::update(float elapsed){
	float friction_x = 0.5;
	float friction_y = 0.5;

	velocity_x = lerp(velocity_x, 0.0f, elapsed * friction_x);
	velocity_y = lerp(velocity_y, 0.0f, elapsed * friction_y);
	velocity_x += accel_x * elapsed;
	velocity_y += accel_y * elapsed;
	x += velocity_x * elapsed;
	y += velocity_y * elapsed;


	transMatrix.identity();
	transMatrix.Translate(x, y, 0);
}

SheetSprite::SheetSprite(){}

SheetSprite::SheetSprite(unsigned int textureID, float u, float v, float width, float height, float size) :
textureID(textureID), u(u), v(v), width(width), height(height), size(size)
{}
void SheetSprite::draw(GameApp* app){
	glBindTexture(GL_TEXTURE_2D, textureID);
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

