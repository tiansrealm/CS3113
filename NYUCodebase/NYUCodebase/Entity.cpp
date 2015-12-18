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

Entity::Entity(ShaderProgram *shader, SheetSprite& sprite, float x, float y) :
shader(shader), sprite(sprite), x(x), y(y), velocity_x(0.0f), velocity_y(0.0f),
accel_x(0.0f), accel_y(0.0f), shape(RECTANGLE), is_static(false),
collideTop(false), collideBot(false), collideLeft(false), collideRight(false)
{
	height = sprite.height;
	width = sprite.width;
	matrix.identity();
	matrix.Translate(x, y, 0);
}
void Entity::draw(){
	sprite.draw(shader);
}

void Entity::update(float elapsed){
	float friction_x = 1.0;
	float friction_y = 1.0;

	//float gravity_x = 0 ;
	float gravity_y = -10.0f * 10;
	if (!is_static){
		velocity_y += gravity_y * elapsed;
	}


	velocity_x = lerp(velocity_x, 0.0f, elapsed * friction_x);
	//velocity_y = lerp(velocity_y, 0.0f, elapsed * friction_y);
	accel_x = lerp(accel_x, 0.0f, elapsed *  friction_x);
	velocity_x += accel_x * elapsed;
	velocity_y += accel_y * elapsed;


	move(velocity_x * elapsed, velocity_y * elapsed);

}
void Entity::move(float x_shift, float y_shift){
	x += x_shift;
	y += y_shift;
	matrix.Translate(x_shift, y_shift, 0);
}
bool Entity::collidesWith(const Entity& other, bool applyShift){
	if (shape == RECTANGLE && other.shape == RECTANGLE){
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
					velocity_y = 0;
					collideTop = true;
				}
				else  if (largestIntersect == botIntersect){
					move(0, botIntersect + extra);
					velocity_y = 0;
					collideBot = true;
				}
				else if (largestIntersect == leftIntersect){
					move(leftIntersect + extra, 0);
					velocity_x = 0;
					collideLeft = true;
				}
				else if (largestIntersect == rightIntersect){
					move(-(rightIntersect + extra), 0);
					velocity_x = 0;
					collideRight = true;
				}
			}
		return true;
	}

	return false;
}
//===================================================================================================================================

circleEntity::circleEntity(ShaderProgram *shader, SheetSprite& sprite, float x, float y)
	: Entity(shader, sprite, x, y)
{
	if (sprite.width == sprite.height){
		radius = sprite.width / 2;
	}
	else{ throw "need square spriteTexture for circle entity"; }
	shape = CIRCLE;
}



//===================================================================================================================================
SheetSprite::SheetSprite(){}

SheetSprite::SheetSprite(unsigned int textureID, float u, float v, float u_w, float v_h, float h, float w) :
textureID(textureID), u(u), v(v), u_width(u_w), v_height(v_h), height(h), width(w)
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

