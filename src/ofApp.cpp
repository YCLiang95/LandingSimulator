
//--------------------------------------------------------------
//
//  Kevin M. Smith
//
//  Mars HiRise Project - startup scene
// 
//  This is an openFrameworks 3D scene that includes an EasyCam
//  and example 3D geometry which I have reconstructed from Mars
//  HiRis photographs taken the Mars Reconnaisance Orbiter
//
//  You will use this source file (and include file) as a starting point
//  to implement assignment 5  (Parts I and II)
//
//  Please do not modify any of the keymappings.  I would like 
//  the input interface to be the same for each student's 
//  work.  Please also add your name/date below.

//  Please document/comment all of your work !
//  Have Fun !!
//
//  Student Name:   < Your Name goes Here >
//  Date: <date of last version>


#include "ofApp.h"



//--------------------------------------------------------------
// setup scene, lighting, state and load geometry
//
void ofApp::setup(){

	current_camera = 0;
	bWireframe = false;
	bDisplayPoints = false;
	bAltKeyDown = false;
	bCtrlKeyDown = false;
	bRocketLoaded = false;
	bTerrainSelected = true;
//	ofSetWindowShape(1024, 768);
	cam.setPosition(25, 25, 0);
	//cam.setDistance(50);
	cam.setTarget(glm::vec3(0, 0, 0));
	cam.setNearClip(.1);
	cam.setFov(65.5);   // approx equivalent to 28mm in 35mm format
	ofSetVerticalSync(true);
	//cam.disableMouseInput();
	cam.disableMouseInput();
	cameras.push_back(&cam);
	cam2.setTarget(glm::vec3(0, 25, 0));
	cam2.setDistance(10);
	cam2.setNearClip(.1);
	cam2.setFov(65.5);
	cam2.enableMouseInput();
	cameras.push_back(&cam2);
	ofEnableSmoothing();
	ofEnableDepthTest();

	cam3.setFov(65.5);
	cam3.setNearClip(.1);
	cam3.setPosition(glm::vec3(0, 25, 0));
	cam3.setTarget(glm::vec3(0, 0, 0));
	cam3.setDistance(50);
	cam3.disableMouseInput();

	cam4.setFov(65.5);
	cam4.setNearClip(.1);
	cam4.setPosition(glm::vec3(0.5, 25, 0));
	cam4.setTarget(glm::vec3(0, 0, 0));
	cam4.disableMouseInput();
	

	cameras.push_back(&cam3);
	cameras.push_back(&cam4);
	gravity = ImpulseForce();
	gravity.magnitude = 0.5f;
	gravity.direction = glm::vec3(0, 1, 0);

	rocket_up = ImpulseForce();
	rocket_hor = ImpulseForce();
	rocket_up.magnitude = 1.5f;
	rocket_hor.magnitude = 1.0f;

	gui.setup();
	gui.add(guispeed.setup("Speed ", std::to_string(0)));
	gui.add(guiheight.setup("Height ", std::to_string(0)));
	gui.add(guispeed_warning.setup("", std::to_string(0)));

	// setup rudimentary lighting 
	//
	initLightingAndMaterials();

	mars.loadModel("geo/terrain_1.obj");
	mars.setScaleNormalization(false);
	rocket.loadModel("geo/rocket_1.obj");
	rocket.setScale(0.25, 0.25, 0.25);
	rocket.setScaleNormalization(false);
	bRocketLoaded = true;
	rocket.setPosition(0, -25, 0);
	oRocket = GameObject();
	oRocket.transform.position = glm::vec3(0, -25, 0);

	ofSetBackgroundColor(ofColor::black);

	octree.create(mars.getMesh(0), 10);

	ps = ParticleSystem();
	Particle p = Particle();
	emitter = ParticleEmitter(&ps, &p);
	emitter.transform.parent = &oRocket.transform;
}

//--------------------------------------------------------------
// incrementally update scene (animation)
//
void ofApp::update() {
	if (!bLanded) {
		cam2.setTarget(glm::vec3(-rocket.getPosition().x, -rocket.getPosition().y, rocket.getPosition().z));
		cam2.setDistance(10);
		cam2.setNearClip(.1);
		oRocket.transform.applyForce(10000, gravity);
		oRocket.transform.applyForce(10000, rocket_hor);
		oRocket.transform.applyForce(10000, rocket_up);
		oRocket.update();
		rocket.setPosition(oRocket.transform.position.x, oRocket.transform.position.y, oRocket.transform.position.z);
		cam4.setPosition(glm::vec3(-oRocket.transform.position.x, -oRocket.transform.position.y, oRocket.transform.position.z));
		cam4.setTarget(oRocket.transform.position + glm::vec3(0.5, -1000, 0));
		emitter.update();
		ps.update();
		float t = ofGetSystemTimeMillis();
		float height = 0;
		if (t - timeLastOctree > 500) {
			height_detection();
			height_line.clear();
			if (bPointSelected) {
				height_line.addVertex(-oRocket.transform.position.x, -oRocket.transform.position.y, oRocket.transform.position.z);
				height_line.addVertex(selectedPoint);
				height = (ofVec3f(-oRocket.transform.position.x, -oRocket.transform.position.y, oRocket.transform.position.z) - selectedPoint).length();
				guiheight = std::to_string(height);
			}

			if (height <= 0.1 && oRocket.transform.speed * oRocket.transform.speedDirection.y <= 0.01) {
				bLanded = true;
			}
			else if (height <= 0.1 && oRocket.transform.speed * oRocket.transform.speedDirection.y > 0.01) {
				//exploded
				bLanded = true;
			}

			guispeed = std::to_string(oRocket.transform.speed * oRocket.transform.speedDirection.y * 100);
			if (oRocket.transform.speed * oRocket.transform.speedDirection.y > 0.01)
				guispeed_warning = "Too Fast!";
			else
				guispeed_warning = "Good";
			timeLastOctree = t;
		}
	}
}

void ofApp::height_detection() {
	ofVec3f rayDir = ofVec3f(0, 1, 0);
	Ray ray = Ray(Vector3(-oRocket.transform.position.x, oRocket.transform.position.y, oRocket.transform.position.z),
		Vector3(-oRocket.transform.position.x, oRocket.transform.position.y + 1000, oRocket.transform.position.z));

	TreeNode node = TreeNode();
	if (octree.intersect(ray, octree.root, node)) {
		float distance = 0;

		//Search code from doPointSelection() function
		for (int i = 0; i < node.points.size(); i++) {
			ofVec3f point = cam.worldToCamera(mars.getMesh(0).getVertex(node.points[0]));
			float curDist = point.length();

			if (i == 0 || curDist < distance) {
				distance = curDist;
				selectedPoint = mars.getMesh(0).getVertex(node.points[0]);
			}
		}
		bPointSelected = true;
	}
	else bPointSelected = false;
}

//--------------------------------------------------------------
void ofApp::draw(){

//	ofBackgroundGradient(ofColor(20), ofColor(0));   // pick your own backgroujnd
	ofBackground(ofColor::black);
//	cout << ofGetFrameRate() << endl;

	ofEnableDepthTest();
	cameras[current_camera]->begin();
	ofPushMatrix();
	ofMultMatrix(mars.getModelMatrix());
	if (bWireframe) {                    // wireframe mode  (include axis)
		ofDisableLighting();
		ofSetColor(ofColor::slateGray);
		mars.drawWireframe();
		if (bRocketLoaded) {
			rocket.drawWireframe();
			if (!bTerrainSelected) drawAxis(rocket.getPosition());
		}
		if (bTerrainSelected) drawAxis(ofVec3f(0, 0, 0));
	}
	else {
		ofEnableLighting();              // shaded mode
		mars.drawFaces();

		if (bRocketLoaded) {
			rocket.drawFaces();
			if (!bTerrainSelected) drawAxis(rocket.getPosition());
		}
		if (bTerrainSelected) drawAxis(ofVec3f(0, 0, 0));
	}

	ps.draw();

	if (bDisplayPoints) {                // display points as an option    
		glPointSize(3);
		ofSetColor(ofColor::green);
		mars.drawVertices();
	}
	ofPopMatrix();
	ofPushMatrix();

	// highlight selected point (draw sphere around selected point)
	//
	if (bPointSelected) {
		ofSetColor(ofColor::blue);
		ofDrawSphere(selectedPoint, .1);
	}
	ofSetColor(ofColor::white);
	ofNoFill();
	//octree.drawLeafNodes(octree.root);
	//octree.draw(7, 0);
	height_line.draw();
	ofPopMatrix();
	cameras[current_camera]->end();
	ofDisableDepthTest();
	gui.draw();
}

// 

// Draw an XYZ axis in RGB at world (0,0,0) for reference.
//
void ofApp::drawAxis(ofVec3f location) {

	ofPushMatrix();
	ofTranslate(location);

	ofSetLineWidth(1.0);

	// X Axis
	ofSetColor(ofColor(255, 0, 0));
	ofDrawLine(ofPoint(0, 0, 0), ofPoint(1, 0, 0));
	

	// Y Axis
	ofSetColor(ofColor(0, 255, 0));
	ofDrawLine(ofPoint(0, 0, 0), ofPoint(0, 1, 0));

	// Z Axis
	ofSetColor(ofColor(0, 0, 255));
	ofDrawLine(ofPoint(0, 0, 0), ofPoint(0, 0, 1));

	ofPopMatrix();
}


void ofApp::keyPressed(int key) {

	switch (key) {
	case 'C':
	case 'c':
		current_camera++;
		if (current_camera == 4) current_camera = 0;
		break;
	case 'F':
	case 'f':
		ofToggleFullscreen();
		break;
	case 'H':
	case 'h':
		break;
	case 'r':
		cam.reset();
		break;
	case 't':
		setCameraTarget();
		break;
	case 'u':
		break;
	case 'v':
		togglePointsDisplay();
		break;
	case 'V':
		break;
	case 's':
		savePicture();
		break;
	case 'w':
		toggleWireframeMode();
		break;
	case OF_KEY_ALT:
		cam.enableMouseInput();
		bAltKeyDown = true;
		break;
	case OF_KEY_CONTROL:
		bCtrlKeyDown = true;
		break;
	case OF_KEY_SHIFT:
		break;
	case OF_KEY_DEL:
		break;
	case ' ':
		rocket_up.direction = glm::vec3(0, -1, 0);
		emitter.active = true;
		break;
	case OF_KEY_LEFT:
		rocket_hor.direction = glm::vec3(1, 0, 0);
		break;
	case OF_KEY_RIGHT:
		rocket_hor.direction = glm::vec3(-1, 0, 0);
		break;
	case OF_KEY_UP:
		rocket_hor.direction = glm::vec3(0, 0, -1);
		break;
	case OF_KEY_DOWN:
		rocket_hor.direction = glm::vec3(0, 0, 1);
		break;
	default:
		break;
	}
}

void ofApp::toggleWireframeMode() {
	bWireframe = !bWireframe;
}

void ofApp::toggleSelectTerrain() {
	bTerrainSelected = !bTerrainSelected;
}

void ofApp::togglePointsDisplay() {
	bDisplayPoints = !bDisplayPoints;
}

void ofApp::keyReleased(int key) {

	switch (key) {
	
	case OF_KEY_ALT:
		cam.disableMouseInput();
		bAltKeyDown = false;
		break;
	case OF_KEY_CONTROL:
		bCtrlKeyDown = false;
		break;
	case OF_KEY_SHIFT:
		break;
	case ' ':
		rocket_up.direction = glm::vec3(0, 0, 0);
		emitter.active = false;
		break;
	case OF_KEY_LEFT:
	case OF_KEY_RIGHT:
	case OF_KEY_UP:
	case OF_KEY_DOWN:
		rocket_hor.direction = glm::vec3(0, 0, 0);
		break;
	default:
		break;

	}
}



//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
}


//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}


//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {


}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

// Set the camera to use the selected point as it's new target
//  
void ofApp::setCameraTarget() {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}



//--------------------------------------------------------------
// setup basic ambient lighting in GL  (for now, enable just 1 light)
//
void ofApp::initLightingAndMaterials() {

	static float ambient[] =
	{ .5f, .5f, .5, 1.0f };
	static float diffuse[] =
	{ 1.0f, 1.0f, 1.0f, 1.0f };

	static float position[] =
	{5.0, 5.0, 5.0, 0.0 };

	static float lmodel_ambient[] =
	{ 1.0f, 1.0f, 1.0f, 1.0f };

	static float lmodel_twoside[] =
	{ GL_TRUE };


	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, position);

	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, position);


	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	glLightModelfv(GL_LIGHT_MODEL_TWO_SIDE, lmodel_twoside);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
//	glEnable(GL_LIGHT1);
	glShadeModel(GL_SMOOTH);
} 

void ofApp::savePicture() {
	ofImage picture;
	picture.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
	picture.save("screenshot.png");
	cout << "picture saved" << endl;
}

//--------------------------------------------------------------
//
// support drag-and-drop of model (.obj) file loading.  when
// model is dropped in viewport, place origin under cursor
//
void ofApp::dragEvent(ofDragInfo dragInfo) {
}

bool ofApp::mouseIntersectPlane(ofVec3f planePoint, ofVec3f planeNorm, ofVec3f &point) {
	glm::vec3 mouse(mouseX, mouseY, 0);
	ofVec3f rayPoint = cam.screenToWorld(mouse);
	ofVec3f rayDir = rayPoint - cam.getPosition();
	rayDir.normalize();
	return (rayIntersectPlane(rayPoint, rayDir, planePoint, planeNorm, point));
}