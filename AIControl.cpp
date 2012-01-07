#include "AIControl.h"

using namespace std;
extern char debug[1024];

AIControl::AIControl(int id)
{
	this->lyubuId = id;
}


AIControl::~AIControl(void)
{
	for (int i = 0;i< this->npcStateMachineList.size(); i++){
		delete this->npcStateMachineList[i];
	}
}

int AIControl::AddNPC(ACTORid npc, char * ActionFilename){
	ActorStateMachine* stm = new ActorStateMachine(npc, ActionFilename);
	this->npcStateMachineList.push_back(stm);
	return 0;
}

void AIControl::PlayAction(int skip){
	for (int i = 0;i< this->npcStateMachineList.size(); i++){
		npcStateMachineList[i]->PlayAction(skip);
	}
}
float AIControl::distanceBetweenLyubu() {
	for (int i = 0;i< this->npcStateMachineList.size(); i++){
		int npcId = npcStateMachineList[i]->character;

		FnActor lyubu;
		FnActor npc;
		lyubu.Object(this->lyubuId);
		npc.Object(npcId);

		float lyubuPos[3];
		float npcPos[3];

		lyubu.GetWorldPosition(lyubuPos);
		npc.GetWorldPosition(npcPos);

		float distance;
		distance = sqrt((npcPos[0] - lyubuPos[0]) * (npcPos[0] - lyubuPos[0]) 
			+ (npcPos[0] - lyubuPos[0])	* (npcPos[0] - lyubuPos[0]) 
			+ (npcPos[0] - lyubuPos[0]) * (npcPos[0] - lyubuPos[0]));
		
		sprintf(debug, "%s distance = %f\n",debug,distance);
		
		return distance;

	}
}