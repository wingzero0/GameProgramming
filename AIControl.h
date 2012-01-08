#pragma once
//#include "GameControl.h"
#include "ActorStateMachine.h"
#include <vector>

#define MOVE_DISTANCE 5
#define KEEP_TRACK_DISTANCE 800
#define BOSS_KEEP_TRACK_DISTANCE 2000
#define ATTACK_DISTANCE 100
#define NPC_MOVE_OFFSET 150
#define GUARD_RATE 40


//class AIControl :
	//public GameControl
class AIControl
{
public:
	AIControl(ACTORid id);
	std::vector<ActorStateMachine *> npcStateMachineList;
	ActorStateMachine * bossStateMachine;
	AIControl(void);
	virtual ~AIControl(void);
	int AddNPC(ACTORid ncp, char * ActionFilename);
	int AddBossNPC(ACTORid ncp, char * ActionFilename);
	void PlayAction(int skip);
	void moveTowardLyubu();
private:
	ACTORid lyubuId;
	//ACTORid boss;
	std::vector<ActorStateMachine *> npcDropList;
};

