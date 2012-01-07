#pragma once
//#include "GameControl.h"
#include "ActorStateMachine.h"
#include <vector>

#define MOVE_DISTANCE 5
#define KEEP_TRACK_DISTANCE 1500
#define ATTACK_DISTANCE 100
#define NPC_MOVE_OFFSET 100

//class AIControl :
	//public GameControl
class AIControl
{
public:
	AIControl(int);
	std::vector<ActorStateMachine *> npcStateMachineList;
	AIControl(void);
	virtual ~AIControl(void);
	int AddNPC(ACTORid ncp, char * ActionFilename);
	void PlayAction(int skip);
	void moveTowardLyubu();
private:
	int lyubuId;
};

