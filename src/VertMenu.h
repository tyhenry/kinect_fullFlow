#pragma once
#include "ofMain.h"
#include "ofxCarousel.h"
#include "Button.h"

class VertMenu {

public:
	VertMenu() {}
	// display size
	VertMenu(float x, float y, float displayHeight, int numButtonsDisplay)
		: _x(x), _y(y), _h(displayHeight), _nBtnsDisplay(numButtonsDisplay)
	{}

	//void setPosition(float x, float y) {
	//	_x = x; _y = y;
	//}
	//void setDisplayWidth(float w) { _w = w; }
	//void setNumButtonsDisplay(int numButtons) {
	//	_nBtnsDisplay = numButtons;
	//}

	// add menu item
	int addButton(ofImage img); // adds from left to right
	void setScreenOverlay(ofImage overlay, ofVec2f pos, float w, float h) {
		_overlay = overlay; _bOverlay = true;
		_overlayBounds.set(pos, w, h);
	}
	int addStaticButton(ofImage img, ofVec2f pos, float w, float h);

	void setYPos(float y) { 
		_yOffset = y - _y; 		// calc y offset
	}

	int hover(ofVec2f pos); // returns index of button (-1 if none)
	int grab(ofVec2f pos);
	int release(ofVec2f pos);
	void resetState() {
		_hoverIdx = -1; _grabIdx = -1;
		_hoverStartTime = 0; _grabStartTime = 0;
	}

	float getHoverTime();
	float getGrabTime();
	int getHoverIdx() { return _hoverIdx; }

	void update();

	void drawScreenOverlay(ofVec2f pos, float w, float h);
	void drawScreenOverlay() { 
		drawScreenOverlay(_overlayBounds.getTopLeft(), _overlayBounds.width, _overlayBounds.height);
	}
	void drawMenu();
	void draw() {
		if (_bOverlay) drawScreenOverlay();
		drawMenu();
	}

	int hitTest(ofVec2f pos);

protected:
	ofxCarousel carousel;
	float _x, _y, _h;
	float _yOffset = 0;
	int _nBtnsDisplay;
	ofImage _overlay; bool _bOverlay = false;
	ofRectangle _overlayBounds;

	vector<Button> _buttons;
	int _hoverIdx = -1, _grabIdx = -1;
	float _hoverStartTime = 0, _grabStartTime = 0;
	ofVec2f _grabPos;
};