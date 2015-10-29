#include "GameApp.h"
#include <cmath>

float lerp(float v0, float v1, float t) {
	return (1.0 - t)*v0 + t*v1;
}
bool circleCollison(float x1, float x2, float y1, float y2, float r1, float r2){
	float distance_between = sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
	return distance_between > (r1 + r2);
}
Entity::Entity(){}

Entity::Entity(GameApp *app, SheetSprite& sprite, float x, float y) : 
app(app), sprite(sprite), x(x), y(y), velocity_x(0.0f), velocity_y(0.0f),
accel_x(0.0f), accel_y(0.0f), shape("rectangle")
{
	height = sprite.size;
	width = sprite.size * sprite.height / sprite.width;
	matrix.identity();
	matrix.Translate(x, y, 0);
}
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
	move(velocity_x * elapsed, velocity_y * elapsed);


	for (size_t i = 0; i < app->entities.size(); i++){
		if (this != app->entities[i]){
			collidesWith(*(app->entities[i]), true);
		}
	}
	/*
	x += velocity_x * elapsed;
	y += velocity_y * elapsed;
	matrix.identity();
	matrix.Translate(x, y, 0);
	*/
}
void Entity::move(float xShift, float yShift){
	x += xShift;
	y += yShift;
	matrix.Translate(xShift, yShift, 0);
}
bool Entity::collidesWith(const Entity& other, bool applyShift){
	if (shape == "rectangle" && other.shape == "rectangle"){
		float r1Top = y, r2Top = other.y;
		float r1Bottom = y - height, r2Bottom = other.y - other.height;
		float r1Left = x, r2Left = other.x;
		float r1Right = x + width, r2Right = other.x + other.width;
		float topIntersect = r1Top - r2Bottom;
		float botIntersect = r2Top - r1Bottom;
		float leftIntersect = r2Right - r1Left;
		float rightIntersect = r1Right - r2Left;
		
		//if ( ! ( (r1Bottom > r2Top) || (r1Top < r2Bottom) || 
		//	     (r1Left > r2Right) || (r1Right < r2Left) )  ){
		if ((topIntersect > 0) && (botIntersect > 0) && (leftIntersect > 0) && (rightIntersect > 0))
			if (applyShift){
				float largestIntersect = fmin(topIntersect, fmin(botIntersect, fmin(leftIntersect, rightIntersect)));
				float extra = 0.0001;
				
				if (largestIntersect == topIntersect){
					move(0, -(topIntersect + extra));
				}
				else  if (largestIntersect == botIntersect){
					move(0, botIntersect + extra);
				}
				else if (largestIntersect == leftIntersect){
					move(leftIntersect + extra, 0);
				}
				else if (largestIntersect == rightIntersect){
					move(-(rightIntersect + extra), 0);
				}
			}
		return true;
	}
	
	return false;
}
//===================================================================================================================================

circleEntity::circleEntity(GameApp * app, SheetSprite& sprite, float x, float y)
	: Entity(app, sprite, x, y)
{
	if (sprite.width == sprite.height){
		radius = sprite.width / 2;
	}
	else{ throw "need square spriteTexture for circle entity"; }

	shape = "circle";

}



//===================================================================================================================================
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

		float scale_height = size;
		float scale_width = size * width / height;
		//float aspect = width/height
		float vertices[] = {
			//top left corner = 0,0
			0.0f, -1.0f * scale_height,
			1.0f * scale_width, 0.0f,
			0.0f, 0.0 * scale_height,
			1.0f * scale_width, 0.0f,
			0.0f, -1.0f * scale_height,
			1.0f * scale_width, -1.0f * scale_height

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
		glVertexAttribPointer(app->program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
		glEnableVertexAttribArray(app->program->positionAttribute);
		glVertexAttribPointer(app->program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
		glEnableVertexAttribArray(app->program->texCoordAttribute);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glDisableVertexAttribArray(app->program->texCoordAttribute);
		
}

