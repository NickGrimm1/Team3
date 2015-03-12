#if WINDOWS_BUILD
#include "ParticleEmitter.h"
#include <string>
#include <io.h>
#include <stdio.h>

#define RAND() ((rand()%101)/100.0f)

ParticleEmitter::ParticleEmitter(void)	{
	particleRate		= 100.0f;
	particleLifetime	= 500.0f;
	particleSize		= 24.0f;
	particleVariance	= 0.2f;
	nextParticleTime	= 0.0f;
	particleSpeed		= 0.2f;
	numLaunchParticles	= 10;
	largestSize			= 0;
}


ParticleEmitter::~ParticleEmitter(void)	{
	for(std::vector<Particle *>::iterator i = particles.begin(); i != particles.end(); ++i) {
		delete (*i);
	}
	for(std::vector<Particle *>::iterator i = freeList.begin(); i != freeList.end(); ++i) {
		delete (*i);
	}
}

void ParticleEmitter::Update(float msec)	{
	nextParticleTime -= msec;

	while(nextParticleTime <= 0) {
		nextParticleTime += particleRate;
		for(int i = 0; i < numLaunchParticles; ++i) {
			particles.push_back(GetFreeParticle());
		}
	}

	for(std::vector<Particle *>::iterator i = particles.begin(); i != particles.end();) {
		Particle *p = (*i);

		p->colour.w -= (msec / particleLifetime);

		if(p->colour.w <= 0.0f) {
			freeList.push_back(p);
			i = particles.erase(i);
		}
		else{
			p->position += p->direction*(msec*particleSpeed);
			++i;
		}
	}

	if(particles.size() > largestSize) {
		ResizeArrays();	
	}
}

Particle* ParticleEmitter::GetFreeParticle()	{
	Particle * p = NULL;

	if(freeList.size() > 0) {
		p = freeList.back();
		freeList.pop_back();
	}
	else{ 
		p = new Particle();
	}

	p->colour		= T3Vector4(RAND(),RAND(),RAND(),1.0);
	p->direction	= initialDirection;
	p->direction.x += ((RAND()-RAND()) * particleVariance);
	p->direction.y += ((RAND()-RAND()) * particleVariance);
	p->direction.z += ((RAND()-RAND()) * particleVariance);

	p->direction.Normalise();
	p->position.ToZero();

	return p;
}

void ParticleEmitter::ResizeArrays() {
	delete[] vertices;

	glDeleteBuffers(1, &bufferObject[0]);

	vertices = new Vertex[particles.size()];

	glGenBuffers(1, &bufferObject[0]);
	glBindBuffer(GL_ARRAY_BUFFER, bufferObject[0]);
	glBufferData(GL_ARRAY_BUFFER, particles.size() * sizeof(Vertex), 0, GL_DYNAMIC_DRAW);


	glBindBuffer(GL_ARRAY_BUFFER, 0);

	largestSize = particles.size();
}

void ParticleEmitter::Draw()	{
	for(unsigned int i = 0; i < particles.size(); ++i) {
		vertices[i] = Vertex(particles.at(i)->position, T3Vector3(), particles.at(i)->colour);
	}

	glBindVertexArray(arrayObject);

	glBindBuffer(GL_ARRAY_BUFFER, bufferObject[0]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, particles.size() * sizeof(Vertex), (void*)vertices);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0); // Position
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(T3Vector3))); // Normal
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(T3Vector3) + sizeof(T3Vector3))); // Color
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(T3Vector3) + sizeof(T3Vector3) + sizeof(T3Vector4))); // TexCoord
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(T3Vector3) + sizeof(T3Vector3) + sizeof(T3Vector4) + sizeof(T3Vector2))); // Tangent	//Tell the VAO we have positions...
	glEnableVertexAttribArray(0);	//Binds this buffer to the VAO

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);

	//And now do our usual Drawing stuff...
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture->GetTextureName());
	glDrawArrays(GL_POINTS,  0, particles.size());	// draw ordered list of vertices
	glBindTexture(GL_TEXTURE_2D, 0);

	glDisable(GL_BLEND);
	glBindVertexArray(0); //Remember to turn off our VAO ;)
};

#endif