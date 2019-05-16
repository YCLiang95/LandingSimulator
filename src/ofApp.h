#pragma once

#include "ofMain.h"
#include  "ofxAssimpModelLoader.h"
#include "box.h"
#include "ray.h"
#include "Octree.h"
#include "GameObject.h"
#include "Force.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		void drawAxis(ofVec3f);
		void initLightingAndMaterials();
		void savePicture();
		void toggleWireframeMode();
		void togglePointsDisplay();
		void toggleSelectTerrain();
		void setCameraTarget();
		int current_camera;

		bool mouseIntersectPlane(ofVec3f planePoint, ofVec3f planeNorm, ofVec3f &point);

		ofEasyCam cam;
		ofEasyCam cam2;
		ofEasyCam cam3;
		ofEasyCam cam4;
		vector<ofCamera*> cameras;
		ofxAssimpModelLoader mars, rocket;
		GameObject oRocket;
		ofLight light;
		//Box boundingBox;
		vector<Box> level1, level2, level3;
		Octree octree;
		ImpulseForce gravity;
		ImpulseForce rocket_up;
		ImpulseForce rocket_hor;

		ofxPanel gui;
		ofxLabel guispeed;
	
		bool bAltKeyDown;
		bool bCtrlKeyDown;
		bool bWireframe;
		bool bDisplayPoints;
		bool bPointSelected;
		
		bool bRocketLoaded;
		bool bTerrainSelected;
	
		ofVec3f selectedPoint;
		ofVec3f intersectPoint;

		const float selectionRange = 4.0;
};