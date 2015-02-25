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

/*
In our destructor, we must make sure we delete the particles held in BOTH
of our lists!
*/
ParticleEmitter::~ParticleEmitter(void)	{
	for(std::vector<Particle *>::iterator i = particles.begin(); i != particles.end(); ++i) {
		delete (*i);
	}
	for(std::vector<Particle *>::iterator i = freeList.begin(); i != freeList.end(); ++i) {
		delete (*i);
	}
}

/*
Every frame, we're going to call this Update function. In a 'real' game, this maybe wouldn't
be the Renderer's job, but maybe part of the gameplay 'entity' system. Depends on what
the game components can 'see' (i.e are we hiding what type of API we're using for rendering
from the rest of the game, etc).

It goes through every particle of this emitter, and updates it. if it has no 'life' left,
it is removed from the particle list. If it's time to generate some new particles, we do
that in here, too. Finally, this function resizes our VBOs if necessary. 
*/
void ParticleEmitter::Update(float msec)	{
	nextParticleTime -= msec;	//some time has passed!

	while(nextParticleTime <= 0) {
		nextParticleTime += particleRate;
		//Add a number of particles to the vector, obtained from the free list.
		for(int i = 0; i < numLaunchParticles; ++i) {
			particles.push_back(GetFreeParticle());
		}
	}

	for(std::vector<Particle *>::iterator i = particles.begin(); i != particles.end();/*No I++ here!!! */) {
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

	//If we now have more particles than we have graphics memory for, we
	//must allocate some new space for them, using ResizeArrays.
	if(particles.size() > largestSize) {
		ResizeArrays();	
	}
}

/*
This function gets a pointer to an unused particle - either from the freelist,
or from a newly created particle on the heap.
*/
Particle* ParticleEmitter::GetFreeParticle()	{
	Particle * p = NULL;

	//If we have a spare particle on the freelist, pop it off!
	if(freeList.size() > 0) {
		p = freeList.back();
		freeList.pop_back();
	}
	else{ 
		//no spare particles, so we need a new one
		p = new Particle();
	}

	//Now we have to reset its values - if it was popped off the
	//free list, it'll still have the values of its 'previous life'

	p->colour		= T3Vector4(RAND(),RAND(),RAND(),1.0);
	p->direction	= initialDirection;
	p->direction.x += ((RAND()-RAND()) * particleVariance);
	p->direction.y += ((RAND()-RAND()) * particleVariance);
	p->direction.z += ((RAND()-RAND()) * particleVariance);

	p->direction.Normalise();	//Keep its direction normalised!
	p->position.ToZero();

	return p;	//return the new particle :-)
}

/*
If we end up with more particles than space in graphics memory, we must
allocate some new graphics memory!
*/
void ParticleEmitter::ResizeArrays() {
	//Delete the system memory, as it is of the wrong size now...
	//yes, this does mean we're keeping particle info in THREE places...
	delete[] vertices;

	//Tell OpenGL that we can delete our old VBOs
	glDeleteBuffers(1, &bufferObject[0]);

	//Make some new system memory
	vertices = new Vertex[particles.size()];  // TODO - Maybe fix the wasted space...

	//Make some new VBO space in memory. Note how we don't actually upload any
	//data to the graphics card here! We just 'new' the memory for now.
	glGenBuffers(1, &bufferObject[0]);
	glBindBuffer(GL_ARRAY_BUFFER, bufferObject[0]);
	glBufferData(GL_ARRAY_BUFFER, particles.size() * sizeof(Vertex), 0, GL_DYNAMIC_DRAW);


	glBindBuffer(GL_ARRAY_BUFFER, 0);	//Done with our buffers...

	largestSize = particles.size();		//Update our largest particles size
}

/*
Now for the overloaded Draw function. It's a little bit messy, as we have to copy all of the
Particle data structs into one contiguous lump of memory to send using the OpenGL command
glBufferSubData - which you should remember from the skeletal animation tutorial. 

Why don't we use glMapBuffer? Just uploading a whole load of new data is probably easier
and quicker than faffing with mapping and unmapping buffers!
*/
void ParticleEmitter::Draw()	{
	//Get 2 contiguous sections of memory full of our particle info
	for(unsigned int i = 0; i < particles.size(); ++i) {
		vertices[i] = Vertex(particles.at(i)->position, T3Vector3(), particles.at(i)->colour);
	}

	glBindVertexArray(arrayObject);

	//Bind our vertex data, and update its data with that of the vertices array
	glBindBuffer(GL_ARRAY_BUFFER, bufferObject[0]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, particles.size() * sizeof(Vertex), (void*)vertices);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0); // Position
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(T3Vector3))); // Normal
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(T3Vector3) + sizeof(T3Vector3))); // Color
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(T3Vector3) + sizeof(T3Vector3) + sizeof(T3Vector4))); // TexCoord
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(T3Vector3) + sizeof(T3Vector3) + sizeof(T3Vector4) + sizeof(T3Vector2))); // Tangent	//Tell the VAO we have positions...
	glEnableVertexAttribArray(0);	//Binds this buffer to the VAO

	//We're going to use a type of alpha blending known as additive blending here. Overlapping 
	//particles will have their colours added together on screen, giving us a cool effect when
	//particles are near each other. Fire a lot of particles at once with slow speed to get a
	//'plasma ball' type effect!
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