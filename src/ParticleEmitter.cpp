#include "ParticleEmitter.h"


void ParticleEmitter::update() {
	if (active) {
		if ((ofGetElapsedTimeMillis() - timeLastSpawn) > interval) {
			spawn();
		}
	}

	//Update all particals
	for (vector<GameObject*>::iterator it = particles.begin(); it != particles.end();) {
		(**it).update();
		//Removed dead particals
		if ((**it).isDead) {
			it = particles.erase(it);
			//cout << "Partical Despawn!" << endl;
		} else {
			++it;
		}
	}
}

//Spawn a new partical
//This part will be modulized later for more flexibility
void ParticleEmitter::spawn() {
	timeLastSpawn = ofGetElapsedTimeMillis();

	particle->transform.position = transform.getPosition();
	GameObject* p = particle->clone();

	if (emmitionSound.isLoaded())
		emmitionSound.play();

	p->transform.speed = speed;
	p->transform.direction = direction;
	p->transform.speedDirection = direction;

	pSystem->addParticle(p);
}

void ParticleEmitter::draw() {
	for (vector<GameObject*>::iterator it = particles.begin(); it != particles.end(); ++it) {
		if (!(**it).isDead) (**it).draw();
	}
}

ParticleEmitter::ParticleEmitter(ParticleSystem* ps, GameObject* p) {
	pSystem = ps;
	particle = p;
	transform = Transform();
	direction = glm::vec3(0, 0, 0);
	lifeSpan = 3000;
	speed = 1;
	interval = 1000;
	timeLastSpawn = ofGetElapsedTimeMillis();
}