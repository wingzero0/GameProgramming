/*==============================================================
This is a demo program for TheFly System

Hello !

(C)2004 Chuan-Chang Wang, All Rights Reserved
Created : 0303, 2004, C. Wang

Last Updated : 1010, 2004, C.Wang
===============================================================*/
#include "TheFlyWin32.h"

int oldX, oldY, oldXM, oldYM, oldXMM, oldYMM;

WORLDid gID = FAILED_ID;
VIEWPORTid vID;
SCENEid sID;
OBJECTid nID, cID, lID;
OBJECTid tID;
ACTORid lyubu;

char debug[1024] = "\0";
void debug_message(char*);

void QuitGame(WORLDid, BYTE, BOOL);
BOOL initLyubu();
void PlayAction(int skip);
void GameAI(int);

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
	
	initLyubu();

	// translate the camera
	FnCamera camera;
	camera.Object(cID);
	camera.Rotate(X_AXIS, 90.0f, LOCAL);
	camera.Translate(0.0f, 10.0f, 100.0f, LOCAL);

	// translate the light
	FnLight light;
	light.Object(lID);
	light.SetName("MainLight");
	light.Translate(-50.0f, -50.0f, 50.0f, GLOBAL);

	// set Hotkeys
	FyDefineHotKey(FY_ESCAPE, QuitGame, FALSE);

	// define some mouse functions
	FyBindMouseFunction(LEFT_MOUSE, InitPivot, PivotCam, NULL, NULL);
	FyBindMouseFunction(MIDDLE_MOUSE, InitZoom, ZoomCam, NULL, NULL);
	FyBindMouseFunction(RIGHT_MOUSE, InitMove, MoveCam, NULL, NULL);

	/* bind a timer, frame rate = 30 fps */
	FyBindTimer(0, 30.0f, GameAI, TRUE);

	// invoke the system
	FyInvokeTheFly(TRUE);
}

BOOL initLyubu(){
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
	pos[1] = -3208;
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
	return TRUE;
}

void PlayAction(int skip){
	FnActor actor;
	actor.Object(lyubu);
	
	if (actor.Play(0,LOOP, skip, FALSE,TRUE) == FALSE){
		//sprintf(debug, "%s play action failed\n", debug);
	}else{
		//sprintf(debug, "%s played\n", debug);
	}
}
//------------------------------------------------------------------------------------
// timer callback function which will be invoked by TheFly3D System every 1/30 second
// C.Wang 0308, 2004
//------------------------------------------------------------------------------------
void GameAI(int skip)
{
	//Play action
	//PlayAction(skip);
	// render the scene
	FnViewport vp;
	vp.Object(vID);
	vp.Render(cID, TRUE, TRUE);
	PlayAction(skip);
	debug_message(debug);
	//debug[0] = '\0';
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

void debug_message(char *string){
	FnWorld gw;
	gw.Object(gID);
	gw.StartMessageDisplay();
	gw.MessageOnScreen(20,200,string,255,0,0);
	gw.FinishMessageDisplay();
}