#include "GameControl.h"
#include <math.h>

#define FLOAT_ERROR 0.001
#define MOVE_LENGTH 10.0
extern char debug[1024];
extern OBJECTid tID;
//extern ACTORid lyubu;

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

BOOL GameControl::CharacterMoveForward(DIRECTION_CODE code){
	FnActor actor;
	actor.Object(this->mainChar->character);
	BOOL beBlock;
	int ret = actor.MoveForward(MOVE_LENGTH,TRUE, FALSE, 0.0, TRUE);
	this->mainChar->ChangeState(STATERUN);

	if (ret == 0){
		beBlock = TRUE;
	}else{
		beBlock = FALSE;
	}

	// please check "beBlock" and decide what to do.
	if (code == MOVE_FORWARD) {
		GameControl::CamFallow();
	}
	else if (code == MOVE_BACK) {
		GameControl::CamBackOff();
	}
	
	if (ret != 0){ // ret will be -1 if the character hit the wall 
		return FALSE;
	}else{
		return TRUE;
	}
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

<<<<<<< HEAD
float GameControl::GenerateTargetDir(DIRECTION_CODE code){
=======
int GameControl::GenerateTargetDir(DIRECTION_CODE code){
>>>>>>> gameprogramming/master
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
<<<<<<< HEAD
		return this->CalcLeftRight(code);
=======
		this->CalcLeftRight(code);
>>>>>>> gameprogramming/master
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

<<<<<<< HEAD
float GameControl::CalcLeftRight(DIRECTION_CODE code){
=======
int GameControl::CalcLeftRight(DIRECTION_CODE code){
>>>>>>> gameprogramming/master
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
<<<<<<< HEAD
	return theta;
=======
	return 0;
>>>>>>> gameprogramming/master
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

void GameControl::CamFallow() {
	float cam_pos[3], ly_pos[3];
	FnActor actor;
	actor.Object(this->mainChar->character);
	
	FnObject cam;
	cam.Object(this->camera);

	float fDir[3];
	float uDir[3];
	fDir[0] = 0.0;
	fDir[1] = 1.0;
	fDir[2] = 0.0;
	uDir[0] = 0.0;
	uDir[1] = 0.0;
	uDir[2] = 1.0;
	
	cam.SetWorldDirection(fDir,uDir);

	cam.MoveForward(MOVE_LENGTH,TRUE, FALSE, 0.0, TRUE);

	actor.GetWorldPosition(ly_pos);
	cam.GetWorldPosition(cam_pos);
	float dis = (cam_pos[0] - ly_pos[0]) * (cam_pos[0] - ly_pos[0]) + (cam_pos[1] - ly_pos[1]) * (cam_pos[1] - ly_pos[1]);
	//sprintf(debug, "%s distance between camera and lyubu is %f\n", debug, dis);

	if (dis < 129600) {
		cam.MoveForward(-MOVE_LENGTH,TRUE, FALSE, 0.0, TRUE);
	}
	
	BOOL flag = cam.PutOnTerrain(tID,FALSE,115.0);

	if (flag == FALSE) {
		sprintf(debug, "%s put on fail\n", debug);
	}
	
	fDir[2] = -0.2;
	uDir[1] = 0.2;
	cam.SetWorldDirection(fDir,uDir);

}
void GameControl::CamBackOff() {
	FnObject cam;
	cam.Object(this->camera);
	
	float fDir[3];
	float uDir[3];
	fDir[0] = 0.0;
	fDir[1] = 1.0;
	fDir[2] = 0.0;
	uDir[0] = 0.0;
	uDir[1] = 0.0;
	uDir[2] = 1.0;
	
	cam.SetWorldDirection(fDir,uDir);

	cam.MoveForward(-MOVE_LENGTH,TRUE, FALSE, 0.0, TRUE);
	
	BOOL flag = cam.PutOnTerrain(tID,FALSE,115.0);

	if (flag == FALSE) {
		sprintf(debug, "%s put on fail\n", debug);
	}
	
	fDir[2] = -0.2;
	uDir[1] = 0.2;
	cam.SetWorldDirection(fDir,uDir);

<<<<<<< HEAD
}
void GameControl::CamRotate(float theta, int key) {
	FnActor act;
	act.Object(this->mainChar->character);
	float aPos[3];
	act.GetWorldPosition(aPos);

	FnCamera cam;
	cam.Object(this->camera);
	float cPos[3];
	cam.GetWorldPosition(cPos);

	float vec[3];
	for (int i = 0;i<2;i++){
		vec[i] = aPos[i] - cPos[i];
	}
	sprintf(debug, "%s vec[0]=%f, vec[1]=%f, vec[2]=%f\n", debug, vec[0], vec[1], vec[2]);
	
	float oriFDir[3], oriUDir[3];
	cam.GetWorldDirection(oriFDir,oriUDir);
	sprintf(debug, "%s oriFDir[0]=%f, oriFDir[1]=%f, oriFDir[2]=%f\n", debug, oriFDir[0], oriFDir[1], oriFDir[2]);

	oriUDir[0] = 0;
	oriUDir[1] = 0;
	oriUDir[2] = 0;
	cam.SetDirection(vec, oriUDir);
	
	/*
	sprintf(debug, "%s theta is %f, and key is %d\n", debug, theta, key);

	FnObject cam;
	cam.Object(this->camera);
	
	float oriFDir[3], oriUDir[3];
	
	cam.GetWorldDirection(oriFDir,oriUDir);

	float fDir[3];
	float uDir[3];
	fDir[0] = oriFDir[0];
	fDir[1] = oriFDir[1];
	fDir[2] = 0.0;
	uDir[0] = oriUDir[0];
	uDir[1] = oriUDir[1];
	uDir[2] = oriUDir[2];

	cam.SetWorldDirection(fDir,uDir);
	
	cam.GetDirection(oriFDir,oriUDir);

	float fDir[3];
	float uDir[3];
	fDir[0] = oriFDir[0];
	fDir[1] = oriFDir[1];
	fDir[2] = 0.0;
	uDir[0] = oriUDir[0];
	uDir[1] = oriUDir[1];
	uDir[2] = 1.0;

	cam.SetDirection(fDir,uDir);
	
	cam.Rotate(Y_AXIS, theta * 180 / PI, LOCAL);
	
	float nowFDir[3], nowUDir[3];	
	cam.GetWorldDirection(nowFDir,nowUDir);
	fDir[0] = nowFDir[0];
	fDir[1] = nowFDir[1];
	fDir[2] = oriFDir[2];
	uDir[0] = nowUDir[0];
	uDir[1] = nowUDir[1];
	uDir[2] = oriUDir[2];
	
	cam.SetWorldDirection(fDir, uDir);
	*/
	
=======
>>>>>>> gameprogramming/master
}