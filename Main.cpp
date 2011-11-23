/*==============================================================
This is a demo program for TheFly System

Hello !

(C)2004 Chuan-Chang Wang, All Rights Reserved
Created : 0303, 2004, C. Wang

Last Updated : 1010, 2004, C.Wang
===============================================================*/
#include "TheFlyWin32.h"
#include "KeyboardControl.h"
#include "ActorStateMachine.h"

int oldX, oldY, oldXM, oldYM, oldXMM, oldYMM;

WORLDid gID = FAILED_ID;
VIEWPORTid vID;
SCENEid sID;
OBJECTid nID, cID, lID;
OBJECTid tID;
ACTORid lyubu;
KeyboardControl *kc;

char debug[1024] = "\0";
char loopBuff[1024] = "\0";
void debug_message(char*, char*);

void QuitGame(WORLDid, BYTE, BOOL);
BOOL initLyubu();
void Reset(WORLDid gID, BYTE code, BOOL value);
void PlayAction(int skip);
void GameAI(int);
void Render(int);
void GetPosDetail(char *);
BOOL BlindKeys();
void KeyboardCommand(WORLDid gID, BYTE code, BOOL value);

void InitPivot(WORLDid, int, int);
void PivotCam(WORLDid, int, int);
void InitMove(WORLDid, int, int);
void MoveCam(WORLDid, int, int);
void InitZoom(WORLDid, int, int);
void ZoomCam(WORLDid, int, int);

/*------------------
the main program
C.Wang 0308, 2004
-------------------*/
void main(int argc, char **argv)
{
	// create a new world
	gID = FyWin32CreateWorld("GameProgramming 2011 !", 0, 0, 800, 600, 32, FALSE);

	FnWorld gw;
	gw.Object(gID);
	gw.SetObjectPath("Data\\NTU4\\Scenes"); // for terrain loading
	gw.SetScenePath("Data\\NTU4\\Scenes"); // for scene loading
	gw.SetTexturePath("Data\\NTU4\\Scenes\\Textures");
	gw.SetShaderPath("Data\\NTU4\\Shaders");
	
	// create a viewport
	vID = gw.CreateViewport(0, 0, 800, 600);
	FnViewport vp;
	vp.Object(vID);
	vp.SetBackgroundColor(0.3f, 0.4f, 0.5f);

	// create a 3D scene & the 3D entities
	sID = gw.CreateScene(10);

	FnScene scene;
	scene.Object(sID);
	BOOL flag;
	flag = scene.Load("scene2");
	if (flag == FALSE){
		sprintf(debug, "%s scene load fail\n", debug);
	}
	nID = scene.CreateObject(ROOT);
	cID = scene.CreateCamera(ROOT);
	lID = scene.CreateLight(ROOT);

	// create terrain
	tID = scene.CreateTerrain(ROOT);
	if (tID == FAILED_ID){
		exit(-1);
	}
	FnTerrain terrain;
	terrain.Object(tID);
	flag = terrain.Load("terrain");
	if (flag == FALSE){
		sprintf(debug, "%s ground load fail\n", debug);
	}
	flag = terrain.GenerateTerrainData();
	if (flag == FALSE){
		sprintf(debug, "%s ground generate fail\n", debug);
		//exit(-1);
	}
	
	// translate the camera
	FnCamera camera;
	camera.Object(cID);
	camera.Rotate(X_AXIS, 90.0f, LOCAL);
	camera.Translate(0.0f, 10.0f, 100.0f, LOCAL);
	
	initLyubu();

	// translate the light
	FnLight light;
	light.Object(lID);
	light.SetName("MainLight");
	light.Translate(-50.0f, -50.0f, 50.0f, GLOBAL);

	// set Hotkeys
	BlindKeys();
	
	/* bind a timer, frame rate = 30 fps */
	FyBindTimer(0, 30.0f, GameAI, TRUE);	
	FyBindTimer(1, 30.0f, Render, TRUE);	

	// invoke the system
	FyInvokeTheFly(TRUE);
}

BOOL initLyubu(){ // init Lyubu and Camera
	FnWorld gw;
	// create actor:lyubu
	gw.Object(gID);
	gw.SetTexturePath("Data\\NTU4\\Characters");
	gw.SetCharacterPath("Data\\NTU4\\Characters");

	FnScene scene;
	scene.Object(sID);
	lyubu = scene.LoadActor("Lyubu");
	if (lyubu == FAILED_ID){
		return FALSE;
	}
	FnActor actor;
	actor.Object(lyubu);
	float pos[3];
	pos[0] = 3569.0;
	pos[1] = -3210;
	pos[2] = 100;
	actor.SetPosition(pos);
	
	//flag = actor.PutOnTerrain(tID,FALSE,0.0,10.0,10.0,30.0,100.0);
	BOOL flag = actor.PutOnTerrain(tID,FALSE,0.0);

	if (flag == FALSE){
		sprintf(debug, "%s put on fail\n", debug);
		return FALSE;
	}
	// set lyubu idle action
	ACTIONid idleID = actor.GetBodyAction(NULL,"IDLE");
	//ACTIONid idleID = actor.GetBodyAction(NULL,"WALK");
	if (idleID == FAILED_ID){
		sprintf(debug, "%s get action failed\n", debug);
		return FALSE;
	}else{
		sprintf(debug, "%s get action success\n", debug);
	}
	
	//actor.MakeCurrentAction(0,NULL,idleID,0.0,TRUE);
	//if (actor.MakeCurrentAction(0,NULL,FAILED_ID) == FAILED_ID){
	if (actor.MakeCurrentAction(0,NULL,idleID) == FAILED_ID){
		sprintf(debug, "%s make current fail\n", debug);
	}else{
		sprintf(debug, "%s make action success\n", debug);
	}
	
	if (actor.Play(0,START, 0.0, FALSE,TRUE) == FALSE){
		sprintf(debug, "%s play action failed\n", debug);
	}

	FnCamera camera;
	camera.Object(cID);
	pos[0] = 3569.0;
	pos[1] = -3570;
	pos[2] = 115.0;
	camera.SetWorldPosition(pos);

	float fDir[3];
	float uDir[3];
	fDir[0] = 0.0;
	fDir[1] = 1.0;
	fDir[2] = -0.2;
	uDir[0] = 0.0;
	uDir[1] = 0.2;
	uDir[2] = 1.0;
	
	camera.SetWorldDirection(fDir,uDir);
	sprintf(debug, "%sface:%f %f %f\n", debug,fDir[0],fDir[1],fDir[2]);
	sprintf(debug, "%sup:%f %f %f\n", debug, uDir[0],uDir[1],uDir[2]);


	fDir[2] = 0.0f;
	uDir[1] = 0.0f;
	actor.SetWorldDirection(fDir,uDir);


	//kc = new KeyboardControl(lyubu, cID);
	ActorStateMachine * lyubuState = new ActorStateMachine(lyubu);
	kc = new KeyboardControl(lyubuState, cID);
	sprintf(debug, "%sactorID=%d cID=%d",debug, lyubu, cID);
	
	return TRUE;
}

void Reset(WORLDid gID, BYTE code, BOOL value){
	if (code == FY_F1) {
		if (value) {
			FnScene scene;
			scene.Object(sID);
			scene.DeleteActor(lyubu);
			debug[0] = '\0';
			ActorStateMachine * lyubuState = kc->mainChar;
			delete lyubuState;
			delete kc;
			initLyubu();
		}
	}
	
}
void PlayAction(int skip){
	FnActor actor;
	actor.Object(lyubu);
	
	if (actor.Play(0,LOOP, (float)skip, FALSE,TRUE) == FALSE){
		//sprintf(debug, "%s play action failed\n", debug);
	}else{
		//sprintf(debug, "%s played\n", debug);
	}
}

void GetPosDetail(char *buffer){
	FnCamera camera;
	camera.Object(cID);
	float fDir[3];
	float uDir[3];
	float pos[3];
	camera.GetWorldPosition(pos);
	camera.GetWorldDirection(fDir, uDir);
	sprintf(buffer, "carmer pos[0] = %f,pos[1] = %f,pos[2] = %f \n", pos[0],pos[1],pos[2]);
	sprintf(buffer, "%scarmer fDir[0] = %f,fDir[1] = %f,fDir[2] = %f \n", buffer, fDir[0],fDir[1],fDir[2]);
	sprintf(buffer, "%scarmer uDir[0] = %f,uDir[1] = %f,uDir[2] = %f \n", buffer, fDir[0],fDir[1],fDir[2]);
	/*
	FnActor actor;
	actor.Object(lyubu);
	actor.GetWorldPosition(pos);
	sprintf(buffer, "lyubu pos[0] = %f,pos[1] = %f,pos[2] = %f \n", pos[0],pos[1],pos[2]);	
	*/
}

BOOL BlindKeys(){
	FyDefineHotKey(FY_ESCAPE, QuitGame, FALSE);
	FyDefineHotKey(FY_F1, Reset, FALSE);

	// define some mouse functions
	FyBindMouseFunction(LEFT_MOUSE, InitPivot, PivotCam, NULL, NULL);
	FyBindMouseFunction(MIDDLE_MOUSE, InitZoom, ZoomCam, NULL, NULL);
	FyBindMouseFunction(RIGHT_MOUSE, InitMove, MoveCam, NULL, NULL);

	//test keyboard control obj
	//FyDefineHotKey(FY_W, KeyboardCommand, FALSE);
	//FyDefineHotKey(FY_A, KeyboardCommand, FALSE);
	//FyDefineHotKey(FY_S, KeyboardCommand, FALSE);
	//FyDefineHotKey(FY_D, KeyboardCommand, FALSE);
	return TRUE;
}

/*
void KeyboardCommand(WORLDid gID, BYTE code, BOOL value){
	kc->Command(gID,code,value);
}
*/
//------------------------------------------------------------------------------------
// timer callback function which will be invoked by TheFly3D System every 1/30 second
// C.Wang 0308, 2004
//------------------------------------------------------------------------------------
void GameAI(int skip)
{
	kc->Command();
}

void Render(int skip){
	// render the scene
	FnViewport vp;
	vp.Object(vID);
	vp.Render(cID, TRUE, TRUE);
	PlayAction(skip);
	GetPosDetail(loopBuff);
	debug_message(debug,loopBuff);
	FnWorld gw;
	gw.Object(gID);
	gw.SwapBuffers();	
}
//-------------------
// quit the demo
// C.Wang 0304, 2004
//-------------------
void QuitGame(WORLDid gID, BYTE code, BOOL value)
{
	if (code == FY_ESCAPE) {
		if (value) {
			FyWin32EndWorld(gID);
			delete kc;
		}
	}
}


/*-----------------------------------
initialize the pivot of the camera
C.Wang 0329, 2004
------------------------------------*/
void InitPivot(WORLDid g, int x, int y)
{
	oldX = x;
	oldY = y;
}


/*------------------
pivot the camera
C.Wang 0329, 2004
-------------------*/
void PivotCam(WORLDid g, int x, int y)
{
	FnObject model;

	if (x != oldX) {
		model.Object(cID);
		model.Rotate(Z_AXIS, (float) (x - oldX), GLOBAL);
		oldX = x;
	}

	if (y != oldY) {
		model.Object(cID);
		model.Rotate(X_AXIS, (float) (y - oldY), GLOBAL);
		oldY = y;
	}
}


/*----------------------------------
initialize the move of the camera
C.Wang 0329, 2004
-----------------------------------*/
void InitMove(WORLDid g, int x, int y)
{
	oldXM = x;
	oldYM = y;
}


/*------------------
move the camera
C.Wang 0329, 2004
-------------------*/
void MoveCam(WORLDid g, int x, int y)
{
	if (x != oldXM) {
		FnObject model;

		model.Object(cID);
		model.Translate((float)(x - oldXM)*0.1f, 0.0f, 0.0f, LOCAL);
		oldXM = x;
	}
	if (y != oldYM) {
		FnObject model;

		model.Object(cID);
		model.Translate(0.0f, (float)(oldYM - y)*0.1f, 0.0f, LOCAL);
		oldYM = y;
	}
}


/*----------------------------------
initialize the zoom of the camera
C.Wang 0329, 2004
-----------------------------------*/
void InitZoom(WORLDid g, int x, int y)
{
	oldXMM = x;
	oldYMM = y;
}


/*------------------
zoom the camera
C.Wang 0329, 2004
-------------------*/
void ZoomCam(WORLDid g, int x, int y)
{
	if (x != oldXMM || y != oldYMM) {
		FnObject model;

		model.Object(cID);
		model.Translate(0.0f, 0.0f, (float)(x - oldXMM), LOCAL);
		oldXMM = x;
		oldYMM = y;
	}
}

void debug_message(char *string, char *string2){
	FnWorld gw;
	gw.Object(gID);
	gw.StartMessageDisplay();
	gw.MessageOnScreen(20,20,string2,255,0,0);
	gw.MessageOnScreen(20,200,string,255,0,0);
	gw.FinishMessageDisplay();
}