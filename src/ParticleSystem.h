#pragma once

#include "ofMain.h"
#include"GameObject.h"
#include "Force.h"

//Not Implemented yet
//A Partical system will controll mutilple emitter

class ParticleSystem {
public:
	void update();
	vector<GameObject*> particles;
	void draw();
	void addParticle(GameObject* particle);
	void applyForce(Force force);
};