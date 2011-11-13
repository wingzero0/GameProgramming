#include "KeyboardControl.h"


KeyboardControl::KeyboardControl(void)
{
}


KeyboardControl::~KeyboardControl(void)
{
}

KeyboardControl::KeyboardControl(ACTORid character, OBJECTid camera){
	GameControl::GameControl(character, camera);
}