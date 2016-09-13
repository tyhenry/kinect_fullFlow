#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	ofBackground(0);
	width = ofGetWidth();
	height = ofGetHeight();
	float cropWidth = height*height / width;
	ofVec2f cropPos = ofVec2f(width*0.5 - cropWidth*0.5, 0);
	crop.set(cropPos, cropWidth, height); // for vertical screen

	kinect.open();
	kinect.init(true, true); // color, body
	user = User(kinect.getCoordinateMapper());

	hover.load("hover.png");
	grab.load("grab.png");

	setupMenus();
}

//--------------------------------------------------------------
void ofApp::update(){

	kinect.update();
	if (!kinect.hasColorStream()) return;

	bNewUser = user.setBody(kinect.getCentralBodyPtr());
	user.update();

	if (bUser && bNewUser) {
		menus.restart();
	}

	if (bUser) { // have user
		bool bRelease = false;
		// same user
		if (!bNewUser) {

			// if we're in attract menu, hover on either hand
			if (menus.getMenuName() == "attract") {
				ofVec2f rPos = user.getJoint2dPos(JointType_HandRight);
				ofVec2f lPos = user.getJoint2dPos(JointType_HandLeft);
				VertMenu* menu = menus.getMenuPtr("attract");
				int rHoverBtn = menu->hitTest(rPos);
				int lHoverBtn = menu->hitTest(lPos);
				if (rHoverBtn == 1) {
					bRighty = true;
					menus.hover(rPos);
				}
				else if (lHoverBtn == 0) {
					bRighty = false;
					menus.hover(lPos);
				}

				if (menus.isHoverSelect()) { // if we've hovered enough to select
					menus.next(); // next screen
				}
			}

			// if we're in wear mode, check hover time for back buttons
			else if (menus.getMenuName() == "wear") {
				// set hover pos
				JointType hand = bRighty ? JointType_HandRight : JointType_HandLeft;
				ofVec2f hPos = user.getJoint2dPos(hand);
				menus.hover(hPos);

				if (menus.isHoverSelect()) {
					VertMenu* menu = menus.getMenuPtr("wear");
					int idx = menu->getHoverIdx();
					switch (idx) {
					case 0:
						menus.goToMenu("cut");
						break;
					case 1:
						menus.goToMenu("category");
						break;
					case 2:
						menus.goToMenu("pattern");
						break;
					}
				}
			}

			// for other menus, get grab state
			else {
				HandState hState = bRighty ? user.getRightHandState() : user.getLeftHandState();
				// check for grab
				// --------------
				if (!bGrabbing) { // was not grabbing
					if (hState == HandState_Closed) {
						// closed hand to init new grab
						bGrabbing = true;
					}
				}
				else { // was grabbing
					if (hState != HandState_Unknown &&
						hState != HandState_Closed) {
						// must be closed or unknown to continue grab
						// otherwise, release
						bGrabbing = false;
						bRelease = true;
					}
				}

				// do menu interaction
				JointType hand = bRighty ? JointType_HandRight : JointType_HandLeft;
				ofVec2f hPos = user.getJoint2dPos(hand);
				if (bRelease) {
					menus.release(hPos);
				}
				else if (bGrabbing) {
					menus.grab(hPos);
				}
				else {
					menus.hover(hPos);
					if (menus.isHoverSelect()) {
						menus.next(); // next screen
					}
				}
			}
		}
		else { // new user, reset righty & grab state
			bRighty = true;
			bGrabbing = false;
		}

	}
	else { // no user, restart
		bGrabbing = false;
		bRighty = true;
		menus.restart();
	}

	menus.update();
}

//--------------------------------------------------------------
void ofApp::draw(){

	// draw stuff!

}

void ofApp::setupMenus(float topY, float height, int numButtons) {

	menus.clearMenus();

	ofRectangle closeRect(crop.getRight - 120, topY + height + 10, height/numButtons, height/numButtons);

	// 0 attract menu

	VertMenu menuAttract;
	menuAttract.setScreenOverlay(ofImage("introScreen.png"), crop.position, crop.width, crop.height);
	menuAttract.addStaticButton(ofImage(), ofVec2f(125, 375), 100, 125);
	menuAttract.addStaticButton(ofImage(), ofVec2f(400, 375), 100, 125);
	menus.addMenu(menuAttract,"attract");

	// 1 cut menu

	VertMenu menuCut(crop.getRight() - 120, topY, height, 3);
	for (int i = 0; i < 4; i++) {
		string imgPath = "shirtCut_" + ofToString(i) + ".png";
		menuCut.addButton(ofImage(imgPath));
	}
	menuCut.addStaticButton(ofImage("close.png"), closeRect.position, closeRect.width, closeRect.height);
	menus.addMenu(menuCut,"cut");

	// 2 category menu

	VertMenu menuCat(crop.getRight() - 120, topY, height, 3);
	for (int i = 0; i < 6; i++) {
		string imgPath = "cat_" + ofToString(i) + ".png";
		menuCat.addButton(ofImage(imgPath));
	}
	menuCat.addStaticButton(ofImage("close.png"), closeRect.position, closeRect.width, closeRect.height);
	menus.addMenu(menuCat,"category");

	// 3 pattern menu

	VertMenu menuPat(crop.getRight() - 120, topY, height, 3);
	for (int i = 5; i >= 0; i--) {
		string imgPath = "cat_" + ofToString(i) + ".png";
		menuPat.addButton(ofImage(imgPath));
	}
	menuPat.addStaticButton(ofImage("close.png"), closeRect.position, closeRect.width, closeRect.height);
	menus.addMenu(menuPat,"pattern");

	// 4 wear menu

	VertMenu menuWear(crop.getRight() - 120, topY, height, 3);
	menuWear.addButton(ofImage("shirtCut_2.png"));
	menuWear.addButton(ofImage("cat_0.png"));
	menuWear.addButton(ofImage("cat_5.png"));
	menus.addMenu(menuWear, "wear");
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

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
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
