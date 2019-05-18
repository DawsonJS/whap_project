#include <stdio.h>
#include "raylib.h"
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
	};
	struct charAspects man = {TRUE, FALSE, FALSE, 0};
    int screenWidth = 480;
    int screenHeight = 272;
    InitWindow(screenWidth, screenHeight, VERSION);
    Texture2D charLeft[9] = {LoadTexture("images/L1.png"), LoadTexture("images/L2.png"), LoadTexture("images/L3.png"),
							LoadTexture("images/L4.png"), LoadTexture("images/L5.png"), LoadTexture("images/L6.png"), 
							LoadTexture("images/L7.png"), LoadTexture("images/L8.png"), LoadTexture("images/L9.png")};
	Texture2D charRight[9] = {LoadTexture("images/R1.png"), LoadTexture("images/R2.png"), LoadTexture("images/R3.png"),
							LoadTexture("images/R4.png"), LoadTexture("images/R5.png"), LoadTexture("images/R6.png"), 
							LoadTexture("images/R7.png"), LoadTexture("images/R8.png"), LoadTexture("images/R9.png")};
	Texture2D background = LoadTexture("images/background.png");
	Texture2D character = LoadTexture("images/standing.png");
	int posX = 50;
	int posY = 208;
    int vel = 4;
    
    SetTargetFPS(30);
    
    int windowDraw(struct charAspects charAspects, Texture2D charLeft[], Texture2D charRight[], Texture2D character, int posX, int posY, Texture2D background)
    {
		DrawTexture(background, 0, 0, WHITE);
		if ((charAspects.walkCount + 1) >= 27)
			{
			charAspects.walkCount = 0;
			}
		if (charAspects.isStanding == TRUE)
			{
			DrawTexture(character, posX, posY, WHITE);
			}
		else {
			if (charAspects.walkRight == TRUE)
				{
				DrawTexture(charRight[charAspects.walkCount / 3], posX, posY, WHITE);
				if (IsKeyDown(KEY_RIGHT)){
					charAspects.walkCount += 1;
					}
				else{
					charAspects.walkCount = 0;
					}
				}
			if (charAspects.walkLeft == TRUE)
				{
				DrawTexture(charLeft[charAspects.walkCount / 3], posX, posY, WHITE);
				if (IsKeyDown(KEY_LEFT)){
				charAspects.walkCount += 1;
					}
				else{
					charAspects.walkCount = 0;
				}
				}
			}
		return charAspects.walkCount;
	}
    
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        if (IsKeyDown(KEY_RIGHT) && posX < (screenWidth - 64 - vel))
		{
			posX += vel;
			man.walkRight = TRUE;
			man.walkLeft = FALSE;
			man.isStanding = FALSE;
		}
		if (IsKeyDown(KEY_LEFT) && posX > vel)
		{
			posX -= vel;
			man.walkRight = FALSE;
			man.walkLeft = TRUE;
			man.isStanding = FALSE;
		}
		if (IsKeyPressed(KEY_DOWN))
		{
			man.isStanding = TRUE;
		}
        
        BeginDrawing();
		ClearBackground(RAYWHITE);
		man.walkCount = windowDraw(man, charLeft, charRight, character, posX, posY, background);
        EndDrawing();
    }
	CloseWindow();
    return 0;
}
