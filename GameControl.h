#pragma once
#include "TheFlyWin32.h"
#include "ActorStateMachine.h"

#define MOVE_FORWARD 0
#define MOVE_BACK 1
#define MOVE_LEFT 2
#define MOVE_RIGHT 3

typedef int DIRECTION_CODE;

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
	GameControl(ActorStateMachine * mainChar, OBJECTid camera);

	virtual int DirControl() = 0;
	BOOL CharacterMoveForward();
	BOOL CharacterNormalAttack();
	BOOL CharacterSetIdle();
	int TurnToTargetDir(float degree = 30.0f);
	BOOL isOnTargetDir();
	int GenerateTargetDir(DIRECTION_CODE code);
private:
	int dir_normalize(float pos[3]);
};

