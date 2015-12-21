#include "Entity.h"
#include "Tools.h"
#include <cmath>


Entity::Entity(){}

Entity::Entity(ShaderProgram *shader, SheetSprite* sprite, float x, float y, bool is_static) :
shader(shader), sprite(sprite), pos(Vector(x,y)), vel(Vector(0.0f,0.0f)),
accel(Vector(0.0f,0.0f)), shape(RECTANGLE), is_static(is_static),
collideTop(false), collideBot(false), collideLeft(false), collideRight(false)
{

	height = sprite->height;
	width = sprite->width;
	matrix.identity();
	matrix.Translate(x, y, 0);
	hp = 1;
	name = "none";
	rotation = 0.0f;
	aniTime = 0;
}
void Entity::draw(){
	sprite->width = width;
	sprite->height = height;
	sprite->draw(shader);
}

void Entity::setWidthHeight(float w, float h){
	width = w;
	height = h;
}
void Entity::setPos(float x, float y){
	pos.x = x;
	pos.y = y;
	matrix.identity();
	matrix.Translate(x, y, 0);
}
void Entity::update(float elapsed){
	collideTop = collideBot = collideRight = collideLeft = false;

	if (!is_static){
		float friction_x = 1.0;
		float friction_y = 1.0;

		//float gravity_x = 0 ;
		float gravity_y = -10.0f * 10;

		vel.y += gravity_y * elapsed;
		move(vel.x * elapsed, vel.y * elapsed);
		vel.x = lerp(vel.x, 0.0f, elapsed * friction_x);
		//vel.y = lerp(vel.y, 0.0f, elapsed * friction_y);
		accel.x = lerp(accel.x, 0.0f, elapsed *  friction_x);
		vel.x += accel.x * elapsed;
		vel.y += accel.y * elapsed;

	}

}
void Entity::move(float x_shift, float y_shift){
	pos.x += x_shift;
	pos.y += y_shift;
	matrix.Translate(x_shift, y_shift, 0);
}
bool Entity::collidesWith(int x, int y){

	return (x >= pos.x && x <= pos.x + width && y <= pos.y && y >= pos.y - height);
}
bool Entity::collidesWith(const Entity& other, bool applyShift){
	if (shape == RECTANGLE && other.shape == RECTANGLE){
		float r1Top = pos.y, r2Top = other.pos.y;
		float r1Bottom = pos.y - height, r2Bottom = other.pos.y - other.height;
		float r1Left = pos.x, r2Left = other.pos.x;
		float r1Right = pos.x + width, r2Right = other.pos.x + other.width;
		float topIntersect = r1Top - r2Bottom;
		float botIntersect = r2Top - r1Bottom;
		float leftIntersect = r2Right - r1Left;
		float rightIntersect = r1Right - r2Left;

		//if ( ! ( (r1Bottom > r2Top) || (r1Top < r2Bottom) || 
		//	     (r1Left > r2Right) || (r1Right < r2Left) )  ){
		if ((topIntersect >= 0) && (botIntersect >= 0) && (leftIntersect >= 0) && (rightIntersect >= 0)){
			if (applyShift){
				float largestIntersect = fmin(topIntersect, fmin(botIntersect, fmin(leftIntersect, rightIntersect)));
				float extra = 0.0001;

				if (largestIntersect == topIntersect){
					move(0, -(topIntersect + extra));
					vel.y = 0;
					collideTop = true;
				}
				else  if (largestIntersect == botIntersect){
					move(0, botIntersect + extra);
					vel.y = 0;
					collideBot = true;
				}
				else if (largestIntersect == leftIntersect){
					move(leftIntersect + extra, 0);
					vel.x = 0;
					collideLeft = true;
				}
				else if (largestIntersect == rightIntersect){
					move(-(rightIntersect + extra), 0);
					vel.x = 0;
					collideRight = true;
				}
			}
			
			return true;
		}
	}
	return false;
}
//===================================================================================================================================

circleEntity::circleEntity(ShaderProgram *shader, SheetSprite* sprite, float x, float y)
	: Entity(shader, sprite, x, y)
{
	if (sprite->width == sprite->height){
		radius = sprite->width / 2;
	}
	else{ throw "need square spriteTexture for circle entity"; }
	shape = CIRCLE;
}



//===================================================================================================================================
