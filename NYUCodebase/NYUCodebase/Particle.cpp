#include "Particle.h"

float lerp(float v0, float v1, float t);

Color::Color() : r(0.0f), g(0.0f), b(0.0f), a(0.0f){}
Vector::Vector() : x(0.0f), y(0.0f){}

Vector:: Vector(float x, float y): x(x), y(y){}
Color::Color(float r, float g, float b, float a) :
	r(r), g(g), b(b), a(a) {}

Particle::Particle(Vector p, Vector v, float l, float sd, Color cd) : 
	position(p), velocity(v), lifetime(l), sizeDeviation(sd), colorDeviation(cd) {}

ParticleEmitter::ParticleEmitter(){}
ParticleEmitter::ParticleEmitter(unsigned int particleCount, unsigned int textureID, ShaderProgram *shader) 
	: textureID(textureID), shader(shader) {
	position = Vector(0.0f, 0.0f);
	velocity = Vector(20.0f, 20.0f);
	velocityDeviation = Vector(0.0f, 0.0f);
	colorDeviation = Color(0.0f, 0.0f, 0.0f, 0.0f);
	gravity = Vector(0.0f, -10.0f);
	maxLifetime = 5.0f;
	startSize = 10.0f;
	endSize = 0.0f;
	maxSizeDeviation = 2.0f;
	
	//initilize particles
	for (int i = 0; i < particleCount; ++i) {
		float randLifeTime = maxLifetime * ((float)rand() / (float)RAND_MAX);
		float randVelocityX = velocity.x + velocityDeviation.x * ((float)rand() / (float)RAND_MAX);
		float randVelocityY = velocity.y + velocityDeviation.y * ((float)rand() / (float)RAND_MAX);
		Vector randVelocity = Vector(randVelocityX, randVelocityY);
		float randSizeDev = maxSizeDeviation * ((float)rand() / (float)RAND_MAX);
		float randR = colorDeviation.r * ((float)rand() / (float)RAND_MAX);
		float randG = colorDeviation.g * ((float)rand() / (float)RAND_MAX);
		float randB = colorDeviation.b * ((float)rand() / (float)RAND_MAX);
		float randA = colorDeviation.a * ((float)rand() / (float)RAND_MAX);
		Color randColorDev = Color(randR, randG, randB, randA);
		particles.push_back(new Particle(position, randVelocity, randLifeTime, randSizeDev, randColorDev));

	}
}
void ParticleEmitter::update(float elapsed){
	for (int i = 0; i < particles.size(); i++) {
		particles[i]->position.x += particles[i]->velocity.x * elapsed;
		particles[i]->position.y += particles[i]->velocity.y * elapsed;
		particles[i]->velocity.x += gravity.x * elapsed;
		particles[i]->velocity.y += gravity.y * elapsed;
		particles[i]->lifetime += elapsed;
		if (particles[i]->lifetime >= maxLifetime){
			particles[i]->position = position;
			particles[i]->velocity = velocity;
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
			particles[i]->position.x, particles[i]->position.y + size,
			particles[i]->position.x, particles[i]->position.y,
			particles[i]->position.x + size, particles[i]->position.y + size,
			particles[i]->position.x + size, particles[i]->position.y + size,
			particles[i]->position.x, particles[i]->position.y,
			particles[i]->position.x + size, particles[i]->position.y 
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