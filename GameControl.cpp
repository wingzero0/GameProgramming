#include "GameControl.h"
#include <math.h>

#define FLOAT_ERROR 0.001
#define MOVE_LENGTH 5.0
extern char debug[1024];

GameControl::GameControl(void)
{
}


GameControl::~GameControl(void)
{
}

GameControl::GameControl(ActorStateMachine * character, OBJECTid camera){
	this->camera = camera;
	this->mainChar = character;
}

BOOL GameControl::CharacterMoveForward(){
	FnActor actor;
	actor.Object(this->mainChar->character);
	int ret = actor.MoveForward(MOVE_LENGTH,TRUE, FALSE, 0.0, TRUE);
	this->mainChar->ChangeState(STATERUN);
	//sprintf(debug ,"%d FY_W ret = %d\n",this->mainChar->character, ret);
	return TRUE;
}

BOOL GameControl::CharacterNormalAttack(){
	if (this->mainChar->state == STATEBEATTACK){
		return FALSE;
	}
	this->mainChar->ChangeState(STATEATTACK);
	sprintf(debug ,"%d attacking\n",this->mainChar->character);
	return TRUE;
}

BOOL GameControl::CharacterSetIdle(){
	this->mainChar->ChangeState(STATEIDLE);
	return TRUE;
}

int GameControl::GenerateTargetDir(DIRECTION_CODE code){
	FnCamera cam;
	cam.Object(this->camera);
	float camPos[3];
	cam.GetWorldPosition(camPos);

	FnActor act;
	act.Object(this->mainChar->character);
	float actPos[3];
	act.GetWorldPosition(actPos);
	if (code == MOVE_FORWARD){
		for (int i = 0; i< 2;i++) {
			this->dir[i] = actPos[i] - camPos[i];
		}
		this->dir[2] = 0.0f;
		this->dir_normalize(this->dir);
	}else if (code == MOVE_BACK){
		for (int i = 0; i< 2;i++) {
			this->dir[i] = camPos[i] - actPos[i] ;
		}
		this->dir[2] = 0.0f;
		this->dir_normalize(this->dir);
	}else if (code == MOVE_LEFT || code == MOVE_RIGHT){
		this->CalcLeftRight(code);
	}
	return 0;
}

BOOL GameControl::isOnTargetDir(){
	FnActor act;
	act.Object(this->mainChar->character);
	float actFdir[3];
	float actUdir[3];
	act.GetWorldDirection(actFdir,actUdir);
	if ( (abs(this->dir[0] - actFdir[0]) > FLOAT_ERROR) || (abs(this->dir[1] - actFdir[1]) > FLOAT_ERROR) ){
		return FALSE;
	}else{
		return TRUE;
	}
}

int GameControl::TurnToTargetDir(float degree){
	// this function only works in the case that up direction of actor is in positive-z

	// if the gap bigger than 5 degrees, then turn 5 degrees
	// else set the facing dir to the target dir directly
	FnActor act;
	act.Object(this->mainChar->character);
	float actFdir[3];
	float actUdir[3];
	act.GetWorldDirection(actFdir,actUdir);

	if ( abs(actUdir[2] - 1.0f) > FLOAT_ERROR){
		// the actor is not z-up
		sprintf(debug ,"udir=%f\n", actUdir[2]);
		return -1;
	}
	// calculate the gap by arc cosine
	float dot = 0.0f;
	for (int i = 0;i<3;i++){
		dot += actFdir[i] * this->dir[i];
	}
#define PI 3.1415926
	if ( acos(dot) * 180 / PI < degree){
		//sprintf(debug, "%s small than degree(%f)\n", debug,degree);
		act.SetWorldDirection(this->dir, NULL);
		return 0;
	}

	float cross = actFdir[0] * this->dir[1] - actFdir[1] * this->dir[0];
	//sprintf(debug, "%sface:%f %f %f\n", debug,actFdir[0],actFdir[1],actFdir[2]);
	//sprintf(debug, "%straget:%f %f %f\n", debug, this->dir[0],this->dir[1],this->dir[2]);
	//sprintf(debug, "%scross:%f\n", debug, cross);
	if (cross > 0.0f){// up, turn left
		act.TurnRight(-degree);
	}else{
		act.TurnRight(degree);
	}
	return 0;
}

int GameControl::dir_normalize(float pos[3]){
	float sum = 0.0f;
	for (int i = 0; i< 3;i++) {
		sum += pos[i] * pos[i];
	}
	sum = sqrt(sum);
	if (sum == 0.0f){
		return -1;
	}
	for (int i = 0; i< 3;i++) {
		pos[i] = pos[i] / sum;
	}
	return 0;
}

int GameControl::CalcLeftRight(DIRECTION_CODE code){
	FnActor act;
	act.Object(this->mainChar->character);
	float aPos[3];
	act.GetWorldPosition(aPos);

	FnCamera cam;
	cam.Object(this->camera);
	float cPos[3];
	cam.GetWorldPosition(cPos);

	float orig[2];
	float newVector[2];
	float r = 0.0f;
	for (int i = 0;i<2;i++){
		orig[i] = aPos[i] - cPos[i];
		newVector[i] = orig[i];
		r += orig[i] * orig[i];
	}
	r = sqrt(r);

	float theta;
	theta = asin( MOVE_LENGTH / (2 * r) );
	theta *= 2;

	if (code == MOVE_LEFT){
		this->Rotate(theta, newVector);
	}else if(code == MOVE_RIGHT){
		this->Rotate(-theta, newVector);
	}

	for (int i = 0;i<2;i++){
		this->dir[i] = newVector[i] - orig[i];
	}
	this->dir[2] = 0.0f;

	this->dir_normalize(this->dir);
	//sprintf(debug, "%sdir:%f %f %f\n", debug, this->dir[0], this->dir[1], this->dir[2]);
	return 0;
}

int GameControl::Rotate(float theta, float vector[2]){
	// it will re-set vector
	float x;
	float y;
	x = cos(theta) * vector[0] - sin(theta) * vector[1]; 
	y = sin(theta) * vector[0] + cos(theta) * vector[1];
	vector[0] = x;
	vector[1] = y;
	//sprintf(debug, "%stheta:%f v0:%f v1:%f\n", debug, theta,vector[0], vector[1]);
	return 0;
}