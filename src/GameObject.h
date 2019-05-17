#pragma once

#include "ofMain.h"
#include "Transform.h"

//Yecheng Liang
//code reused from 2D vintage shooter

//Similar to unity, game objects are objects that can be placed in to the world
//It's the base class, for now it's mostly empty
class GameObject {

public:
	int id;

	Transform transform;

	//void virtual move(glm::vec3);
	void virtual update();
	virtual void draw();
	GameObject();
	~GameObject() {};
	bool isDead = false;

	virtual GameObject* clone() { 
		GameObject* result = new GameObject(*this);
		return result;
	};

	//Some event pass in just for override
	void virtual keyPressed(int key) {};
	void virtual keyReleased(int key) {};
	void virtual mouseMoved(int x, int y) {};
	void virtual mouseDragged(int x, int y, int button) {};
	void virtual mousePressed(int x, int y, int button) {};
	void virtual mouseReleased(int x, int y, int button) {};
};