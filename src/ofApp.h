#pragma once

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "Util.h"
#include "box.h"
#include "ray.h"
#include "Octree.h"
#include "Force.h"
#include "ofxGui.h"
#include "ParticleEmitter.h"

//--------------------------------------------------------------
//Yecheng Liang
//Jiabao Qiu

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
		void height_detection();

		//camera
		void setup_camear();
		void update_camera();

		ofEasyCam cam;
		ofEasyCam cam2;
		ofEasyCam cam3;
		ofEasyCam cam4;
		vector<ofCamera*> cameras;
		ofxAssimpModelLoader mars, rocket;
		GameObject oRocket;
		Box rocketBox;
		ofLight light;
		//Box boundingBox;

		Octree octree;
		ImpulseForce gravity;
		ImpulseForce rocket_up;
		ImpulseForce rocket_hor;

		ofPolyline height_line;

		ofxPanel gui;
		ofxLabel guispeed;
		ofxLabel guiheight;
		ofxLabel guispeed_warning;
		ofxLabel Guid1;
		ofxLabel Gui_x;
		ofxLabel Gui_y;

	
		bool bAltKeyDown;
		bool bCtrlKeyDown;
		bool bWireframe;
		bool bDisplayPoints;
		bool bPointSelected;
		bool bLanded = false;
		
		bool bRocketLoaded;
		bool bTerrainSelected;
	
		ofVec3f selectedPoint;
		ofVec3f intersectPoint;

		const float selectionRange = 4.0;
		float timeLastOctree = 0;

		ParticleSystem ps;
		ParticleEmitter emitter;

		ofLight sunLight;
		ofLight spotLight;

		glm::vec3 landingArea1;
		glm::vec3 landingArea2;
		glm::vec3 landingArea3;

		ofImage background;

		float height = 999;
};
