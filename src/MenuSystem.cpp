#include "MenuSystem.h"

int MenuSystem::addMenu(VertMenu menu, string name) {
	_menus.push_back(menu);
	_menuNames.push_back(name);
	return _menus.size();
}

void MenuSystem::hover(ofVec2f pos) {
	if (exists(_cMenu)){
		_menus[_cMenu].hover(pos);
		_hoverTime = _menus[_cMenu].getHoverTime();
	}
}

void MenuSystem::grab(ofVec2f pos) {
	if (exists(_cMenu)) {
		_menus[_cMenu].grab(pos);
	}
}

void MenuSystem::release(ofVec2f pos) {
	if (exists(_cMenu)) {
		_menus[_cMenu].release(pos);
	}
}

void MenuSystem::prev() {
	if (_cMenu > 0) {
		resetAll();
		_cMenu--;
	}
}

void MenuSystem::next() {
	if (_cMenu < _menus.size()) {
		resetAll();
		_cMenu++;
	}
}

void MenuSystem::restart() {
	resetAll();
	_cMenu = 0;
}

void MenuSystem::update() {
	if (exists(_cMenu)) {
		_menus[_cMenu].update();
	}
}

void MenuSystem::draw() {
	if (exists(_cMenu)) {
		_menus[_cMenu].draw();
	}
	else {
		cout << "drawing menu " << _cMenu << " that doesn't exist!" << endl;
	}
}

void MenuSystem::resetAll() {
	for (int i = 0; i < _menus.size(); i++) {
		_menus[i].resetState();
	}
}
void MenuSystem::reset(int menuIdx) {
	if (exists(menuIdx)) _menus[menuIdx].resetState();
}

void MenuSystem::goToMenu(int menuIdx) {
	if (exists(_cMenu)) {
		resetAll();
		_cMenu = menuIdx;
	}
}
void MenuSystem::goToMenu(string name) {
	int idx = getIndexFromName(name);
	if (idx != -1) {
		resetAll();
		_cMenu = idx;
	}
}