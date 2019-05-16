#pragma once
#include "ofMain.h"
#include "Force.h"


//Similar to Unity tranformation
//Record most position/rotation infor of a game object
//Also include simple physics calculation
class Transform {
public:
	//Current position
	glm::vec3 position;
	// The current object facing
	//In radians, relative to the Y axis
	float angle;

	//The parent of this tranform
	Transform* parent;

	//Some simple speed data
	//Determind how fast it turn, current speed, the drag that slow it down, and the acceration
	float turningSpeed = 0.0f;
	float speed = 0.0f;
	float acceration = 0.0f;
	glm::vec3 accerationDirection;
	float maxSpeed = 2.0f;
	float drag = 0.5f;
	float size = 32.0f;

	//Current movemenet direction
	//It's different from angle
	glm::vec3 direction = glm::vec3(0, -1, 0);
	glm::vec3 speedDirection = glm::vec3(0, -1, 0);

	//Return the position, adding all the parents' transformation
	glm::vec3 getPosition();
	//Return current angle, adding all the parents' roation

	//Whether it should ignore parents' rotation
	//Used for partical and turret that has independent rotation calculation
	bool indenpendentRotation = false;

	void update();
	void applyForce(float weight, ImpulseForce force);

	bool static collide(Transform a, Transform b) {
		if (glm::distance(a.position, b.position) <= (a.size + b.size))
			return true;
		else
			return false;
	}
};