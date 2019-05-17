#pragma once

#include "ofMain.h"
#include"GameObject.h"
#include "Force.h"

//--------------------------------------------------------------
//Yecheng Liang
//code reused from 2D vintage shooter

class ParticleSystem {
public:
	void update();
	vector<GameObject*> particles;
	void draw();
	void addParticle(GameObject* particle);
	void applyForce(ImpulseForce force);

	float weight = 100;
};