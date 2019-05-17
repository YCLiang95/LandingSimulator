#pragma once

#include "ofMain.h"
#include "GameObject.h"

//--------------------------------------------------------------
//Yecheng Liang
//code reused from 2D vintage shooter
//basicly no changes

//The partical, emitted by the emitter
class Particle : public GameObject{
public:


	virtual void update() override;
	virtual void draw() override;
	virtual GameObject* clone() override;

	float lifeSpan = 3000;
	glm::vec3 direction;
	float speed;
	float timeOfSpawn;
};