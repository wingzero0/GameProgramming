#include "LyubuStateMachine.h"

extern char debug[1024];

LyubuStateMachine::LyubuStateMachine(void)
{
}


LyubuStateMachine::~LyubuStateMachine(void)
{
}

LyubuStateMachine::LyubuStateMachine(ACTORid character):ActorStateMachine(character){

}

BOOL LyubuStateMachine::PlayAction(int skip){
	FnActor actor;
	actor.Object(this->character);
	if (this->CanBeControl() == TRUE){
		actor.Play(0,LOOP, (float)skip, FALSE,TRUE);
	}else if (this->state == STATEATTACK){
		this->PlayAttackAction(skip);
	}
	return TRUE;
}

BOOL LyubuStateMachine::PlayAttackAction(int skip){
	FnActor actor;
	actor.Object(this->character);
	ACTIONid actionID;

	char attackName[20] = "\0";
	if (this->startAttack == TRUE){// first attack
		this->startAttack = FALSE; // reset
		sprintf(debug, "%sstart attack\n", debug);
		if (this->attackKeyQueue[currentAttackIndex] == NORMAL_ATT ){
			sprintf(attackName, "NormalAttack%d", currentAttackIndex + 1); 
			// attackName should be "NormalAttack1"
		}else if (this->attackKeyQueue[currentAttackIndex] == HEAVY_ATT){
			sprintf(attackName, "HeavyAttack%d", currentAttackIndex + 1); 
			// attackName should be "HeavyAttack1"
		}
	}else{
		// the attack name should be refine from reading the file
		if (actor.Play(0,ONCE, (float)skip, FALSE,TRUE) == FALSE){
			// play the next one
			currentAttackIndex++;
			if (currentAttackIndex >= lastAttackIndex){
				// finish attacking
				this->ChangeState(STATEIDLE);// should be change into combatidle;
				this->attackDisable = FALSE;
				currentAttackIndex = 0;
				lastAttackIndex = 0;
				return FALSE;
			}else if (this->attackKeyQueue[currentAttackIndex] == NORMAL_ATT){
				// get the next one attacking pos
				sprintf(attackName, "NormalAttack%d", currentAttackIndex + 1);
			}else if (this->attackKeyQueue[currentAttackIndex] == HEAVY_ATT){
				// get the next one attacking pos
				sprintf(attackName, "HeavyAttack%d", currentAttackIndex);
			}else{
				sprintf(debug, "%s next Attack fail condition\n", debug);
				return FALSE;
			}
		}else{
			return TRUE;
		}
	}
	actionID = actor.GetBodyAction(NULL,attackName);
	sprintf(debug, "%s geting pos %s lastAttack%d\n", debug, attackName, lastAttackIndex );
	if (actionID == FAILED_ID){
		sprintf(debug, "%s get %s fail\n", attackName);
		return FALSE;
	}
	if (actor.MakeCurrentAction(0,NULL,actionID) == FAILED_ID){
		sprintf(debug, "%s make current fail\n", debug);
		return FALSE;
	}

	if (actor.Play(0,START, 0.0, FALSE,TRUE) == FALSE){
		sprintf(debug, "%s play action failed\n", debug);
		return FALSE;
	}
	return TRUE;
}