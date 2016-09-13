#pragma once
#include "ofMain.h"
#include "ofxCarousel.h"
#include "Button.h"

class VertMenu {

public:
	// display size
	VertMenu(float x=0, float y=0, float displayHeight=0, int numButtonsDisplay=1)
		: _x(x), _y(y), _h(displayHeight), _nBtnsDisplay(numButtonsDisplay)
	{
		_w = _h / _nBtnsDisplay;
		_ofWidth = ofGetWidth(); _ofHeight = ofGetHeight();
		fbo.allocate(_ofWidth, _ofHeight, GL_RGBA);
	}

	//void setPosition(float x, float y) {
	//	_x = x; _y = y;
	//}
	//void setDisplayWidth(float w) { _w = w; }
	//void setNumButtonsDisplay(int numButtons) {
	//	_nBtnsDisplay = numButtons;
	//}

	// add menu item
	int addButton(ofImage img, bool drawHighlight = true); // adds from left to right
	void setScreenOverlay(ofImage overlay, ofVec2f pos, float w, float h) {
		_overlay = overlay; _bOverlay = true;
		_overlayBounds.set(pos, w, h);
	}
	int addStaticButton(ofImage img, ofVec2f pos, float w, float h, bool drawHighlight = true);

	void setYPos(float y) { 
		// calc y offset
		float yOffset = y - _y;
		for (int i = 0; i < _buttons.size(); i++) {
			if (_buttons[i].isStatic()) {
				float posY = _buttons[i].getTopLeftPos().y;
				_buttons[i].setTop(posY + yOffset);
			}
		}
		_y = y;
	}
	void setXPos(float x) {
		// calc x offset
		float xOffset = x - _x;
		for (int i = 0; i < _buttons.size(); i++) {
			float posX = _buttons[i].getTopLeftPos().x;
			_buttons[i].setLeft(posX + xOffset);
		}
		_x = x;
	}

	int hover(ofVec2f pos); // returns index of button (-1 if none)
	int grab(ofVec2f pos);
	int release(ofVec2f pos);
	void resetState() {
		_hoverIdx = -1; _grabIdx = -1;
		_hoverStartTime = 0; _grabStartTime = 0;
	}
	void resetPos() {
		for (auto& btn : _buttons) {
			btn.resetPos();
		}
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
	float _w = 0;
	float _yOffset = 0;
	int _nBtnsDisplay;
	ofImage _overlay; bool _bOverlay = false;
	ofRectangle _overlayBounds;

	vector<Button> _buttons;
	int _hoverIdx = -1, _grabIdx = -1;
	float _hoverStartTime = 0, _grabStartTime = 0;
	ofVec2f _grabPos;

	ofFbo fbo;
	float _ofWidth=0, _ofHeight=0;
};