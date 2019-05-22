#include <stdio.h>
#include <math.h>
#include "raylib.h"
#define VERSION "Time Heist 0.1"
#define TRUE 1
#define FALSE 0

// Global Variables
int idleFrame = 0;
int runFrame = 0;
int jumpFrame = 0;
int isLeft = FALSE;
Vector2 pos = {0, 420};
float vel = 6;
int btnAction = FALSE;
int firstBtnAction = FALSE;
int isJump = FALSE;

int main()
{
	//Window Initialization
	int screenWidth = 960;
	int screenHeight = 544;
	InitWindow(screenWidth, screenHeight, VERSION);
	SetTargetFPS(30);
	
	//Image Resizing
	Image run = LoadImage("images/runSheet.png");
	ImageResize(&run, (run.width * 2), (run.height * 2));
	Image idle = LoadImage("images/idleSheet.png");
	ImageResize(&idle, (idle.width * 2), (idle.height * 2));
	Image jump = LoadImage("images/jumpSheet.png");
	ImageResize(&jump, (jump.width * 2), (jump.height * 2));
	Image back = LoadImage("images/background.png");
	ImageResize(&back, screenWidth, screenHeight);
	
	//Texture Loading
	Texture2D background = LoadTextureFromImage(back);
	Texture2D titleScreen = LoadTexture("images/titleScreen.png");
	Texture2D runSheet = LoadTextureFromImage(run);
	Texture2D idleSheet = LoadTextureFromImage(idle);
	Texture2D jumpSheet = LoadTextureFromImage(jump);
	Texture2D startButtonSheet = LoadTexture("images/startButtonSheet.png");
	

	//Music Loading
	InitAudioDevice();
	Music music01 = LoadMusicStream("sounds/fairy_fountain.ogg");
	PlayMusicStream(music01);
	
	// Frame Variables
	float idleFrameWidth = (float)(idleSheet.width / 3);
	float runFrameWidth = (float)(runSheet.width / 6);
	float buttonFrame = (float)(startButtonSheet.height / 3);
	float jumpFrameWidth = (float)(jumpSheet.width / 4);
	Rectangle sourceRec = { 0, 0, startButtonSheet.width, buttonFrame };
	Rectangle btnBounds = { screenWidth / 2 - startButtonSheet.width / 2, screenHeight / 2 - startButtonSheet.height / 3 / 2, startButtonSheet.width, buttonFrame };
	int btnState = 0;
	Vector2 mousePoint = { 0.0f, 0.0f };
	
	// Title Variables
	Vector2 titlePos = {(screenWidth / 5), 544};
	int titleVel = 2;
	
	int GetKeyDown()
	{
		if((IsKeyDown(KEY_D) == 1) || (IsKeyDown(KEY_RIGHT) == 1)){
			return KEY_RIGHT;
		}
		else if((IsKeyDown(KEY_A) == 1) || (IsKeyDown(KEY_LEFT) == 1)){
			return KEY_LEFT;
		}
		else if(IsKeyPressed(KEY_SPACE)){
			return KEY_SPACE;
		}
		else{
			return 0;
		}
	}

	void drawChar(Texture2D runSheet, Texture2D idleSheet, int runFrameWidth, int idleFrameWidth)
	{
		if((idleFrame + 1) >= 18)
		{
			idleFrame = 0;
		}
		else if((runFrame + 1) >= 18)
		{
			runFrame = 0;
		}
		
		//Character Movement Display Frame by Frame
		switch(GetKeyDown())
		{
			case KEY_RIGHT:
				if(pos.x < (screenWidth - runFrameWidth - vel))
				{
					pos.x += vel;
				}
				isLeft = FALSE;
				DrawTextureRec(runSheet, (Rectangle){0 + runFrameWidth * (runFrame / 3), (float)(0 + (runSheet.height / 2) * isLeft), runFrameWidth, (float)(runSheet.height / 2)},(Vector2)pos, WHITE);
				runFrame ++;
				break;
			case KEY_LEFT:
				if(pos.x > vel)
				{
					pos.x -= vel;
				}
				isLeft = TRUE;
				DrawTextureRec(runSheet, (Rectangle){0 + runFrameWidth * (runFrame / 3), (float)(0 + (runSheet.height / 2) * isLeft), runFrameWidth, (float)(runSheet.height / 2)},(Vector2)pos, WHITE);
				runFrame ++;
				break;
			default:
				DrawTextureRec(idleSheet, (Rectangle){0 + idleFrameWidth * (idleFrame / 6), (float)(0 + (idleSheet.height / 2) * isLeft), idleFrameWidth, (float)(idleSheet.height / 2)},(Vector2)pos, WHITE);
				idleFrame ++;
				jumpFrame = 0;
				runFrame = 0;
		}
	}
	
	int firstBtnFunction()
	{
		if(firstBtnAction)
		{
			UnloadTexture(startButtonSheet);
			UnloadTexture(titleScreen);
			firstBtnAction = FALSE;
			return 0;
		}
		else return 0;
	}
	//Main Window Loop for Drawing / Music / Anything Else
	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		mousePoint = GetMousePosition();
		UpdateMusicStream(music01);

        sourceRec.y = btnState*buttonFrame;
		if(GetMusicTimePlayed(music01) >= GetMusicTimeLength(music01)) 
		{
			StopMusicStream(music01);
			PlayMusicStream(music01);
		}
		
		BeginDrawing();
        ClearBackground(RAYWHITE);
		DrawTexture(background, 0, 0, WHITE);
		if(IsKeyPressed(KEY_SPACE))
		{
			titlePos.y = 20;
		}
		else if(titlePos.y != 20)
		{
			titlePos.y -= titleVel;
		}
		
		DrawFPS(0, 0);
		
		//Check if Button is Pressed then Display Character
		if(CheckCollisionPointRec(mousePoint, btnBounds))
		{
			if(IsMouseButtonDown(MOUSE_LEFT_BUTTON)) btnState = 2;
			else btnState = 1;
			
			if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) 
			{
				btnAction = TRUE;
				firstBtnAction = TRUE;
			}
		}
		else btnState = 0;
		
		if(btnAction)
		{
			
			if(IsKeyPressed(KEY_SPACE))
			{
				isJump = TRUE;
			}
			if(isJump)
			{
				DrawTextureRec(jumpSheet, (Rectangle){0 + jumpFrameWidth * (jumpFrame / 6), (float)(0 + (jumpSheet.height / 2) * isLeft), jumpFrameWidth, (float)(jumpSheet.height / 2)},(Vector2){pos.x, (float)(-64 * sin((3.14 / 24) * jumpFrame) + 420)}, WHITE);
				jumpFrame ++;
				idleFrame = 0;
				runFrame = 0;
				if (jumpFrame >= 24) {jumpFrame = 0; isJump = FALSE;}
			}
			else
			{
			drawChar(runSheet, idleSheet, runFrameWidth, idleFrameWidth);
			firstBtnFunction();
			}
        }
        else
        {
			DrawTextureRec(startButtonSheet, sourceRec, (Vector2){ btnBounds.x, btnBounds.y }, WHITE);
			DrawTexture(titleScreen, titlePos.x, titlePos.y, WHITE);
		}
		
		
        EndDrawing();
    }
    UnloadMusicStream(music01);
	CloseAudioDevice();
	CloseWindow();
    return 0;
}
