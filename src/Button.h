#pragma once
#include "ofMain.h"

class Button {
public:
	Button(int index=0, ofImage bgImg=ofImage(), ofVec2f centerPos=ofVec2f(), float width=0, float height=0, bool isStatic=false, bool drawHighlight=true)
		: idx(index), bg(bgImg), center(centerPos), w(width), h(height), bStatic(isStatic), bDrawHighlight(drawHighlight) {
		origCenter = center; origW = w; origH = h;
		setBounds();
	}
	void setBounds() { setBounds(center, w, h); }
	void setBounds(ofVec2f centerPos, float width, float height) {
		center = centerPos; w = width; h = height;
		float boundW = width * 0.95;
		float boundH = height * 0.95;
		float boundX = center.x - boundW*0.5;
		float boundY = center.y - boundH*0.5;
		bounds = ofRectangle(boundX, boundY, boundW, boundH);
	}
	void setWidth(float width) {
		w = width; setBounds();
	}
	float getWidth() { return w; }
	void setHeight(float height) { 
		h = height; setBounds();
	}
	float getHeight() { return h; }
	void setIndex(int index) { idx = index;  }
	int getIndex() { return idx; }
	void setCenterPos(ofVec2f centerPos) {
		center = centerPos; setBounds();
	}
	ofVec2f getCenterPos() { return center; }
	void setTopLeftPos(ofVec2f topLeftPos) {
		setLeft(topLeftPos.x);
		setTop(topLeftPos.y);
	}
	ofVec2f getTopLeftPos() {
		ofVec2f tl;
		tl.x = center.x - w*0.5;
		tl.y = center.y - h*0.5;
		return tl;
	}
	void setLeft(float x) {
		origCenter = center; origW = w; origH = h;
		center.x = x + w*0.5; setBounds();
	}
	void setTop(float y) {
		origCenter = center; origW = w; origH = h;
		center.y = y + h*0.5; setBounds();
	}
	void resetPos() {
		center = origCenter; w = origW; h = origH;
		setBounds();
	}
	ofRectangle getBounds() { return bounds; }
	void setHovered(bool hov) {
		hovered = hov;
	}
	void hover() { hovered = true; }
	void noHover() { hovered = false; }
	bool isHovered() { return hovered; }
	bool isStatic() { return bStatic; }

	void draw() {
		if (hovered && bDrawHighlight) {
			ofPushStyle();
			ofSetColor(255);
			ofFill();
			ofDrawRectangle(bounds.x - 3, bounds.y - 3,
				bounds.width + 6, bounds.height + 6); // highlight
			ofPopStyle();
		}
		bg.draw(bounds);
		if (bStatic && bDrawHighlight) {
			ofPushStyle();
			ofNoFill();
			ofSetColor(0);
			ofSetLineWidth(5);
			ofDrawRectangle(bounds.x - 3, bounds.y - 3,
				bounds.width + 6, bounds.height + 6); // highlight
			ofPopStyle();
		}
	}
protected:
	int idx;
	ofImage bg;
	ofVec2f center; float w = 0, h = 0;
	ofVec2f origCenter; float origW = 0, origH = 0;
	ofRectangle bounds;
	bool hovered = false;
	bool bStatic = false;
	bool bDrawHighlight = true;
};