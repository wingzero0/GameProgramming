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
int KeyboardControl::Command(){//the return value doesn't represent anything.
	//if (this->mainChar->CanBeControl() == FALSE){
		//return 0;
	//}
	if (FyCheckHotKeyStatus(FY_W)){
		this->CharacterMoveForward();
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
