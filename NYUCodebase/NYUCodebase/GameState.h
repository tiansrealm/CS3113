#pragma once

#include "GameState.h"
#include <string>

class GameState{
	friend class GameApp;
public:
	GameState(string& name);

private:
	std::string name;
	void Render();
	void Update(float elapsed);

}