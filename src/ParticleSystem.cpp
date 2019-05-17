#include "ParticleSystem.h"

//--------------------------------------------------------------
//Yecheng Liang
//code reused from 2D vintage shooter
//basicly no changes

void ParticleSystem::update(){
	//Update all particals
	for (vector<GameObject*>::iterator it = particles.begin(); it != particles.end();) {
		(**it).update();
		//Removed dead particals
		if ((**it).isDead) {
			it = particles.erase(it);
			//cout << "Partical Despawn!" << endl;
		}
		else {
			++it;
		}
	}
};

void ParticleSystem::draw() {
	//draw all particle
	for (vector<GameObject*>::iterator it = particles.begin(); it != particles.end(); ++it) {
		if (!(**it).isDead) (**it).draw();
	}
};

void ParticleSystem::addParticle(GameObject* particle) {
	particles.push_back(particle);
}

void ParticleSystem::applyForce(ImpulseForce force) {
	for (vector<GameObject*>::iterator it = particles.begin(); it != particles.end();)
		(**it).transform.applyForce(weight, force);
}
