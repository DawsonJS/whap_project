#include <stdio.h>
#include <raylib.h>
#define VERSION "WHAP Project 0.1"
#define TRUE 1
#define FALSE 0

int main()
{
	//Window Initialization
	int screenWidth = 480;
	int screenHeight = 272;
	InitWindow(screenWidth, screenHeight, VERSION);
	SetTargetFPS(30);
	
	//Texture / Sprite Sheets Loading
	Texture2D runSheet = LoadTexture("images/runSheet.png");
	Texture2D background = LoadTexture("images/background.png");
	Texture2D idleSheet = LoadTexture("images/idleSheet.png");
	
	// Frame Variables
	float idleFrameWidth = (float)(idleSheet.width / 3);
	float runFrameWidth = (float)(runSheet.width / 6);
	int idleFrame = 0;
	int runFrame = 0;
	int isLeft = FALSE;
	
	//Movement Variables
	Vector2 pos = {0, 208};
	float vel = 4;
	
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
		BeginDrawing();
		ClearBackground(RAYWHITE);
		DrawTexture(background, 0, 0, WHITE);
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
	CloseWindow();
    return 0;
}
