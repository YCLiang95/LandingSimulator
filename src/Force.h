#pragma once
#include"ofMain.h"

class Force {

};

class ImpulseForce : public Force {
public:
	float magnitude;
	glm::vec3 direction;
};