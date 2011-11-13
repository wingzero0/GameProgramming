#pragma once
#include "gamecontrol.h"
class KeyboardControl :
	public GameControl
{
public:
	KeyboardControl(void);
	~KeyboardControl(void);
	KeyboardControl(ACTORid character, OBJECTid camera);
};

