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
		int standCount;
	};
	struct charAspects man = {TRUE, TRUE, FALSE, 0, 0};
    int screenWidth = 480;
    int screenHeight = 272;
    InitWindow(screenWidth, screenHeight, VERSION);
    Texture2D charLeft[6] = {LoadTexture("images/runleft00.png"), LoadTexture("images/runleft01.png"), LoadTexture("images/runleft02.png"),
							LoadTexture("images/runleft03.png"), LoadTexture("images/runleft04.png"), LoadTexture("images/runleft05.png")};
	Texture2D charRight[6] = {LoadTexture("images/runright00.png"), LoadTexture("images/runright01.png"), LoadTexture("images/runright02.png"),
							LoadTexture("images/runright03.png"), LoadTexture("images/runright04.png"), LoadTexture("images/runright05.png")};
	Texture2D background = LoadTexture("images/background.png");
	Texture2D idleRight[3] = {LoadTexture("images/idleright00.png"), LoadTexture("images/idleright01.png"), LoadTexture("images/idleright02.png")};
	Texture2D idleLeft[3] = {LoadTexture("images/idleleft00.png"), LoadTexture("images/idleleft01.png"), LoadTexture("images/idleleft02.png")};
	int posX = 0;
	int posY = 208;
    int vel = 4;
    
    SetTargetFPS(30);
    
    struct charAspects windowDraw(struct charAspects charAspects, Texture2D charLeft[], Texture2D charRight[], Texture2D idleRight[], Texture2D idleLeft[], int posX, int posY, Texture2D background)
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
				DrawTexture(idleRight[charAspects.standCount / 6], posX, posY, WHITE);
				charAspects.standCount += 1;
				}
			if (charAspects.walkLeft == TRUE)
				{
				DrawTexture(idleLeft[charAspects.standCount / 6], posX, posY, WHITE);
				charAspects.standCount += 1;
				}
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
		return charAspects;
	}
    
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        if (IsKeyDown(KEY_RIGHT) && posX < (screenWidth - 37 - vel))
		{
			posX += vel;
			man.walkRight = TRUE;
			man.walkLeft = FALSE;
			man.isStanding = FALSE;
		}
		if (IsKeyDown(KEY_LEFT) && posX > 0)
		{
			posX -= vel;
			man.walkRight = FALSE;
			man.walkLeft = TRUE;
			man.isStanding = FALSE;
		}
		if (IsKeyUp(KEY_RIGHT) && IsKeyUp(KEY_LEFT))
		{
			man.isStanding = TRUE;
		}
        
        BeginDrawing();
		//ClearBackground(RAYWHITE);
		man = windowDraw(man, charLeft, charRight, idleRight, idleLeft, posX, posY, background);
        EndDrawing();
    }
	CloseWindow();
    return 0;
}
