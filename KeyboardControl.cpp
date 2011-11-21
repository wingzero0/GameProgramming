#include "KeyboardControl.h"

extern char debug[1024];

KeyboardControl::KeyboardControl(void)
{
}


KeyboardControl::~KeyboardControl(void)
{
}

KeyboardControl::KeyboardControl(ActorStateMachine * character, OBJECTid camera):GameControl(character,camera){

}
/*
KeyboardControl::KeyboardControl(ACTORid character, OBJECTid camera):GameControl(character, camera){

}
*/

int KeyboardControl::DirControl(){
	if (this->mainChar->CanBeControl() == FALSE){
		return -1;
	}

	int key;

	if (FyCheckHotKeyStatus(FY_W)){
		key = MOVE_FORWARD;
	}else if (FyCheckHotKeyStatus(FY_A)){
		key = MOVE_LEFT;
	}else if (FyCheckHotKeyStatus(FY_S)){
		key = MOVE_BACK;
	}else if (FyCheckHotKeyStatus(FY_D)){
		key = MOVE_RIGHT;
	}

	this->GenerateTargetDir(key);
	
	if (this->isOnTargetDir()){
		//sprintf(debug ,"in move\n");
		this->CharacterMoveForward(key);
	}else{
		int ret = this->TurnToTargetDir();
		//sprintf(debug ,"in turning: ret=%d\n", ret);
	}
	
	return 0;
}
int KeyboardControl::Command(){//the return value doesn't represent anything.
	if (FyCheckHotKeyStatus(FY_W) || FyCheckHotKeyStatus(FY_A) 
		|| FyCheckHotKeyStatus(FY_S) || FyCheckHotKeyStatus(FY_D)){
		this->DirControl();
	}else if(FyCheckHotKeyStatus(FY_J)) {
		this->CharacterNormalAttack();
	}else{
		this->CharacterSetIdle();
		sprintf(debug ,"in else condition\n");
	}


	return 0;
}

/*
int KeyboardControl::Command(WORLDid gID, BYTE code, BOOL value){
	if (code == FY_W){
		if (value){
			FnActor actor;
			actor.Object(this->mainChar);
			int ret = actor.MoveForward(1.0,TRUE, FALSE, 0.0, TRUE);
			sprintf(debug ,"%d FY_W ret = %d\n",this->mainChar, ret);
		}else{
			sprintf(debug ,"FY_W released\n");
		}
	}
	return 0;
}
*/
