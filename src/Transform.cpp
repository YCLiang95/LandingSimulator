#include "Transform.h"

//Yecheng Liang
//code reused from 2D vintage shooter

glm::vec3 Transform::getPosition() {
          	if (parent != NULL)
		return parent->getPosition() + position;
	else
		return position;
};


//update the transform
//calculate the speed and direction based on acceration
//Rotational direction are ignored in this project, which was used in 2D vintage shooter
void Transform::update() {

	if (acceration > 0) {
		speedDirection = speedDirection * speed + accerationDirection * acceration;
		speed = speedDirection.length();
		speedDirection = speedDirection / speed;

		if (speed > 0.0f)
			speed = min(speed, maxSpeed);
		else
			speed = max(speed, -maxSpeed);
	} else if (speed != 0) {
		speed *= (1 - drag);
		if (abs(speed) < 0.05f) speed = 0.0f;
	}
	if (abs(speed) > 0.0f) {
		position += speedDirection * speed;
	}
	acceration = 0;
	accerationDirection.x = 0;
	accerationDirection.y = 0;
	accerationDirection.z = 0;
}

//Apply a force
//Calculate the acceration based on force and weight
void Transform::applyForce(float weight, ImpulseForce force) {
	accerationDirection = accerationDirection * acceration + force.direction * (force.magnitude / weight);
	acceration = accerationDirection.length();
	accerationDirection = accerationDirection / acceration;
}