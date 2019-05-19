#include <stdio.h>
#include <raylib.h>
#define VERSION "WHAP Project 0.1"
#define TRUE 1
#define FALSE 0

int main()
{
    struct charAspects{
		int isStanding;
		int walkRight;
		int walkLeft;
		int walkCount;
		int standCount;
	};
	struct charAspects man = {TRUE, TRUE, FALSE, 0, 0};
    int screenWidth = 480;
    int screenHeight = 272;
    InitWindow(screenWidth, screenHeight, VERSION);
    Texture2D runSheet = LoadTexture("images/runSheet.png");
	Texture2D background = LoadTexture("images/background.png");
	Texture2D idleSheet = LoadTexture("images/idleSheet.png");
	float idleFrameWidth = (float)(idleSheet.width / 3);
	float runFrameWidth = (float)(runSheet.width / 6);
	Vector2 pos = {0, 208};
    float vel = 4;
    
    SetTargetFPS(30);
    
    struct charAspects windowDraw(struct charAspects charAspects, Texture2D runSheet, Texture2D idleSheet, Vector2 pos, Texture2D background, float idleFrameWidth, float runFrameWidth)
    {
		DrawTexture(background, 0, 0, WHITE);
		DrawFPS(0, 0);
		if ((charAspects.walkCount + 1) >= 18)
			{
			charAspects.walkCount = 0;
			}
		if ((charAspects.standCount + 1) >= 18)
			{
			charAspects.standCount = 0;
			}
		if (charAspects.isStanding == TRUE)
			{
			if (charAspects.walkRight == TRUE)
				{
				DrawTextureRec(idleSheet, (Rectangle){0 + idleFrameWidth * (charAspects.standCount / 6), 0, idleFrameWidth, (float)(idleSheet.height / 2)},(Vector2)pos, WHITE);
				charAspects.standCount += 1;
				}
			else if (charAspects.walkLeft == TRUE)
				{
				DrawTextureRec(idleSheet, (Rectangle){0 + idleFrameWidth * (charAspects.standCount / 6), 37, idleFrameWidth, (float)(idleSheet.height / 2)},(Vector2)pos, WHITE);
				charAspects.standCount += 1;
				}
			}
		else {
			if (charAspects.walkRight == TRUE)
				{
				DrawTextureRec(runSheet, (Rectangle){0 + runFrameWidth * (charAspects.walkCount / 3), 0, runFrameWidth, (float)(runSheet.height / 2)},(Vector2)pos, WHITE);
				if (IsKeyDown(KEY_RIGHT)){
					charAspects.walkCount += 1;
					}
				else{
					charAspects.walkCount = 0;
					}
				}
			else if (charAspects.walkLeft == TRUE)
				{
				DrawTextureRec(runSheet, (Rectangle){0 + runFrameWidth * (charAspects.walkCount / 3), 37, runFrameWidth, (float)(runSheet.height / 2)},(Vector2)pos, WHITE);
				if (IsKeyDown(KEY_LEFT)){
				charAspects.walkCount += 1;
					}
				else{
					charAspects.walkCount = 0;
				}
				}
			}
		return charAspects;
	}
    
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        if (IsKeyDown(KEY_RIGHT) && pos.x < (screenWidth - 37 - vel))
		{
			pos.x += vel;
			man.walkRight = TRUE;
			man.walkLeft = FALSE;
			man.isStanding = FALSE;
		}
		else if (IsKeyDown(KEY_LEFT) && pos.x > 0)
		{
			pos.x -= vel;
			man.walkRight = FALSE;
			man.walkLeft = TRUE;
			man.isStanding = FALSE;
		}
		else if (IsKeyUp(KEY_RIGHT) && IsKeyUp(KEY_LEFT))
		{
			man.isStanding = TRUE;
		}
        
        BeginDrawing();
		//ClearBackground(RAYWHITE);
		man = windowDraw(man, runSheet, idleSheet, pos, background, idleFrameWidth, runFrameWidth);
        EndDrawing();
    }
	CloseWindow();
    return 0;
}
