#include "Particle.h"

void Particle::update() {
	if (ofGetElapsedTimeMillis() - timeOfSpawn > lifeSpan) isDead = true;
	transform.update();
}

void Particle::draw() {
	ofDrawSphere(transform.position, .1);
}

GameObject* Particle::clone() {
	Particle* result = new Particle(*this);
	result->timeOfSpawn = ofGetElapsedTimeMillis();
	return result;
};