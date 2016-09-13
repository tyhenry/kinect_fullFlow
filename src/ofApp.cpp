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

	hover.load(ofToDataPath("hover.png"));
	grab.load(ofToDataPath("grab.png"));

	setupMenus();
}

//--------------------------------------------------------------
void ofApp::update(){

	kinect.update();
	if (!kinect.hasColorStream()) return;

	bNewUser = user.setBody(kinect.getCentralBodyPtr());
	user.update();
	bUser = user.hasBody();

	if (bUser && bNewUser && bResetMenus) {
		menus.restart();
		menus.getMenuPtr("attract")->resetPos();
	}

	if (bUser) { // have user
		bool bRelease = false;
		// change menu height to match body position
		if (menus.getMenuName() != "attract")
			menus.setTop(user.getJoint2dPos(JointType_Head).y);

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
					// if lefty, move menus to left side
					if (!bRighty) {
						menus.setLeft(crop.getLeft() + 20);
					}
					else {
						menus.setLeft(crop.getRight() - 120);
					}
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
		if (bResetMenus) {
			menus.restart();
			menus.getMenuPtr("attract")->resetPos();
		}
	}

	menus.update();
}

//--------------------------------------------------------------
void ofApp::draw(){

	vertScreen.begin();
	{
		// user
		if (bDrawUser) {
			user.draw();
		}

		// menu
		menus.draw();

		// hand icons
		if (bUser) {

			// get hand position
			JointType hand = bRighty ? JointType_HandRight : JointType_HandLeft;
			ofVec2f handCPos = user.getJoint2dPos(hand);
			ofRectangle handRect;
			if (bRighty) {
				handRect.set(handCPos.x - 50, handCPos.y - 50, 100, 100);
			}
			else {
				handRect.set(handCPos.x + 50, handCPos.y - 50, -100, 100);
			}

			// grab
			if (bGrabbing) {
				grab.draw(handRect);
			}
			// or hover
			else {
				float hoverTime = menus.getHoverTime();
				// draw circle for hover timer
				hover.draw(handRect);
				if (hoverTime > 0.5) {
					// draw growing arc
					ofPushStyle();
					// arc bg
					float radius = handRect.getHeight()*0.75f;
					ofSetColor(0);
					ofSetLineWidth(15);
					ofPolyline timerbg;
					timerbg.arc(handCPos, radius, radius, 0, 360, 60);
					timerbg.draw();
					// arc
					ofSetColor(255);
					ofSetLineWidth(10);
					ofPolyline timer;
					float angleEnd = ofMap(hoverTime, 0.5, 3.0, 1, 360, true);
					timer.arc(handCPos, radius, radius, 0, angleEnd, 60);
					timer.draw();
					ofPopStyle();
				}
			}
			if (menus.getMenuName() == "attract") {
				// draw other hand
				JointType oHand = bRighty ? JointType_HandLeft : JointType_HandRight;
				ofVec2f oHandCPos = user.getJoint2dPos(oHand);
				ofRectangle oHandRect;
				if (!bRighty) {
					oHandRect.set(oHandCPos.x - 50, oHandCPos.y - 50, 100, 100);
				}
				else {
					oHandRect.set(oHandCPos.x + 50, oHandCPos.y - 50, -100, 100);
				}
				hover.draw(oHandRect);
			}
		}
	}
	vertScreen.end();

}

void ofApp::setupMenus(float topY, float height, int numButtons) {

	menus.clearMenus();

	ofRectangle closeRect(crop.getRight() - 120, topY + height + 15, height/numButtons, height/numButtons);

	// 0 attract menu

	VertMenu menuAttract;
	menuAttract.setScreenOverlay(ofImage("introScreen.png"), crop.position, crop.width, crop.height);
	menuAttract.addStaticButton(ofImage(), ofVec2f(crop.getLeft()+125, 375), 100, 125, false);
	menuAttract.addStaticButton(ofImage(), ofVec2f(crop.getLeft()+400, 375), 100, 125, false);
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
	if (key == 'v' || key == 'V') {
		vertScreen.nextScreenMode();
	}
	else if (key == 'u' || key == 'U') {
		bDrawUser = !bDrawUser;
	}
	else if (key == OF_KEY_LEFT) {
		// page menu back
		menus.prev();
	}
	else if (key == OF_KEY_RIGHT) {
		// page menu forward
		menus.next();
	}
	else if (key == 'r' || key == 'R') {
		bResetMenus = !bResetMenus;
	}
	else if (key == 'c' || key == 'C') {
		bDrawColor = !bDrawColor;
	}

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
