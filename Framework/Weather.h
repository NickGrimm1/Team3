#pragma once
#include "../Framework/ParticleEmitter.h"

//Extended the class so i could overload the position code, so that particles spawn over the 
//terrain. Added some ease of use things too because YOLO.
class Weather :	public ParticleEmitter
{
public:
	Weather(Vector3 dim) : ParticleEmitter(){ dimensions = dim; }
	virtual ~Weather(void){};

	void Update(float msec);
protected:
	Vector3	  dimensions;

	Particle* GetFreeParticle();
};

