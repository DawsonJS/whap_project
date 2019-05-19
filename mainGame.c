#include <stdio.h>
#include "raylib.h"
#define VERSION "WHAP Project 0.1"
#define TRUE 1
#define FALSE 0

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
	Image back = LoadImage("images/background.png");
	ImageResize(&back, screenWidth, screenHeight);
	
	//Texture Loading
	Texture2D background = LoadTextureFromImage(back);
	Texture2D titleScreen = LoadTexture("images/titleScreen.png");
	Texture2D runSheet = LoadTextureFromImage(run);
	Texture2D idleSheet = LoadTextureFromImage(idle);
	
	//Music Loading
	InitAudioDevice();
	Music music01 = LoadMusicStream("sounds/fairy_fountain.ogg");
	PlayMusicStream(music01);
	
	// Frame Variables
	float idleFrameWidth = (float)(idleSheet.width / 3);
	float runFrameWidth = (float)(runSheet.width / 6);
	int idleFrame = 0;
	int runFrame = 0;
	int isLeft = FALSE;
	
	//Movement Variables
	Vector2 pos = {0, 420};
	float vel = 6;
	
	Vector2 titlePos = {(screenWidth / 6), 544};
	int titleVel = 2;
	
	int GetKeyDown()
	{
		if((IsKeyDown(KEY_D) == 1) || (IsKeyDown(KEY_RIGHT) == 1)){
			return KEY_RIGHT;
		}
		else if((IsKeyDown(KEY_A) == 1) || (IsKeyDown(KEY_LEFT) == 1)){
			return KEY_LEFT;
		}
		else{
			return 0;
		}
	}

	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		UpdateMusicStream(music01);
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
		DrawTexture(titleScreen, titlePos.x, titlePos.y, WHITE);
		DrawFPS(0, 0);
		
		//Frame Counter
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
				DrawTextureRec(idleSheet, (Rectangle){0 + idleFrameWidth * (idleFrame / 6), (float)(0 + (runSheet.height / 2) * isLeft), idleFrameWidth, (float)(idleSheet.height / 2)},(Vector2)pos, WHITE);
				idleFrame ++;
				runFrame = 0;
		}
        EndDrawing();
    }
    UnloadMusicStream(music01);
	CloseAudioDevice();
	CloseWindow();
    return 0;
}
