/*******************************************************************************************
*
*   Physac - Physics movement
*
*   NOTE 1: Physac requires multi-threading, when InitPhysics() a second thread is created to manage physics calculations.
*   NOTE 2: Physac requires static C library linkage to avoid dependency on MinGW DLL (-static -lpthread)
*
*   Use the following line to compile:
*
*   gcc -o $(NAME_PART).exe $(FILE_NAME) -s -static  /
*       -lraylib -lpthread -lglfw3 -lopengl32 -lgdi32 -lopenal32 -lwinmm /
*       -std=c99 -Wl,--subsystem,windows -Wl,-allow-multiple-definition
*
*   Copyright (c) 2016-2018 Victor Fisac
*
********************************************************************************************/

#include "raylib.h"

#define PHYSAC_IMPLEMENTATION
#define PHYSAC_NO_THREADS
#include "physac.h"

#define VELOCITY    0.25f
#define TRUE 1
#define FALSE 0

int x = 0;
int isLeft = FALSE;
int isJump = FALSE;
int jumpFrame = 0;
int currentFrame = 0;

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(screenWidth, screenHeight, "Physac [raylib] - Physics movement");
    
    //Image Resizing
	Image run = LoadImage("images/runSheet.png");
	ImageResize(&run, (run.width * 2), (run.height * 2));
	Image idle = LoadImage("images/idleSheet.png");
	ImageResize(&idle, (idle.width * 2), (idle.height * 2));
	Image jump = LoadImage("images/jumpSheet.png");
	ImageResize(&jump, (jump.width * 2), (jump.height * 2));
	Texture2D runSheet = LoadTextureFromImage(run);
	Texture2D idleSheet = LoadTextureFromImage(idle);
	Texture2D jumpSheet = LoadTextureFromImage(jump);
	UnloadImage(run);
	UnloadImage(idle);
	UnloadImage(jump);

    // Physac logo drawing position
    int logoX = screenWidth - MeasureText("Physac", 30) - 10;
    int logoY = 15;

    // Initialize physics and default physics bodies
    InitPhysics();

    // Create floor and walls rectangle physics body
    PhysicsBody floor = CreatePhysicsBodyRectangle((Vector2){ screenWidth/2, screenHeight }, screenWidth, 100, 10);
    PhysicsBody platformLeft = CreatePhysicsBodyRectangle((Vector2){ screenWidth*0.25f, screenHeight*0.6f }, screenWidth*0.25f, 10, 10);
    PhysicsBody platformRight = CreatePhysicsBodyRectangle((Vector2){ screenWidth*0.75f, screenHeight*0.6f }, screenWidth*0.25f, 10, 10);
    PhysicsBody wallLeft = CreatePhysicsBodyRectangle((Vector2){ -5, screenHeight/2 }, 10, screenHeight, 10);
    PhysicsBody wallRight = CreatePhysicsBodyRectangle((Vector2){ screenWidth + 5, screenHeight/2 }, 10, screenHeight, 10);

    // Disable dynamics to floor and walls physics bodies
    floor->enabled = false;
    platformLeft->enabled = false;
    platformRight->enabled = false;
    wallLeft->enabled = false;
    wallRight->enabled = false;

    // Create movement physics body
    PhysicsBody body = CreatePhysicsBodyRectangle((Vector2){ screenWidth/2, screenHeight/2 }, (float)(runSheet.width / 6), (float)(runSheet.height / 2), 1);
    body->freezeOrient = true;      // Constrain body rotation to avoid little collision torque amounts

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

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
				body->velocity.x = VELOCITY; 
				return 1;
				break;
			case KEY_LEFT:
				isLeft = TRUE;
				body->velocity.x = -VELOCITY;
				return 1;
				break;
			case KEY_SPACE:
				body->velocity.y = -VELOCITY * 6;
				return 2;
				break;
			default:
				return 0;
		}
	}

	void drawChar(Texture2D runSheet, Texture2D idleSheet, Texture2D jumpSheet, PhysicsBody player)
	{ // Takes Variable from Previous Function and Draws cooresponding sprite
		int mvmentOption = charInput(runSheet, player);
		if(currentFrame >= 60) currentFrame = 0;
		switch(mvmentOption) 
		{
			case 0: //Default Idle Stance
				DrawTextureRec(idleSheet, 
					(Rectangle){0 + (float)(idleSheet.width / 3) * (currentFrame / 20), 
					(float)(0 + ((idleSheet.height / 2) * isLeft)), (float)(idleSheet.width / 3), 
					(float)(idleSheet.height / 2)},(Vector2)GetPhysicsShapeVertex(body, 3), WHITE);
				break;
			case 1: //Running Horizontal
				DrawTextureRec(runSheet, 
					(Rectangle){0 + (float)(runSheet.width / 6) * (currentFrame / 10), 
					(float)(0 + ((runSheet.height / 2) * isLeft)), (float)(runSheet.width / 6), 
					(float)(runSheet.height / 2)},(Vector2)GetPhysicsShapeVertex(body, 3), WHITE);
				break;
			case 2:
				DrawTextureRec(jumpSheet, 
					(Rectangle){0 + (float)(jumpSheet.width / 4) * (currentFrame / 4), 
					(float)(0 + (jumpSheet.height / 2) * isLeft), (float)(jumpSheet.width / 4), 
					(float)(jumpSheet.height / 2)},(Vector2)GetPhysicsShapeVertex(body, 3), WHITE);
				break;
		}
		currentFrame ++;
	}

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        RunPhysicsStep();

        if (IsKeyPressed('R'))    // Reset physics input
        {
            // Reset movement physics body position, velocity and rotation
            body->position = (Vector2){ screenWidth/2, screenHeight/2 };
            body->velocity = (Vector2){ 0, 0 };
            SetPhysicsBodyRotation(body, 0);
        }

        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(BLACK);

            DrawFPS(screenWidth - 90, screenHeight - 30);

            // Draw created physics bodies
            int bodiesCount = GetPhysicsBodiesCount();
            for (int i = 0; i < bodiesCount; i++)
            {
                PhysicsBody body = GetPhysicsBody(i);

                int vertexCount = GetPhysicsShapeVerticesCount(i);
                for (int j = 0; j < vertexCount; j++)
                {
                    // Get physics bodies shape vertices to draw lines
                    // Note: GetPhysicsShapeVertex() already calculates rotation transformations
                    Vector2 vertexA = GetPhysicsShapeVertex(body, j);

                    int jj = (((j + 1) < vertexCount) ? (j + 1) : 0);   // Get next vertex or first to close the shape
                    Vector2 vertexB = GetPhysicsShapeVertex(body, jj);

                    DrawLineV(vertexA, vertexB, GREEN);     // Draw a line between two vertex positions
                }
            }

            DrawText("Use 'ARROWS' to move player", 10, 10, 10, WHITE);
            DrawText("Press 'R' to reset example", 10, 30, 10, WHITE);

            DrawText("Physac", logoX, logoY, 30, WHITE);
            DrawText("Powered by", logoX + 50, logoY - 7, 10, WHITE);
            drawChar(runSheet, idleSheet, jumpSheet, body);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    ClosePhysics();       // Unitialize physics

    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
