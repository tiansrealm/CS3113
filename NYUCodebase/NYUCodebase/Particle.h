#pragma once
#include "Tools.h"

class Particle { 
public:  
	Particle(Vector p, Vector v, float l, float sd, Color cd);
	Vector pos;         
	Vector vel;         
	float lifetime; 
	float sizeDeviation;
	Color colorDeviation;
};

class ParticleEmitter { 
public:         
	ParticleEmitter(unsigned int particleCount, unsigned int textureID, ShaderProgram *shader, float x = 0, float y = 0);
	ParticleEmitter();                   
	void update(float elapsed);         
	void render();
	void resetParticles();

	bool is_on;
	Vector pos;
	Vector vel;
	Vector velDev;
	Vector gravity;         
	float maxLifetime;
	float startSize;         
	float endSize;         
	float maxSizeDeviation;
	unsigned int pCount;
	Color startColor;
	Color endColor;
	Color colorDeviation;
	std::vector<Particle*> particles;
	unsigned int textureID;
	ShaderProgram *shader;
	
};