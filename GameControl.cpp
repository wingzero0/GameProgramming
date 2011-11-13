#include "GameControl.h"


GameControl::GameControl(void)
{
}


GameControl::~GameControl(void)
{
}

GameControl::GameControl(ACTORid character, OBJECTid camera){
	this->camera = camera;
	this->mainChar = character;
}