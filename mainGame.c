#include <stdio.h>
#include <math.h>
#include "raylib.h"
#define PHYSAC_IMPLEMENTATION
#define PHYSAC_NO_THREADS
#include "physac.h"
#define VERSION "Time Tales 0.2"
#define TRUE 1
#define FALSE 0
#define VELOCITY 0.15f
#define BACKVELOCITY 10
#define SCREENWIDTH 800
#define SCREENHEIGHT 600
#define SCALAR 1.5

//Global Variables
int groundLev = 495;
Vector2 charPos = {10, 495};
Vector2 backPos = {0, 0};
int currentFrame = 0;
int isLeft = FALSE;
int isJump = FALSE;
Vector2 titlePos = {(SCREENWIDTH / 5), 544};
Vector2 mousePoint = { 0.0f, 0.0f };
int gameCheck = 0;
int scrollinMan = FALSE;
int screenNum = 0;
int levelZero[3] = {FALSE, FALSE, FALSE};

//Physics Objects
//levelZero
PhysicsBody bottomBoxes;
PhysicsBody topBox;
PhysicsBody ledge;
PhysicsBody platform;
PhysicsBody threeBarrels;
PhysicsBody oneBarrel;
PhysicsBody ledgeTwo;
PhysicsBody oneBox;
PhysicsBody platformTwo;
PhysicsBody platformThree;


int main()
{
	//Window Initialization

	InitWindow(SCREENWIDTH, SCREENHEIGHT, VERSION);
	SetTargetFPS(60);
	SetConfigFlags(FLAG_MSAA_4X_HINT);
	
	//Image Resizing
	Image run = LoadImage("images/runSheet1.png");
	ImageResize(&run, (run.width * SCALAR), (run.height * SCALAR));
	Image idle = LoadImage("images/idleSheet1.png");
	ImageResize(&idle, (idle.width * SCALAR), (idle.height * SCALAR));
	//Image jump = LoadImage("images/jumpSheet.png");
	//ImageResize(&jump, (jump.width * SCALAR), (jump.height * SCALAR));
	Texture2D backGround = LoadTexture("images/backSheet.png");
	Texture2D levelZeroBack = LoadTexture("images/levelZero.png");
	//Texture Loading
	
	Texture2D runSheet = LoadTextureFromImage(run);
	Texture2D idleSheet = LoadTextureFromImage(idle);
	//Texture2D jumpSheet = LoadTextureFromImage(jump);
	Texture2D titleScreen = LoadTexture("images/titleScreen.png");
	Texture2D startButtonSheet = LoadTexture("images/startButtonSheet.png");
	UnloadImage(run);
	UnloadImage(idle);
	//UnloadImage(jump);
	
	//Physics Engine
	InitPhysics();
	PhysicsBody floor = CreatePhysicsBodyRectangle((Vector2){SCREENWIDTH / 2, SCREENHEIGHT}, SCREENWIDTH, 65, 10);
    PhysicsBody wallLeft = CreatePhysicsBodyRectangle((Vector2){ -5, SCREENHEIGHT / 2 }, 10, SCREENHEIGHT, 10);
    PhysicsBody wallRight = CreatePhysicsBodyRectangle((Vector2){ SCREENWIDTH + 5, SCREENHEIGHT / 2 }, 10, SCREENHEIGHT, 10);
	floor->enabled = false;
	wallLeft->enabled = false;
	wallRight->enabled = false;
	PhysicsBody charBody = CreatePhysicsBodyRectangle((Vector2)charPos, (float)(runSheet.width / 6), (float)(runSheet.height / 2), 1);
	charBody->freezeOrient = true;      // Constrain body rotation to avoid little collision torque amounts
	
	int charInput(Texture2D runSheet, PhysicsBody player) 
	{ // Handles Input from User and Turns it into Variable
		int key;
		if(IsKeyPressed(KEY_SPACE))
		{
			key = KEY_SPACE;
		}
		else if((IsKeyDown(KEY_D) == 1) || (IsKeyDown(KEY_RIGHT) == 1)){
			key = KEY_RIGHT;
		}
		else if((IsKeyDown(KEY_A) == 1) || (IsKeyDown(KEY_LEFT) == 1)){
			key = KEY_LEFT;
		}
		else{
			key = 0;
		}
		switch(key)
		{
			case KEY_RIGHT:
				isLeft = FALSE;
				if(IsKeyDown(KEY_LEFT_CONTROL)) {player->velocity.x = VELOCITY * 2;}
				else {player->velocity.x = VELOCITY;}
				return 1;
				break;
			case KEY_LEFT:
				isLeft = TRUE;
				if(IsKeyDown(KEY_LEFT_CONTROL)) {player->velocity.x = -VELOCITY * 2;}
				else {player->velocity.x = -VELOCITY;}
				return 1;
				break;
			case KEY_SPACE:
				player->velocity.y = -VELOCITY * 7.5;
				return 0;
				break;
			default:
				return 0;
		}
	}

	void drawChar(Texture2D runSheet, Texture2D idleSheet, PhysicsBody player)
	{ // Takes Variable from Previous Function and Draws cooresponding sprite
		int mvmentOption = charInput(runSheet, player);
		if(currentFrame >= 60) currentFrame = 0;
		switch(mvmentOption) 
		{
			case 0: //Default Idle Stance
				DrawTextureRec(idleSheet, 
					(Rectangle){0 + (float)(idleSheet.width / 3) * (currentFrame / 20), 
					(float)(0 + ((idleSheet.height / 2) * isLeft)), (float)(idleSheet.width / 3), 
					(float)(idleSheet.height / 2)},(Vector2)GetPhysicsShapeVertex(player, 3), WHITE);
				break;
			case 1: //Running Horizontal
				DrawTextureRec(runSheet, 
					(Rectangle){0 + (float)(runSheet.width / 6) * (currentFrame / 10), 
					(float)(0 + ((runSheet.height / 2) * isLeft)), (float)(runSheet.width / 6), 
					(float)(runSheet.height / 2)},(Vector2)GetPhysicsShapeVertex(player, 3), WHITE);
				break;
			//case 2:
				//DrawTextureRec(jumpSheet, 
					//(Rectangle){0 + (float)(jumpSheet.width / 4) * (currentFrame / 15), 
					//(float)(0 + (jumpSheet.height / 2) * isLeft), (float)(jumpSheet.width / 4), 
					//(float)(jumpSheet.height / 2)},(Vector2)GetPhysicsShapeVertex(player, 3), WHITE);
				//break;
		}
		currentFrame ++;
	}
	void startButtonFunction(Texture2D buttonSheet)
	{ // Function of Beginning Button
		UnloadTexture(buttonSheet);
		UnloadTexture(titleScreen);
		gameCheck = 1;
	}
	void buttonFunction(Texture2D buttonSheet, Vector2 btnCoords, int functionNumber)
	{ // Allows for Creation of more than one button
		mousePoint = GetMousePosition();
		Rectangle sourceRec = { 0, 0, buttonSheet.width, (float)(buttonSheet.height / 3)};
		Rectangle btnBounds = { btnCoords.x, btnCoords.y, buttonSheet.width, (float)(buttonSheet.height / 3)};
		int btnState = 0;
		int btnAction = FALSE;
		if(CheckCollisionPointRec(mousePoint, btnBounds))
		{
			if(IsMouseButtonDown(MOUSE_LEFT_BUTTON)) btnState = 2;
			else btnState = 1;
			
			if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) 
			{
				btnAction = TRUE;
			}
		}
		else btnState = 0;
		sourceRec.y = btnState*(float)(buttonSheet.height / 3);
		DrawTextureRec(buttonSheet, sourceRec, (Vector2){ btnBounds.x, btnBounds.y }, WHITE);
		if(btnAction)
		{
			switch(functionNumber)
			{
				case 1:
					startButtonFunction(buttonSheet);
					break;
			}
		}
	}
	int titleDisplay(Texture2D titleScreen)
	{ // Intro to Game
		if(IsKeyPressed(KEY_ENTER) && titlePos.y > 20)
		{
			titlePos.y = 20;
			DrawTexture(titleScreen, titlePos.x, titlePos.y, WHITE);
			return 1;
		}
		else if(titlePos.y > 20)
		{
			titlePos.y -= 2;
			DrawTexture(titleScreen, titlePos.x, titlePos.y, WHITE);
			return 1;
		}
		else
		{
			DrawTexture(titleScreen, titlePos.x, titlePos.y, WHITE);
			return 0;
		}
	}
	void backParallax(Texture2D runSheet, Texture2D idleSheet, Texture2D backSheet, PhysicsBody player)
	{ // Background movement according to screenNum (0-2)
		DrawTexture(backSheet, backPos.x, backPos.y, WHITE);
		if(scrollinMan == 1 || scrollinMan == 2)
		{
			if(scrollinMan == 2 && screenNum > 0 && screenNum <= 2) 
			{
				backPos.x += BACKVELOCITY;
				if(player->position.x < (SCREENWIDTH - 50) && backPos.x == (-1 * SCREENWIDTH * (screenNum - 1))) 
				{ 
					player->position = (Vector2){SCREENWIDTH - 50, 565};
					player->velocity = (Vector2){0, 0};
					SetPhysicsBodyRotation(player, 0);
					screenNum -= 1;
					scrollinMan = FALSE;
				}
			}
			else if(!((screenNum + 1) > 2))
			{
				backPos.x -= BACKVELOCITY;
				if(player->position.x != 50 && backPos.x == -1 * SCREENWIDTH * (screenNum + 1))
				{ 
					player->position = (Vector2){50, 565};
					player->velocity = (Vector2){0, 0};
					SetPhysicsBodyRotation(player, 0);
					screenNum ++;
					scrollinMan = FALSE;
				}
			}
		}
		else drawChar(runSheet, idleSheet, player);
		if(player->position.x >= (SCREENWIDTH - (float)(runSheet.width / 6)) && screenNum < 2) {DrawText("Press Enter to Continue", (SCREENWIDTH - 175), 470, 15, BLACK); if(IsKeyPressed(KEY_ENTER)) scrollinMan = TRUE;}
		else if(player->position.x <= 50 && screenNum > 0) {DrawText("Press Enter to Continue", 5, 470, 15, BLACK); if(IsKeyPressed(KEY_ENTER)) scrollinMan = 2;}
	}
	void levelZeroDeInit()
	{ //De-initializes the screen to move on to the next on Level 0
		if(levelZero[screenNum - 1])
		{
			switch(screenNum - 1)
			{
				case 0:
					DestroyPhysicsBody(bottomBoxes);
					DestroyPhysicsBody(topBox);
					DestroyPhysicsBody(ledge);
					DestroyPhysicsBody(platform);
					levelZero[screenNum - 1] = FALSE;
					break;
				case 1:
					DestroyPhysicsBody(threeBarrels);
					DestroyPhysicsBody(oneBarrel);
					DestroyPhysicsBody(ledgeTwo);
					DestroyPhysicsBody(oneBox);
					levelZero[screenNum - 1] = FALSE;
					break;
				case 2:
					DestroyPhysicsBody(platformTwo);
					DestroyPhysicsBody(platformThree);
					levelZero[screenNum - 1] = FALSE;
					break;
			}
		}
		else if(levelZero[screenNum + 1])
		{
			switch(screenNum + 1)
			{
				case 0:
					DestroyPhysicsBody(bottomBoxes);
					DestroyPhysicsBody(topBox);
					DestroyPhysicsBody(ledge);
					DestroyPhysicsBody(platform);
					levelZero[screenNum + 1] = FALSE;
					break;
				case 1:
					DestroyPhysicsBody(threeBarrels);
					DestroyPhysicsBody(oneBarrel);
					DestroyPhysicsBody(ledgeTwo);
					DestroyPhysicsBody(oneBox);
					levelZero[screenNum + 1] = FALSE;
					break;
				case 2:
					DestroyPhysicsBody(platformTwo);
					DestroyPhysicsBody(platformThree);
					levelZero[screenNum + 1] = FALSE;
					break;
			}
		}
	}
	void levelZeroInit()
	{ // Initializes the current screen on Level 0
		if(!(levelZero[screenNum]))
		{
			switch(screenNum)
			{
				case 0:
					bottomBoxes = CreatePhysicsBodyRectangle((Vector2){190, 544}, 73, 33, 10);
					topBox = CreatePhysicsBodyRectangle((Vector2){190, 511}, 39, 33, 10);
					ledge = CreatePhysicsBodyRectangle((Vector2){638.5, 524}, 48, 13, 10);
					platform = CreatePhysicsBodyRectangle((Vector2){731.5, 477}, 128, 5, 10);
					bottomBoxes->enabled = false;
					topBox->enabled = false;
					ledge->enabled = false;
					platform->enabled = false;
					levelZero[screenNum] = TRUE;
					break;
				case 1:
					levelZeroDeInit();
					threeBarrels = CreatePhysicsBodyRectangle((Vector2){173.5, 548.5}, 72, 24, 10);
					oneBarrel = CreatePhysicsBodyRectangle((Vector2){498.5, 548.5}, 24, 26, 10);
					ledgeTwo = CreatePhysicsBodyRectangle((Vector2){574.5, 520}, 48, 13, 10);
					oneBox = CreatePhysicsBodyRectangle((Vector2){640, 544}, 39, 33, 10);
					threeBarrels->enabled = false;
					oneBarrel->enabled = false;
					ledgeTwo->enabled = false;
					oneBox->enabled = false;
					levelZero[screenNum] = TRUE;
					break;
				case 2:
					levelZeroDeInit();
					platformTwo = CreatePhysicsBodyRectangle((Vector2){359, 518}, 95, 5, 10);
					platformThree = CreatePhysicsBodyRectangle((Vector2){466, 472}, 95, 5, 10);
					platformTwo->enabled = false;
					platformThree->enabled = false;
					levelZero[screenNum] = TRUE;
					break;
			}
		}
	}
	while(!WindowShouldClose())
	{
		RunPhysicsStep();
		BeginDrawing();
		ClearBackground(RAYWHITE);
		switch(gameCheck){
			case 0:
				DrawTexture(backGround, backPos.x, backPos.y, WHITE);
				if(!titleDisplay(titleScreen))
				{
					buttonFunction(startButtonSheet, (Vector2){SCREENWIDTH / 2 - startButtonSheet.width / 2, SCREENHEIGHT / 2 - startButtonSheet.height / 3 / 2}, 1);
					drawChar(runSheet, idleSheet, charBody);
				}
				break;
			case 1:
				backParallax(runSheet, idleSheet, levelZeroBack, charBody);
				levelZeroInit();
				break;
		}
		DrawFPS(5, 5);
		EndDrawing();
	}
	CloseWindow();
    return 0;
}
