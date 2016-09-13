#pragma once

#include "ofMain.h"
#include "ofxKinectForWindows2.h"
#include "Kinect.h"
#include "User.h"
#include "VertScreen.h"
#include "MenuSystem.h"
#include "VertMenu.h"
#include "Button.h"

class ofApp : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();

	void setupMenus(float topY=100, float height=300, int numButtons=3);

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

	Kinect kinect;
	User user;

	VertScreen vertScreen;
	float width = 1920;
	float height = 1080;
	ofRectangle crop;

	MenuSystem menus;

	ofImage hover;
	ofImage grab;

	bool bUser = false; // has a user?
	bool bNewUser = false; // new user?
	bool bGrabbing = false; // user grabbing?

	bool bDrawUser = true;
	bool bDrawColor = false;
	bool bResetMenus = true;

	bool bRighty = true;

};
