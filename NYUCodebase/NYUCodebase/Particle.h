#pragma once
#include <vector>
#include "GameApp.h"
struct Vector {
	Vector::Vector();
	Vector(float x, float y);
	float x;
	float y;
};
struct Color {
	Color();
	Color(float r, float g, float b, float a);
	float r;
	float g;
	float b;
	float a;
};

class Particle { 
public:  
	Particle(Vector p, Vector v, float l, float sd, Color cd);
	Vector position;         
	Vector velocity;         
	float lifetime; 
	float sizeDeviation;
	Color colorDeviation;
};

class ParticleEmitter { 
public:         
	ParticleEmitter(unsigned int particleCount, unsigned int textureID, ShaderProgram *shader);
	ParticleEmitter();                   
	void update(float elapsed);         
	void render();
	
	Vector position;
	Vector velocity;
	Vector velocityDeviation;
	Vector gravity;         
	float maxLifetime;
	float startSize;         
	float endSize;         
	float maxSizeDeviation;
	Color startColor;
	Color endColor;
	Color colorDeviation;
	std::vector<Particle*> particles;
	unsigned int textureID;
	ShaderProgram *shader;
	
};