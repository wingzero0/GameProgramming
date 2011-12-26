#include "BattleRoom.h"

using namespace std;
extern char debug[1024];

BattleRoom::BattleRoom(void)
{
}


BattleRoom::~BattleRoom(void)
{
}

BattleRoom::BattleRoom(ActorStateMachine *playerStateMachine, vector<ActorStateMachine *> npcStateMachineList){
	this->npcStateMachineList = npcStateMachineList;
	this->playerStateMachine = playerStateMachine;
	this->playerHitMap.clear();
}

void BattleRoom::RefreshArena(){
	FnActor actor;
	FnActor npc;
	actor.Object(this->playerStateMachine->character);
	
	float playerPos[3];
	float npcPos[3];
	actor.GetWorldPosition(playerPos);

	this->AreanList.clear();
	for (int i =0;i< this->npcStateMachineList.size();i++){
		npc.Object(this->npcStateMachineList[i]->character);
		npc.GetWorldPosition(npcPos);
		
		if (this->CheckDistanceAndState(playerPos, npcPos, 
				this->playerStateMachine->state, this->npcStateMachineList[i]->state ) == TRUE){
			this->JoinArena( this->npcStateMachineList[i] );
			this->npcStateMachineList[i]->ChangeState(STATEATTACK,TRUE);
		}
	}
	//sprintf(debug, "%s npc state machine list size = %d\n",debug,this->npcStateMachineList.size());
	this->PerformAttack();
}

BOOL BattleRoom::CheckDistanceAndState(float playerPos[3], float npcPos[3], ActorState playerState,ActorState npcState ){
	if (playerState == STATEATTACK || npcState == STATEATTACK){
		float dist = 0.0;
		for (int i = 0;i< 3;i++){
			dist += (playerPos[i] - npcPos[i]) * (playerPos[i] - npcPos[i]);
		}
		if (dist < BATTLE_RADIUS){
			//sprintf(debug, "%s dist = %f\n",debug,dist);
			return TRUE;
		}
	}
	return FALSE;
}

BOOL BattleRoom::JoinArena(ActorStateMachine * npcStm){
	this->AreanList.push_back(npcStm);
	return TRUE;
}

void BattleRoom::PerformAttack(){
	if (this->AreanList.empty() == true){// no npc in the arean means that no attack exists.
		return;
	}
	if (playerStateMachine->state == STATEATTACK && playerStateMachine->effectiveAttack == TRUE ){
		if (this->playerStateMachine->newAttack == TRUE){
			this->playerStateMachine->newAttack = FALSE;
			this->playerHitMap.clear();
		}
		FnActor actor;
		actor.Object(this->playerStateMachine->character);
		float pPos[3];
		float pDir[3];
		//float pUDir[3];
		actor.GetWorldDirection(pDir,NULL);
		actor.GetWorldPosition(pPos);

		FnActor npc;
		float nPos[3];
		
		for (int i= 0;i< this->AreanList.size();i++){
			ACTORid tmpid = AreanList[i]->character;
			if (playerHitMap.find(tmpid) == playerHitMap.end()){
				npc.Object(tmpid);
				npc.GetWorldPosition(nPos);
				if (this->AttackCheck(pDir, pPos, nPos, BATTLE_RADIUS / 1.5) == TRUE){// lyubu attack area is the half of Battle raidus
					// get a new victim;
					sprintf(debug, "%s new victim\n",debug);
					if ( this->AreanList[i]->state != STATEDIE){
						this->AreanList[i]->ChangeState(STATEDAMAGE,TRUE);
					}
				}
				this->playerHitMap[tmpid] = TRUE;
			}
		}
	}// should check the npc's attack;
}

BOOL BattleRoom::AttackCheck(float attackerDir[3], float attackerPos[3], float vitimPos[3], float attackRange){
	float dist = 0.0;
	for (int i = 0;i< 3;i++){
		dist += (attackerPos[i] - vitimPos[i]) * (attackerPos[i] - vitimPos[i]);
	}
	//sprintf(debug, "%s dist = %lf\n",debug,dist);
	if ( dist >= attackRange){
		return FALSE;
	}
	float cosine,dotProduct;
	//float v[3];
	dotProduct = 0.0;
	for (int i = 0;i< 3;i++){
		dotProduct += (vitimPos[i] - attackerPos[i]) * attackerDir[i];
	}
	float length = 0.0;
	for (int i = 0;i< 3;i++){
		length += (vitimPos[i] - attackerPos[i])* (vitimPos[i] - attackerPos[i]);
	}
	cosine = dotProduct / length;
	//sprintf(debug, "%s cosine = %lf\n",debug,cosine);
	if (cosine >= 0){
		return TRUE;
	}else{
		return FALSE;
	}
}