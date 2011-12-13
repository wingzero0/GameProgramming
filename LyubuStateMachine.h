#pragma once
#include "ActorStateMachine.h"
#include "KeyboardControl.h"
class LyubuStateMachine :
	public ActorStateMachine
{
public:
	LyubuStateMachine(void);
	virtual ~LyubuStateMachine(void);
	LyubuStateMachine(ACTORid character);
	//virtual BOOL PlayAction(int skip);
	virtual BOOL PlayAttackAction(int skip);
	/*
	BOOL isNowAttackState(void);	//if now is attack state(i.e. var attackState == true), return true
	void resetAttackState(void);	//reset var attackState
	void setAttackState(void);	//set var attackState
	*/
private:
	BOOL attackState;
};

