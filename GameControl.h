#pragma once
#include "TheFlyWin32.h"

class GameControl
{
protected:
	ACTORid mainChar;
	OBJECTid camera;
	float dir[3];
public:
	GameControl(void);
	virtual ~GameControl(void);
	GameControl(ACTORid character, OBJECTid camera);// need character, camera
	BOOL CharacterMove();
	int TurnToTargetDir(float degree = 5.0);
	BOOL isOnTargetDir();
};

