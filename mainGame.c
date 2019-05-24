#include <stdio.h>
#include <math.h>
#include "raylib.h"
#define PHYSAC_IMPLEMENTATION
#define PHYSAC_NO_THREADS
#include "physac.h"
#define VERSION "Time Tales 0.1"
#define TRUE 1
#define FALSE 0
#define VELOCITY 2
#define SCREENWIDTH 800
#define SCREENHEIGHT 600

//Global Variables
int groundLev = 495;
Vector2 charPos = {0, 495};
Vector2 backPos = {0, 0};
int currentFrame = 0;
int isLeft = FALSE;
int isJump = FALSE;
Vector2 titlePos = {(SCREENWIDTH / 5), 544};
Vector2 mousePoint = { 0.0f, 0.0f };
int gameCheck = 0;
int scrollinMan = FALSE;
int screenNum = 0;

int main()
{
	//Window Initialization

	InitWindow(SCREENWIDTH, SCREENHEIGHT, VERSION);
	SetTargetFPS(60);
	
	//Image Resizing
	Image run = LoadImage("images/runSheet.png");
	ImageResize(&run, (run.width * 2), (run.height * 2));
	Image idle = LoadImage("images/idleSheet.png");
	ImageResize(&idle, (idle.width * 2), (idle.height * 2));
	Image jump = LoadImage("images/jumpSheet.png");
	ImageResize(&jump, (jump.width * 2), (jump.height * 2));
	Texture2D backGround = LoadTexture("images/backSheet.png");
	//Texture Loading
	
	Texture2D runSheet = LoadTextureFromImage(run);
	Texture2D idleSheet = LoadTextureFromImage(idle);
	Texture2D jumpSheet = LoadTextureFromImage(jump);
	Texture2D titleScreen = LoadTexture("images/titleScreen.png");
	Texture2D startButtonSheet = LoadTexture("images/startButtonSheet.png");
	UnloadImage(run);
	UnloadImage(idle);
	UnloadImage(jump);
	
	//Input
	int charInput(Texture2D runSheet)
	{
		int key;
		if(IsKeyPressed(KEY_SPACE))
		{
			isJump = TRUE;
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
		if(isJump)
		{
			charPos.y = (-64 * sin((3.14 / 60) * currentFrame) + groundLev);
			if((IsKeyDown(KEY_D) == 1) || (IsKeyDown(KEY_RIGHT) == 1))
			{
				if(charPos.x < (SCREENWIDTH - (float)(runSheet.width / 6) - VELOCITY))
				{
					charPos.x += VELOCITY;
					}
			}
			else if((IsKeyDown(KEY_A) == 1) || (IsKeyDown(KEY_LEFT) == 1))
			{
				isLeft = TRUE;
				if(charPos.x > VELOCITY)
				{
					charPos.x -= VELOCITY;
				}
			}
			return 2;
		}
		else
		{
			switch(key)
			{
				case KEY_RIGHT:
					if(charPos.x < (SCREENWIDTH - (float)(runSheet.width / 6) - VELOCITY))
					{
						charPos.x += VELOCITY;
					}
					isLeft = FALSE;
					return 1;
					break;
				case KEY_LEFT:
					if(charPos.x > VELOCITY)
					{
						charPos.x -= VELOCITY;
					}
					isLeft = TRUE;
					return 1;
					break;
				default:
					return 0;
			}
		}
	}
	
	//Character Drawing
	void drawChar(Texture2D runSheet, Texture2D idleSheet, Texture2D jumpSheet)
	{
		int caseSwitch = charInput(runSheet);
		if(currentFrame >= 60) {currentFrame = 0; isJump = FALSE;}
		switch(caseSwitch)
		{
			case 0: //Default Idle Stance
				DrawTextureRec(idleSheet, (Rectangle){0 + (float)(idleSheet.width / 3) * (currentFrame / 20), (float)(0 + ((idleSheet.height / 2) * isLeft)), (float)(idleSheet.width / 3), (float)(idleSheet.height / 2)},(Vector2)charPos, WHITE);
				break;
			case 1: //Running Horizontal
				DrawTextureRec(runSheet, (Rectangle){0 + (float)(runSheet.width / 6) * (currentFrame / 10), (float)(0 + ((runSheet.height / 2) * isLeft)), (float)(runSheet.width / 6), (float)(runSheet.height / 2)},(Vector2)charPos, WHITE);
				break;
			case 2: // Jump
				DrawTextureRec(jumpSheet, (Rectangle){0 + (float)(jumpSheet.width / 4) * (currentFrame / 15), (float)(0 + (jumpSheet.height / 2) * isLeft), (float)(jumpSheet.width / 4), (float)(jumpSheet.height / 2)},(Vector2)charPos, WHITE);
				break;
		}
		currentFrame ++;
	}
	void startButtonFunction(Texture2D buttonSheet)
	{
		UnloadTexture(buttonSheet);
		UnloadTexture(titleScreen);
		gameCheck = 1;
	}
	void buttonFunction(Texture2D buttonSheet, Vector2 btnCoords, int functionNumber)
	{
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
	{
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
	void backParallax(Texture2D runSheet, Texture2D backSheet)
	{
		DrawTexture(backSheet, backPos.x, backPos.y, WHITE);
		if(scrollinMan == 1 || scrollinMan == 2)
		{
			if(scrollinMan == 2 && screenNum > 0) 
			{
				backPos.x += VELOCITY;
				if(charPos.x < (SCREENWIDTH - (float)(runSheet.width / 6))) {charPos.x += VELOCITY;}
				if(backPos.x == (SCREENWIDTH * (screenNum - 1))) {screenNum -= 1; scrollinMan = FALSE;}
			}
			else
			{
				backPos.x -= VELOCITY;
				if(charPos.x > VELOCITY) charPos.x -= VELOCITY;
				if(backPos.x == -1 * SCREENWIDTH) {screenNum ++; scrollinMan = FALSE;}
			}
		}
		if(charPos.x >= (SCREENWIDTH - (float)(runSheet.width / 6) - VELOCITY)) {DrawText("Press Enter to Continue", (SCREENWIDTH - 175), 470, 15, BLACK); if(IsKeyPressed(KEY_ENTER)) scrollinMan = TRUE;}
		else if(charPos.x == VELOCITY && screenNum > 0) {DrawText("Press Enter to Continue", 5, 470, 15, BLACK); if(IsKeyPressed(KEY_ENTER)) scrollinMan = 2;}
	}
	while(!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(RAYWHITE);
		switch(gameCheck){
			case 0:
				DrawTexture(backGround, backPos.x, backPos.y, WHITE);
				if(!titleDisplay(titleScreen))
				{
					buttonFunction(startButtonSheet, (Vector2){SCREENWIDTH / 2 - startButtonSheet.width / 2, SCREENHEIGHT / 2 - startButtonSheet.height / 3 / 2}, 1);
					drawChar(runSheet, idleSheet, jumpSheet);
				}
				break;
			case 1:
				backParallax(runSheet, backGround);
				drawChar(runSheet, idleSheet, jumpSheet);
				break;
		}
		DrawFPS(5, 5);
		EndDrawing();
	}
	CloseWindow();
    return 0;
}
