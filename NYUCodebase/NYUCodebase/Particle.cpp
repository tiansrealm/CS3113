#include "Particle.h"
#include "Tools.h"


Color::Color() : r(0.0f), g(0.0f), b(0.0f), a(0.0f){}
Vector::Vector() : x(0.0f), y(0.0f){}

Vector:: Vector(float x, float y): x(x), y(y){}
Color::Color(float r, float g, float b, float a) :
	r(r), g(g), b(b), a(a) {}

Particle::Particle(Vector p, Vector v, float l, float sd, Color cd) : 
	pos(p), vel(v), lifetime(l), sizeDeviation(sd), colorDeviation(cd) {}

ParticleEmitter::ParticleEmitter(){}
ParticleEmitter::ParticleEmitter(unsigned int particleCount, unsigned int textureID, ShaderProgram *shader,
	float x, float y) 
	: textureID(textureID), shader(shader) {
	pos = Vector(x, y);
	vel = Vector(0.0f, -10.0f);
	velDev = Vector(0.0f, 0.0f);
	colorDeviation = Color(0.0f, 0.0f, 0.0f, 0.0f);
	gravity = Vector(0.0f, -10.0f);
	maxLifetime = 1.5f;
	startSize = 20.0f;
	endSize = 0.0f;
	maxSizeDeviation = 2.0f;
	pCount = particleCount;
	is_on = true;
	//initilize particles
	for (int i = 0; i < particleCount; ++i) {
		float randLifeTime = maxLifetime * ((float)rand() / (float)RAND_MAX);
		float randvelX = vel.x + velDev.x * ((float)rand() / (float)RAND_MAX);
		float randvelY = vel.y + velDev.y * ((float)rand() / (float)RAND_MAX);
		Vector randvel = Vector(randvelX, randvelY);
		float randSizeDev = maxSizeDeviation * ((float)rand() / (float)RAND_MAX);
		float randR = colorDeviation.r * ((float)rand() / (float)RAND_MAX);
		float randG = colorDeviation.g * ((float)rand() / (float)RAND_MAX);
		float randB = colorDeviation.b * ((float)rand() / (float)RAND_MAX);
		float randA = colorDeviation.a * ((float)rand() / (float)RAND_MAX);
		Color randColorDev = Color(randR, randG, randB, randA);
		particles.push_back(new Particle(pos, randvel, randLifeTime, randSizeDev, randColorDev));

	}
}
void ParticleEmitter::resetParticles(){
	for(size_t i = 0; i < particles.size(); ++i){
		delete particles[i];
	}
	particles.clear();

	//initilize particles
	for (int i = 0; i < pCount; ++i) {
		float randLifeTime = maxLifetime * ((float)rand() / (float)RAND_MAX);
		float randvelX = vel.x + velDev.x * ((float)rand() / (float)RAND_MAX);
		float randvelY = vel.y + velDev.y * ((float)rand() / (float)RAND_MAX);
		Vector randvel = Vector(randvelX, randvelY);
		float randSizeDev = maxSizeDeviation * ((float)rand() / (float)RAND_MAX);
		float randR = colorDeviation.r * ((float)rand() / (float)RAND_MAX);
		float randG = colorDeviation.g * ((float)rand() / (float)RAND_MAX);
		float randB = colorDeviation.b * ((float)rand() / (float)RAND_MAX);
		float randA = colorDeviation.a * ((float)rand() / (float)RAND_MAX);
		Color randColorDev = Color(randR, randG, randB, randA);
		particles.push_back(new Particle(pos, randvel, randLifeTime, randSizeDev, randColorDev));

	}
}
void ParticleEmitter::update(float elapsed){
	for (int i = 0; i < particles.size(); i++) {
		particles[i]->pos.x += particles[i]->vel.x * elapsed;
		particles[i]->pos.y += particles[i]->vel.y * elapsed;
		particles[i]->vel.x += gravity.x * elapsed;
		particles[i]->vel.y += gravity.y * elapsed;
		particles[i]->lifetime += elapsed;
		if (particles[i]->lifetime >= maxLifetime){
			particles[i]->pos = pos;
			float randvelX = vel.x + velDev.x * ((float)rand() / (float)RAND_MAX);
			float randvelY = vel.y + velDev.y * ((float)rand() / (float)RAND_MAX);
			particles[i]->vel = Vector(randvelX, randvelY);
			particles[i]->lifetime = 0;
		}
	}
}
void ParticleEmitter::render(){
	
	std::vector<float> vertices;
	std::vector<float> texCoords;
	std::vector<float> colors;
	for (int i = 0; i < particles.size(); i++) {
		float m = (particles[i]->lifetime / maxLifetime);
		float size = lerp(startSize, endSize, m) + particles[i]->sizeDeviation;

		vertices.insert(vertices.end(), {
			particles[i]->pos.x, particles[i]->pos.y + size,
			particles[i]->pos.x, particles[i]->pos.y,
			particles[i]->pos.x + size, particles[i]->pos.y + size,
			particles[i]->pos.x + size, particles[i]->pos.y + size,
			particles[i]->pos.x, particles[i]->pos.y,
			particles[i]->pos.x + size, particles[i]->pos.y 
		});

		texCoords.insert(texCoords.end(), {
			0.0f, 0.0f,
			0.0f, 1.0f,
			1.0f, 0.0f,
			1.0f, 0.0f,
			0.0f, 1.0f,
			1.0f, 1.0f
		});
		for (int j = 0; j < 6; j++) {
			colors.push_back(lerp(startColor.r, endColor.r, m));
			colors.push_back(lerp(startColor.g, endColor.g, m));
			colors.push_back(lerp(startColor.b, endColor.b, m));
			colors.push_back(lerp(startColor.a, endColor.a, m));
		}
	}



	//GLuint colorAttribute = glGetAttribLocation(shader->programID, "color");
	glVertexAttribPointer(shader->positionAttribute, 2, GL_FLOAT, false, 0, vertices.data());
	glEnableVertexAttribArray(shader->positionAttribute);
	//glVertexAttribPointer(colorAttribute, 4, GL_FLOAT, false, 0, colors.data());
	//glEnableVertexAttribArray(colorAttribute);
	glVertexAttribPointer(shader->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords.data());
	glEnableVertexAttribArray(shader->texCoordAttribute);

	glBindTexture(GL_TEXTURE_2D, textureID);
	glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 2);

	glDisableVertexAttribArray(shader->positionAttribute);
	glDisableVertexAttribArray(shader->texCoordAttribute);
	//glDisableVertexAttribArray(colorAttribute);
}