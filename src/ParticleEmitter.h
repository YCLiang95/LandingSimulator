#pragma once

#include "ofMain.h"
#include "Particle.h"
#include "ParticleSystem.h"

//--------------------------------------------------------------
//Yecheng Liang
//code reused from 2D vintage shooter
//basicly no changes

class ParticleEmitter: public GameObject {
public:
	GameObject* particle;
	ParticleSystem* pSystem;

	//How long spawned partical should lived, in mil-sec
	float lifeSpan;
	//The direction of the spawn partical
	glm::vec3 direction;
	//The speed of the spawned partical
	float speed;

	//How long, in mil-sec, between partical spawn
	float interval;
	//System time since last partical spawn
	float timeLastSpawn;

	//Sound when partical spawn
	ofSoundPlayer emmitionSound;

	//only active emitter spawn new partical
	bool active = false;

	//The particals emitted by this emiters
	//This might be move to partical system class in the future
	vector<GameObject*> particles;

	ParticleEmitter() {};
	ParticleEmitter(ParticleSystem* ps, GameObject* p);
	
	virtual void update() override;
	virtual void draw() override;

	bool bApplyForce = false;
	ImpulseForce force;

	//Spawn a new partical
	void spawn();
};