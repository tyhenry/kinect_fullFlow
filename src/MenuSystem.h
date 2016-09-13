#pragma once
#include "ofMain.h"
#include "HorzMenu.h"
#include "VertMenu.h"

class MenuSystem {

public:
	MenuSystem():
		_cMenu(0) {};
	int addMenu(VertMenu menu, string name); // returns num menus in system
	void clearMenus() {
		_menus.clear(); _menuNames.clear();
		_cMenu = 0; _hoverTime = 0;
	}

	void hover(ofVec2f pos);
	void grab(ofVec2f pos);
	void release(ofVec2f pos);
	void prev();
	void next();
	void restart();
	void update();
	void draw();

	void setTop(float y) {
		for (int i = 0; i < _menus.size(); i++) {
			if (_menuNames[i] != "attract")
				_menus[i].setYPos(y);
		}
	}
	void setLeft(float x) {
		for (int i = 0; i < _menus.size(); i++) {
			if (_menuNames[i] != "attract")
				_menus[i].setXPos(x);
		}
	}

	void resetAll(); // reset states of all menus
	void reset(int menuIdx);
	void reset() { reset(_cMenu); } // reset current menu

	int getMenuIndex() { return _cMenu; }
	string getMenuName() {
		if (exists(_cMenu)) return _menuNames[_cMenu];
		return "";
	}
	VertMenu* getMenuPtr(int menuIdx) {
		if (exists(menuIdx)) return &(_menus[menuIdx]);
		return nullptr;
	}
	VertMenu* getMenuPtr(string name) {
		int idx = getIndexFromName(name);
		return getMenuPtr(idx);
	}

	void goToMenu(int menuIdx);
	void goToMenu(string name);

	float getHoverTime() { return _hoverTime; }
	bool isHoverSelect() { return _hoverTime > _hoverWait; }

private:
	vector<VertMenu> _menus;
	vector<string> _menuNames;
	int _cMenu; // current menu
	float _hoverTime = 0; // time button has been hovered
	float _hoverWait = 3; // num secs until hover becomes select

	bool exists(int menuIdx){ 
		if (menuIdx >= 0 && menuIdx < _menus.size()) return true;
		return false;
	}
	int getIndexFromName(string name) {
		for (int i = 0; i < _menuNames.size(); i++) {
			if (name == _menuNames[i]) return i;
		}
		return -1;
	}
};