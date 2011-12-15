#include "GameControl.h"
#include <math.h>

#define FLOAT_ERROR 0.001
#define MOVE_LENGTH 10.0
#define CAM_MIN_HIGHT 115.0f
#define CAM_MAX_HIGHT 300.0f
extern char debug[1024];
extern OBJECTid tID;
//extern ACTORid lyubu;

BOOL firstBlock = TRUE;

GameControl::GameControl(void)
{
}


GameControl::~GameControl(void)
{
}

GameControl::GameControl(ActorStateMachine * character, OBJECTid camera){
	this->camera = camera;
	this->mainChar = character;
	this->CamInTop = FALSE;
}

BOOL GameControl::CharacterMoveForward(DIRECTION_CODE code){
	FnActor actor;
	actor.Object(this->mainChar->character);
	BOOL beBlock;
	int ret = actor.MoveForward(MOVE_LENGTH,TRUE, FALSE, 0.0, TRUE);
	this->mainChar->ChangeState(STATERUN);

	if (ret == 0){
		beBlock = FALSE;
	}else{
		beBlock = TRUE;
	}

	// please check "beBlock" and decide what to do.
	if (code == MOVE_FORWARD) {
		GameControl::CamFallow();
	}else if (code == MOVE_BACK) {
		GameControl::CamBackOff();
	}else if ((code == MOVE_LEFT || code == MOVE_RIGHT) && beBlock == TRUE){
		GameControl::CamRevolution(code);
	}
	
	if (ret != 0){ // ret will be -1 if the character hit the wall 
		return FALSE;
	}else{
		return TRUE;
	}
}

BOOL GameControl::AppendAttackCode(ATTACK_CODE code){
	return this->mainChar->AppendAttackCode(code);
}

BOOL GameControl::CharacterNormalAttack(){
	/*
	if (this->mainChar->state == STATEDAMAGE){
		return FALSE;
	}
	this->mainChar->ChangeState(STATEATTACK);
	sprintf(debug ,"%d attacking\n",this->mainChar->character);
	*/
	return TRUE;
}

/*
BOOL GameControl::CharacterSetIdle(){
	if (this->mainChar->CanBeControl){
		this->mainChar->ChangeState(STATEIDLE);
		return TRUE;
	}
	return TRUE;
}
*/

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

void GameControl::CalcLeftRight(DIRECTION_CODE code){
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

	float fDir[3], uDir[3];
	float tempFDir[3], tempUDir[3];
	
	cam.GetWorldDirection(fDir,uDir);

	tempFDir[0] = fDir[0];
	tempFDir[1] = fDir[1];
	tempFDir[2] = 0.0;
	tempUDir[0] = 0.0;
	tempUDir[1] = 0.0;
	tempUDir[2] = uDir[2];

	cam.SetWorldDirection(tempFDir,tempUDir);
	actor.GetWorldPosition(ly_pos);
	cam.GetWorldPosition(cam_pos);
	
	float dis = (cam_pos[0] - ly_pos[0]) * (cam_pos[0] - ly_pos[0]) + (cam_pos[1] - ly_pos[1]) * (cam_pos[1] - ly_pos[1]);
	//sprintf(debug, "%s distance between camera and lyubu is %f\n", debug, dis);
	if (dis > 129600) {
		//cam.MoveForward(MOVE_LENGTH,TRUE, FALSE, 0.0, FALSE);
		cam.MoveForward(MOVE_LENGTH,FALSE, FALSE, 0.0, FALSE);
	}
	
	BOOL flag;
	if (cam_pos[2] - MOVE_LENGTH / 2.0 > CAM_MIN_HIGHT){
		flag = cam.PutOnTerrain(tID,FALSE, cam_pos[2] - MOVE_LENGTH / 2.0);
	}else{
		this->CamInTop = FALSE;
		flag = cam.PutOnTerrain(tID,FALSE, CAM_MIN_HIGHT);
	}
	//BOOL flag = cam.PutOnTerrain(tID,FALSE,115.0);

	if (flag == FALSE) {
		sprintf(debug, "%s put on fail\n", debug);
	}
	
	fDir[2] = -0.2;
	uDir[1] = 0.2;
	cam.SetWorldDirection(fDir,uDir);
	
	firstBlock = TRUE;
}
void GameControl::CamBackOff() {
	FnObject cam;
	cam.Object(this->camera);
	

	float fDir[3], uDir[3];
	float tempFDir[3], tempUDir[3];
	
	cam.GetWorldDirection(fDir,uDir);

	tempFDir[0] = fDir[0];
	tempFDir[1] = fDir[1];
	tempFDir[2] = 0.0;
	tempUDir[0] = 0.0;
	tempUDir[1] = 0.0;
	tempUDir[2] = uDir[2];

	//sprintf(debug, "%s %f,%f,%f\n", debug, tempFDir[0],tempFDir[1], tempFDir[2]);
	//sprintf(debug, "%s %f,%f,%f\n", debug, tempUDir[0],tempUDir[1], tempUDir[2]);
	FnActor actor;
	actor.Object(this->mainChar->character);
	float ly_pos[3];
	float cam_pos[3];
	actor.GetWorldPosition(ly_pos);
	cam.GetWorldPosition(cam_pos);
	
	cam.SetWorldDirection(tempFDir,tempUDir);
	float dis = (cam_pos[0] - ly_pos[0]) * (cam_pos[0] - ly_pos[0]) + (cam_pos[1] - ly_pos[1]) * (cam_pos[1] - ly_pos[1]);
	int ret = 0;
	if (dis < 129600) {
		ret = cam.MoveForward(-MOVE_LENGTH,TRUE, FALSE, 0.0, TRUE);
	}

	BOOL flag;
	if (ret == 0){
		flag = cam.PutOnTerrain(tID,FALSE,CAM_MIN_HIGHT);
	}else{
		if (firstBlock == TRUE){
			cam.MoveForward(-0.1, FALSE, FALSE, 0.0, TRUE);	//prevent the actor be under the camera, or it can't go out there
			firstBlock = FALSE;
		}
		//sprintf(debug, "%s Camera is blocked\n", debug);
		float pos[3];
		cam.GetWorldPosition(pos);
		if (pos[2] > CAM_MAX_HIGHT) {
			flag = cam.PutOnTerrain(tID,FALSE,CAM_MAX_HIGHT);
			cam.GetWorldPosition(pos);
		}
		else {
			this->CamInTop = TRUE;
			flag = cam.PutOnTerrain(tID,FALSE,pos[2] + MOVE_LENGTH / 2);
		}
	}

	if (flag == FALSE) {
		sprintf(debug, "%s put on fail\n", debug);
	}
	
	fDir[2] = -0.2;
	uDir[1] = 0.2;
	cam.SetWorldDirection(fDir,uDir);

}

void GameControl::CamPointToActor() {
	sprintf(debug ,"in point to actor\n");

	float aPos[3], cPos[3];
	float aFDir[3], aUDir[3];
	float vec[3] = {0};	//vector point to actor from camera

	FnActor act;
	act.Object(this->mainChar->character);
	act.GetWorldPosition(aPos);
	
	act.GetWorldDirection(aFDir, aUDir);
	
	FnCamera cam;
	cam.Object(this->camera);
	cam.GetWorldPosition(cPos);
	
	/*
	float movedDistance = sqrt((aPos[0] - actorOldPosition[0]) * (aPos[0] - actorOldPosition[0])
		+ (aPos[1] - actorOldPosition[1]) * (aPos[1] - actorOldPosition[1])
		+ (aPos[2] - actorOldPosition[2]) * (aPos[0] - actorOldPosition[2]));
	sprintf(debug ,"%s\nmoved distance=%f\n", debug, movedDistance);
	sprintf(debug ,"%s\nold position: %f %f %f\n", debug, actorOldPosition[0], actorOldPosition[1], actorOldPosition[2]);
	sprintf(debug ,"%s\nnow position: %f %f %f\n", debug, aPos[0], aPos[1], aPos[2]);
	if (movedDistance < 15) {
		return;
	} else {
		this->actorOldPosition[0] = aPos[0];
		this->actorOldPosition[1] = aPos[1];
		this->actorOldPosition[2] = aPos[2];
	}
	*/
	


	vec[0] = aPos[0] - cPos[0];
	vec[1] = aPos[1] - cPos[1];
	vec[2] = (aPos[2] + 40) - cPos[2];	//the height of actor is about 40
	dir_normalize(vec);
	//vec[2] = -0.2;

	float cp[3] = {0};	//aUDir cross to vec
	cp[0] = aUDir[1] * vec[2] - aUDir[2] * vec[1];
	cp[1] = aUDir[2] * vec[0] - aUDir[0] * vec[2];
	cp[2] = aUDir[0] * vec[1] - aUDir[1] * vec[0];

	float cUDir[3];	//vec cross to cp
	cUDir[0] = vec[1] * cp[2] - vec[2] * cp[1];
	cUDir[1] = vec[2] * cp[0] - vec[0] * cp[2];
	cUDir[2] = vec[0] * cp[1] - vec[1] * cp[0];

	cam.SetWorldDirection(vec, cUDir);
}

int GameControl::CamRevolution(DIRECTION_CODE code){
	if (this->CamInTop == TRUE){
		return -1;
	}
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
		orig[i] = cPos[i] - aPos[i];
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

	float fDir[3];
	float uDir[3];
	for (int i = 0;i<2;i++){
		fDir[i] = newVector[i] - orig[i];
		uDir[i] = 0.0f;
	}
	fDir[2] = 0.0f;
	uDir[2] = 1.0f;

	this->dir_normalize(fDir);
	cam.SetWorldDirection(fDir,uDir);
	cam.MoveForward(MOVE_LENGTH,TRUE,FALSE,0.0f,TRUE);
	cam.PutOnTerrain(tID,FALSE,cPos[2]);
	return 0;
}

void GameControl::PlayAction(int skip){
	this->mainChar->PlayAction(skip);
	/*
	FnActor actor;
	actor.Object(this->mainChar->character);
	if (this->mainChar->CanBeControl() == TRUE){
		actor.Play(0,LOOP, (float)skip, FALSE,TRUE);
	}else if (this->state == STATEATTACK){
		this->
	}*/
	//return TRUE;
}