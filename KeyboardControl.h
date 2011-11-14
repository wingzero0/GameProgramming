#pragma once
#include "gamecontrol.h"
class KeyboardControl :
	public GameControl
{
public:
	KeyboardControl(void);
	~KeyboardControl(void);
	KeyboardControl(ACTORid character, OBJECTid camera);
	int BlindMoveKey();// can't blind the function in the object method
	int Command(WORLDid gID, BYTE code, BOOL value);
	int Command();
};

