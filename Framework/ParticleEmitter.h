#pragma once
#include "../Framework/T3Vector3.h"
#include "../Framework/T3Vector4.h"
#include "../Framework/OGLRenderer.h"
#include "../Team3Project1/Mesh.h"
#include <vector>
#include "../Team3Project1/Texture.h"

struct Particle {
	T3Vector3 position;
	T3Vector4 colour;
	T3Vector3 direction;
};

class ParticleEmitter : public Mesh	{
public:
	ParticleEmitter(void);
	~ParticleEmitter(void);

	void Update(float msec);

	virtual void Draw();

	void	SetParticleTexture(Texture* tex)	{ texture = tex; }

	float	GetParticleRate()				{return particleRate;}
	void	SetParticleRate(float rate)		{particleRate = rate;}

	float	GetParticleLifetime()			{return particleLifetime;}
	void	SetParticleLifetime(float life) {particleLifetime = life;}

	float	GetParticleSize()				{return particleSize;}
	void	SetParticleSize(float size)		{particleSize = size;}

	float	GetParticleVariance()				{return particleVariance;}
	void	SetParticleVariance(float variance) {particleVariance = variance;}

	float	GetParticleSpeed()				{return particleSpeed;}
	void	SetParticleSpeed(float speed)	{particleSpeed = speed;}

	int		GetLaunchParticles()			{return numLaunchParticles;}
	void	SetLaunchParticles(int num)		{numLaunchParticles = num;}

	void	SetDirection(const T3Vector3 dir) {initialDirection = dir;}
	T3Vector3 GetDirection()					{return initialDirection;}

protected:
	Particle* GetFreeParticle();

	void	ResizeArrays();

	float particleRate;
	float particleLifetime;
	float particleSize;
	float particleVariance;
	float particleSpeed;
	int	  numLaunchParticles;

	T3Vector3 initialDirection;

	float nextParticleTime;		//How long until we next spit out some particles?

	unsigned int largestSize;	//How large has our particle array become?

	std::vector<Particle*>	particles;	//Active particles stay in here :)
	std::vector<Particle*>	freeList;	//'Spare' particles stay in here...

	Texture* texture;
};

