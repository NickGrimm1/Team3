#include "Weather.h"

#define RAND() ((rand()%101)/100.0f)

void Weather::Update(float msec) {
	nextParticleTime -= msec;

	while(nextParticleTime <= 0) {
		nextParticleTime += particleRate;
		//Add a number of particles to the vector, obtained from the free list.
		for(int i = 0; i < numLaunchParticles; ++i) {
			particles.push_back(GetFreeParticle());
		}
	}

	for(std::vector<Particle *>::iterator i = particles.begin(); i != particles.end();/*No I++ here!!! */) {
		Particle *p = (*i);

		p->colour.w -= (msec / (particleLifetime * 2));

		if(p->colour.w <= 0.5f) {
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

Particle* Weather::GetFreeParticle() {
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
	
	p->colour	= Vector4(1.0,1.0,1.0,1.0);
	p->direction	= Vector3(0,-1,0);
	p->direction.x += 0.0f;
	p->direction.y += -1.0f;
	p->direction.z += 0.0f;

	p->direction.Normalise();	//Keep its direction normalised!
	p->position.x = rand() % (int)(dimensions.x);
	p->position.y = dimensions.y;
	p->position.z = rand() % (int)(dimensions.z);

	return p;	//return the new particle :-)
}
