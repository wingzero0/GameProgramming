#pragma once
#include "TheFlyWin32.h"
#include "ActorStateMachine.h"

class GameControl
{
//protected:
public:
	//ACTORid mainChar;
	ActorStateMachine *mainChar;
	OBJECTid camera;
	float dir[3];
public:
	GameControl(void);
	virtual ~GameControl(void);
	//GameControl(ACTORid character, OBJECTid camera);// need character, camera
	GameControl(ActorStateMachine * mainChar, OBJECTid camera);
	BOOL CharacterMoveForward();
	BOOL CharacterNormalAttack();
	BOOL CharacterSetIdle();
	int TurnToTargetDir(float degree = 5.0);
	BOOL isOnTargetDir();
	int GenerateTargetDir();
};

