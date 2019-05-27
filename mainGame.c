#include <stdio.h>
#include <math.h>
#include <string.h>
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
//Look into newBody->shape.vertexData = CreateRectanglePolygon(pos, (Vector2){ width, height });

char fadestoBlack[3][75] = {"This is a test for Level 0\n Press Enter to Continue", "This is a test for Level 1\n Press Enter to Continue", "This is a test for Level 2\n Press Enter to Continue"};

//Global Variables
int groundLev = 495;
Vector2 charPos = {10, 495};
Vector2 backPos = {0, 0};
int currentFrame = 0;
int npcFrame = 0;
int attackFrame = 0;
int textCounter = 0;
int isLeft = FALSE;
int isJump = FALSE;
//int isSlide = FALSE;
int isAttack = FALSE;
Vector2 titlePos = {(SCREENWIDTH / 5), 544};
Vector2 mousePoint = { 0.0f, 0.0f };
int gameCheck = 0;
int scrollinMan = FALSE;
int screenNum = 0;
int levelZero[3] = {FALSE, FALSE, FALSE};
int levelOne[3] = {FALSE, FALSE, FALSE};
int levelTwo[3] = {FALSE, FALSE, FALSE};
int isText = FALSE;
int isBlack = FALSE;
int fadesToBlackIndex;

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
PhysicsBody threeBoxes;
PhysicsBody chainBridge;
PhysicsBody platformFour;
PhysicsBody ledgeThree;
PhysicsBody factoryRoof;
PhysicsBody platformFive;
PhysicsBody twoBarrels;
PhysicsBody twoBoxes;
PhysicsBody platformSix;
PhysicsBody fiveBoxes;

typedef struct
{
	Texture2D spriteSheet;
	int numFrames;
	char textBox[200];
} nonPlayerChar;
nonPlayerChar beardMan;
nonPlayerChar colonialMan;
nonPlayerChar oldMan;
nonPlayerChar oldWoman;
nonPlayerChar trueWizard;

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
	Image jump = LoadImage("images/jumpSheet1.png");
	ImageResize(&jump, (jump.width * SCALAR), (jump.height * SCALAR));
	Image attack = LoadImage("images/attackSheet.png");
	ImageResize(&attack, (attack.width * SCALAR), (attack.height * SCALAR));
	Texture2D backGround = LoadTexture("images/backSheet.png");
	Texture2D levelZeroBack = LoadTexture("images/levelZero.png");
	Texture2D levelOneBack = LoadTexture("images/levelOne.png");
	Texture2D levelTwoBack = LoadTexture("images/levelTwo.png");
	//Texture Loading
	
	Texture2D runSheet = LoadTextureFromImage(run);
	Texture2D idleSheet = LoadTextureFromImage(idle);
	Texture2D jumpSheet = LoadTextureFromImage(jump);
	Texture2D attackSheet = LoadTextureFromImage(attack);
	Texture2D titleScreen = LoadTexture("images/titleScreen.png");
	Texture2D startButtonSheet = LoadTexture("images/startButtonSheet.png");
	UnloadImage(run);
	UnloadImage(idle);
	UnloadImage(jump);
	UnloadImage(attack);
	
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
	
	void fadeToBlack()
	{
		if(!isBlack) {ClearBackground(BLACK); isBlack = TRUE;}
		switch(gameCheck)
		{
			case 1:
				fadesToBlackIndex = 0;
				break;
			case 3:
				fadesToBlackIndex = 1;
				break;
			case 5:
				fadesToBlackIndex = 2;
				break;
		}
		Rectangle container = {SCREENWIDTH / 8, SCREENHEIGHT / 6, 600, 400};
		textCounter += 8;
		DrawTextRec(GetFontDefault(), SubText(fadestoBlack[fadesToBlackIndex], 0, textCounter / 10),
					(Rectangle)container, 14, 1, true, RAYWHITE);
		if(IsKeyPressed(KEY_ENTER)) {isBlack = FALSE; gameCheck++;}
	}
	
	void textBoxDisplay(char message[50], Vector2 Position)
	{
		Rectangle container = {Position.x - 125, Position.y - 50, 225, 50};
		if (IsKeyDown(KEY_LEFT_CONTROL)) textCounter += 8;
		else textCounter += 4;
		
		DrawRectangleLinesEx(container, 3, BLACK); 
		DrawTextRec(GetFontDefault(), SubText(message, 0, textCounter/10),
			(Rectangle){container.x + 4, container.y + 4, container.width - 4, container.height - 4},
			10, 1, true, RAYWHITE);
		if (IsKeyPressed(KEY_ENTER)) {textCounter = 0; isText = FALSE;}
	}
	
	void initNonPlayerChar(nonPlayerChar NPC, Vector2 npcPosition, PhysicsBody player)
	{
		if(npcFrame >= 180) npcFrame = 0;
		DrawTextureRec(NPC.spriteSheet, (Rectangle){0 + (float)(NPC.spriteSheet.width / NPC.numFrames) * (npcFrame / (180 / NPC.numFrames)), 0, (float)(NPC.spriteSheet.width / NPC.numFrames), (float)(NPC.spriteSheet.height)},(Vector2)npcPosition, WHITE);
		Rectangle npcHitBox = {npcPosition.x, npcPosition.y, (NPC.spriteSheet.width / NPC.numFrames), NPC.spriteSheet.height};
		if(CheckCollisionPointRec(player->position, npcHitBox))
		{
			if(isText) textBoxDisplay(NPC.textBox, npcPosition);
			else
			{
				DrawText("Press Enter", npcPosition.x - 20, npcPosition.y - 10, 12, BLACK);
				if(IsKeyPressed(KEY_ENTER)) isText = TRUE;
			}
		}
		npcFrame ++;
	}
	
	int charInput(PhysicsBody player) 
	{ // Handles Input from User and Turns it into Variable
		int key;
		if(IsKeyPressed(KEY_K) && isAttack == FALSE)
		{
			key = KEY_K;
		}
		else if(IsKeyPressed(KEY_SPACE))
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
				return 2;
				break;
			case KEY_K:
				isAttack = TRUE;
				return 3;
				break;
			default:
				return 0;
		}
	}

	void drawChar(Texture2D runSheet, Texture2D idleSheet, Texture2D jumpSheet, Texture2D attackSheet, PhysicsBody player)
	{ // Takes Variable from Previous Function and Draws cooresponding sprite
		int mvmentOption = charInput(player);
		Vector2 vertexDraw = (Vector2)GetPhysicsShapeVertex(player, 3);
		if(currentFrame >= 60) {currentFrame = 0;}
		if(attackFrame >= 30) {attackFrame = 0; isAttack = FALSE;}
		if(isAttack) mvmentOption = 3;
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
			case 2:
				DrawTextureRec(jumpSheet, 
					(Rectangle){0 + (float)(jumpSheet.width / 4) * (currentFrame / 15), 
					(float)(0 + (jumpSheet.height / 2) * isLeft), (float)(jumpSheet.width / 4), 
					(float)(jumpSheet.height / 2)},(Vector2)GetPhysicsShapeVertex(player, 3), WHITE);
				break;
			case 3:
				DrawTextureRec(attackSheet, 
					(Rectangle){0 + (float)(attackSheet.width / 5) * (currentFrame / 6), 
					(float)(0 + (attackSheet.height / 2) * isLeft), (float)(attackSheet.width / 5), 
					(float)(attackSheet.height / 2)},(Vector2){vertexDraw.x - 10, vertexDraw.y - 5}, WHITE);
				attackFrame++;
				break;
				
		}
		currentFrame ++;
	}
	void startButtonFunction(Texture2D buttonSheet)
	{ // Function of Beginning Button
		UnloadTexture(buttonSheet);
		UnloadTexture(titleScreen);
		gameCheck ++;
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
	void backParallax(Texture2D runSheet, Texture2D backSheet, PhysicsBody player)
	{ // Background movement according to screenNum (0-2)
		DrawTexture(backSheet, backPos.x, backPos.y, WHITE);
		if(scrollinMan == 1 || scrollinMan == 2)
		{
			if(scrollinMan == 2 && screenNum > 0 && screenNum <= 2) 
			{
				backPos.x += BACKVELOCITY;
				if(player->position.x < (SCREENWIDTH - 50) && backPos.x == (-1 * SCREENWIDTH * (screenNum - 1))) 
				{ 
					player->position.x = (SCREENWIDTH - 50);
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
					player->position.x = 50;
					player->velocity = (Vector2){0, 0};
					SetPhysicsBodyRotation(player, 0);
					screenNum ++;
					scrollinMan = FALSE;
				}
			}
		}
		if(player->position.x >= (SCREENWIDTH - (float)(runSheet.width / 6)) && screenNum < 2) {DrawText("Press Enter to Continue", (SCREENWIDTH - 180), 470, 15, BLACK); if(IsKeyPressed(KEY_ENTER)) scrollinMan = TRUE;}
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
					UnloadTexture(beardMan.spriteSheet);
					UnloadTexture(colonialMan.spriteSheet);
					levelZero[screenNum - 1] = FALSE;
					isText = FALSE;
					break;
				case 1:
					DestroyPhysicsBody(threeBarrels);
					DestroyPhysicsBody(oneBarrel);
					DestroyPhysicsBody(ledgeTwo);
					DestroyPhysicsBody(oneBox);
					UnloadTexture(oldMan.spriteSheet);
					UnloadTexture(oldWoman.spriteSheet);
					levelZero[screenNum - 1] = FALSE;
					isText = FALSE;
					break;
				case 2:
					DestroyPhysicsBody(platformTwo);
					DestroyPhysicsBody(platformThree);
					UnloadTexture(trueWizard.spriteSheet);
					levelZero[screenNum - 1] = FALSE;
					isText = FALSE;
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
					UnloadTexture(beardMan.spriteSheet);
					UnloadTexture(colonialMan.spriteSheet);
					levelZero[screenNum + 1] = FALSE;
					isText = FALSE;
					break;
				case 1:
					DestroyPhysicsBody(threeBarrels);
					DestroyPhysicsBody(oneBarrel);
					DestroyPhysicsBody(ledgeTwo);
					DestroyPhysicsBody(oneBox);
					UnloadTexture(oldMan.spriteSheet);
					UnloadTexture(oldWoman.spriteSheet);
					levelZero[screenNum + 1] = FALSE;
					isText = FALSE;
					break;
				case 2:
					DestroyPhysicsBody(platformTwo);
					DestroyPhysicsBody(platformThree);
					UnloadTexture(trueWizard.spriteSheet);
					levelZero[screenNum + 1] = FALSE;
					isText = FALSE;
					break;
			}
		}
		else if(levelZero[screenNum + 2])
		{
			switch(screenNum + 2)
			{
				case 0:
					DestroyPhysicsBody(bottomBoxes);
					DestroyPhysicsBody(topBox);
					DestroyPhysicsBody(ledge);
					DestroyPhysicsBody(platform);
					UnloadTexture(beardMan.spriteSheet);
					UnloadTexture(colonialMan.spriteSheet);
					levelZero[screenNum + 2] = FALSE;
					isText = FALSE;
					break;
				case 1:
					DestroyPhysicsBody(threeBarrels);
					DestroyPhysicsBody(oneBarrel);
					DestroyPhysicsBody(ledgeTwo);
					DestroyPhysicsBody(oneBox);
					UnloadTexture(oldMan.spriteSheet);
					UnloadTexture(oldWoman.spriteSheet);
					levelZero[screenNum + 1] = FALSE;
					isText = FALSE;
					break;
				case 2:
					DestroyPhysicsBody(platformTwo);
					DestroyPhysicsBody(platformThree);
					UnloadTexture(trueWizard.spriteSheet);
					levelZero[screenNum + 1] = FALSE;
					isText = FALSE;
					break;
			}
		}
	}
	void levelZeroInit(PhysicsBody player)
	{ // Initializes the current screen on Level 0
		if(!(levelZero[screenNum]))
		{
			switch(screenNum)
			{
				case 0:
					levelZeroDeInit();
					bottomBoxes = CreatePhysicsBodyRectangle((Vector2){190, 544}, 73, 33, 10);
					topBox = CreatePhysicsBodyRectangle((Vector2){190, 511}, 39, 33, 10);
					ledge = CreatePhysicsBodyRectangle((Vector2){638.5, 524}, 48, 13, 10);
					platform = CreatePhysicsBodyRectangle((Vector2){731.5, 477}, 128, 5, 10);
					bottomBoxes->enabled = false;
					topBox->enabled = false;
					ledge->enabled = false;
					platform->enabled = false;
					beardMan = (nonPlayerChar){LoadTexture("images/bearded-idle.png"), 5, "This curse from God is bad for business!"};
					colonialMan = (nonPlayerChar){LoadTexture("images/colonialMan.png"), 4, "If one more family member dies, I'm jumping!"};
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
					oldMan = (nonPlayerChar){LoadTexture("images/oldman-idle.png"), 8, "Man, I hate carrying this medicine all over town . . ."};
					oldWoman = (nonPlayerChar){LoadTexture("images/woman-idle.png"), 7, "Why does he have to complain so much?"};
					levelZero[screenNum] = TRUE;
					break;
				case 2:
					levelZeroDeInit();
					platformTwo = CreatePhysicsBodyRectangle((Vector2){359, 518}, 95, 5, 10);
					platformThree = CreatePhysicsBodyRectangle((Vector2){466, 472}, 95, 5, 10);
					platformTwo->enabled = false;
					platformThree->enabled = false;
					trueWizard = (nonPlayerChar){LoadTexture("images/wizardSheet.png"), 10, "Welcome, my child, to the beginning of your journey. I am Jerry, and I wish to show you the future humanity takes. Press Y."};
					levelZero[screenNum] = TRUE;
					break;
			}
		}
		switch(screenNum)
		{
			case 0:
				if(scrollinMan == FALSE)
				{
					initNonPlayerChar(beardMan, (Vector2){385, 520}, player);
					initNonPlayerChar(colonialMan, (Vector2){692, 422}, player);
				}
				break;
			case 1:
				if(scrollinMan == FALSE)
				{
					initNonPlayerChar(oldMan, (Vector2){380, 522}, player);
					initNonPlayerChar(oldWoman, (Vector2){320, 518}, player);
				}
				break;
			case 2:
				if(scrollinMan == FALSE)
				{
					initNonPlayerChar(trueWizard, (Vector2){672, 484}, player);
					if(IsKeyPressed(KEY_Y) && (isText == TRUE)) 
					{
						screenNum = 0; 
						levelZeroDeInit();
						isText = FALSE;
						textCounter = 0;
						player->position.x = 50;
						player->velocity = (Vector2){0, 0};
						SetPhysicsBodyRotation(player, 0);
						backPos.x = 0;
						gameCheck++;
					}
				}
				break;
		}
	}
		
	void levelOneDeInit()
	{ //De-initializes the screen to move on to the next on Level 1
		if(levelOne[screenNum - 1])
		{
			switch(screenNum - 1)
			{
				case 0:
					DestroyPhysicsBody(threeBoxes);
					DestroyPhysicsBody(oneBox);
					DestroyPhysicsBody(chainBridge);
					DestroyPhysicsBody(platformFour);
					DestroyPhysicsBody(ledgeThree);
					DestroyPhysicsBody(factoryRoof);
					levelOne[screenNum - 1] = FALSE;
					isText = FALSE;
					break;
				case 1:
					DestroyPhysicsBody(platformFive);
					DestroyPhysicsBody(twoBarrels);
					DestroyPhysicsBody(oneBarrel);
					levelOne[screenNum - 1] = FALSE;
					isText = FALSE;
					break;
				case 2:
					DestroyPhysicsBody(twoBoxes);
					DestroyPhysicsBody(oneBox);
					levelOne[screenNum - 1] = FALSE;
					isText = FALSE;
					break;
			}
		}
		else if(levelOne[screenNum + 1])
		{
			switch(screenNum + 1)
			{
				case 0:
					DestroyPhysicsBody(threeBoxes);
					DestroyPhysicsBody(oneBox);
					DestroyPhysicsBody(chainBridge);
					DestroyPhysicsBody(platformFour);
					DestroyPhysicsBody(ledgeThree);
					DestroyPhysicsBody(factoryRoof);
					levelOne[screenNum + 1] = FALSE;
					isText = FALSE;
					break;
				case 1:
					DestroyPhysicsBody(platformFive);
					DestroyPhysicsBody(twoBarrels);
					DestroyPhysicsBody(oneBarrel);
					levelOne[screenNum + 1] = FALSE;
					isText = FALSE;
					break;
				case 2:
					DestroyPhysicsBody(twoBoxes);
					DestroyPhysicsBody(oneBox);
					levelOne[screenNum + 1] = FALSE;
					isText = FALSE;
					break;
			}
		}
		else if(levelOne[screenNum + 2])
		{
			switch(screenNum + 2)
			{
				case 0:
					DestroyPhysicsBody(threeBoxes);
					DestroyPhysicsBody(oneBox);
					DestroyPhysicsBody(chainBridge);
					DestroyPhysicsBody(platformFour);
					DestroyPhysicsBody(ledgeThree);
					DestroyPhysicsBody(factoryRoof);
					levelOne[screenNum + 2] = FALSE;
					isText = FALSE;
					break;
				case 1:
					DestroyPhysicsBody(platformFive);
					DestroyPhysicsBody(twoBarrels);
					DestroyPhysicsBody(oneBarrel);
					levelOne[screenNum + 1] = FALSE;
					isText = FALSE;
					break;
				case 2:
					DestroyPhysicsBody(twoBoxes);
					DestroyPhysicsBody(oneBox);
					UnloadTexture(trueWizard.spriteSheet);
					levelOne[screenNum + 1] = FALSE;
					isText = FALSE;
					break;
			}
		}
	}
	void levelOneInit(PhysicsBody player)
	{ // Initializes the current screen on Level 1
		if(!(levelOne[screenNum]))
		{
			switch(screenNum)
			{
				case 0:
					levelOneDeInit();
					threeBoxes = CreatePhysicsBodyRectangle((Vector2){498, 547.5}, 103, 33, 10);
					oneBox = CreatePhysicsBodyRectangle((Vector2){514, 515}, 39, 33, 10);
					chainBridge = CreatePhysicsBodyRectangle((Vector2){775, 484.5}, 51, 6, 10);
					platformFour = CreatePhysicsBodyRectangle((Vector2){677, 480}, 145, 5, 10);
					ledgeThree = CreatePhysicsBodyRectangle((Vector2){539.5, 423.5}, 32, 6, 10);
					factoryRoof = CreatePhysicsBodyRectangle((Vector2){308.5, 380}, 462, 13, 10);
					threeBoxes->enabled = false;
					oneBox->enabled = false;
					chainBridge->enabled = false;
					platformFour->enabled = false;
					ledgeThree->enabled = false;
					factoryRoof->enabled = false;
					levelOne[screenNum] = TRUE;
					break;
				case 1:
					levelOneDeInit();
					platformFive = CreatePhysicsBodyRectangle((Vector2){122.5, 484.5}, 246, 6, 10);
					twoBarrels = CreatePhysicsBodyRectangle((Vector2){271, 550}, 47, 27, 10);
					oneBarrel = CreatePhysicsBodyRectangle((Vector2){485.5, 550}, 24, 27, 10);
					platformFive->enabled = false;
					twoBarrels->enabled = false;
					oneBarrel->enabled = false;
					levelOne[screenNum] = TRUE;
					break;
				case 2:
					levelOneDeInit();
					twoBoxes = CreatePhysicsBodyRectangle((Vector2){143, 547}, 73, 33, 10);
					oneBox = CreatePhysicsBodyRectangle((Vector2){142, 514}, 39, 33, 10);
					twoBoxes->enabled = false;
					oneBox->enabled = false;
					trueWizard = (nonPlayerChar){LoadTexture("images/wizardSheet.png"), 10, "Welcome, my child, to the beginning of your journey. I am Jerry, and I wish to show you the future humanity takes. Press Y."};
					levelOne[screenNum] = TRUE;
					break;
			}
		}
		
		switch(screenNum)
		{
			case 0:
				if(scrollinMan == FALSE)
				{

				}
				break;
			case 1:
				if(scrollinMan == FALSE)
				{

				}
				break;
			case 2:
				if(scrollinMan == FALSE)
				{
					initNonPlayerChar(trueWizard, (Vector2){672, 484}, player);
					if(IsKeyPressed(KEY_Y) && (isText == TRUE)) 
					{
						screenNum = 0; 
						levelOneDeInit();
						isText = FALSE;
						textCounter = 0;
						player->position.x = 50;
						player->velocity = (Vector2){0, 0};
						SetPhysicsBodyRotation(player, 0);
						backPos.x = 0;
						gameCheck++;
					}
				}
				break;
		}
	}
	
	void levelTwoDeInit()
	{ //De-initializes the screen to move on to the next on Level 2
		if(levelTwo[screenNum - 1])
		{
			switch(screenNum - 1)
			{
				case 0:
					DestroyPhysicsBody(platformSix);
					DestroyPhysicsBody(twoBoxes);
					DestroyPhysicsBody(oneBox);
					levelTwo[screenNum - 1] = FALSE;
					isText = FALSE;
					break;
				case 1:
					DestroyPhysicsBody(fiveBoxes);
					DestroyPhysicsBody(threeBoxes);
					DestroyPhysicsBody(topBox);
					levelTwo[screenNum - 1] = FALSE;
					isText = FALSE;
					break;
				case 2:

					levelTwo[screenNum - 1] = FALSE;
					isText = FALSE;
					break;
			}
		}
		else if(levelTwo[screenNum + 1])
		{
			switch(screenNum + 1)
			{
				case 0:
					DestroyPhysicsBody(platformSix);
					DestroyPhysicsBody(twoBoxes);
					DestroyPhysicsBody(oneBox);
					levelTwo[screenNum + 1] = FALSE;
					isText = FALSE;
					break;
				case 1:
					DestroyPhysicsBody(fiveBoxes);
					DestroyPhysicsBody(threeBoxes);
					DestroyPhysicsBody(topBox);
					levelTwo[screenNum + 1] = FALSE;
					isText = FALSE;
					break;
				case 2:

					levelTwo[screenNum + 1] = FALSE;
					isText = FALSE;
					break;
			}
		}
	}
	void levelTwoInit(PhysicsBody player)
	{ // Initializes the current screen on Level 2
		if(!(levelTwo[screenNum]))
		{
			switch(screenNum)
			{
				case 0:
					levelTwoDeInit();
					platformSix = CreatePhysicsBodyRectangle((Vector2){255.5, 467.5}, 186, 6, 10);
					twoBoxes = CreatePhysicsBodyRectangle((Vector2){413, 548}, 73, 33, 10);
					oneBox = CreatePhysicsBodyRectangle((Vector2){414, 515}, 39, 33, 10);
					platformSix->enabled = false;
					twoBoxes->enabled = false;
					oneBox->enabled = false;
					levelTwo[screenNum] = TRUE;
					break;
				case 1:
					levelTwoDeInit();
					fiveBoxes = CreatePhysicsBodyRectangle((Vector2){388, 546}, 159, 33, 10);
					threeBoxes = CreatePhysicsBodyRectangle((Vector2){401.5, 513}, 98, 33, 10);
					topBox = CreatePhysicsBodyRectangle((Vector2){404, 480}, 39, 33, 10);
					fiveBoxes->enabled = false;
					threeBoxes->enabled = false;
					topBox->enabled = false;
					levelTwo[screenNum] = TRUE;
					break;
				case 2:
					levelTwoDeInit();
					
					levelTwo[screenNum] = TRUE;
					break;
			}
		}
		
		switch(screenNum)
		{
			case 0:
				if(scrollinMan == FALSE)
				{

				}
				break;
			case 1:
				if(scrollinMan == FALSE)
				{

				}
				break;
			case 2:
				if(scrollinMan == FALSE)
				{

				}
				break;
		}
	}
	
	while(!WindowShouldClose())
	{
		RunPhysicsStep();
		BeginDrawing();
		switch(gameCheck){
			case 0:
				ClearBackground(RAYWHITE);
				DrawTexture(backGround, backPos.x, backPos.y, WHITE);
				if(!titleDisplay(titleScreen))
				{
					buttonFunction(startButtonSheet, (Vector2){SCREENWIDTH / 2 - startButtonSheet.width / 2, SCREENHEIGHT / 2 - startButtonSheet.height / 3 / 2}, 1);
				}
				break;
			case 1:
				fadeToBlack();
				break;
			case 2:
				backParallax(runSheet, levelZeroBack, charBody);
				if(scrollinMan == FALSE) drawChar(runSheet, idleSheet, jumpSheet, attackSheet, charBody);
				levelZeroInit(charBody);
				break;
			case 3:
				fadeToBlack();
				break;
			case 4:
				backParallax(runSheet, levelOneBack, charBody);
				if(scrollinMan == FALSE) drawChar(runSheet, idleSheet, jumpSheet, attackSheet, charBody);
				levelOneInit(charBody);
				break;
			case 5:
				fadeToBlack();
				break;
			case 6:
				backParallax(runSheet, levelTwoBack, charBody);
				if(scrollinMan == FALSE) drawChar(runSheet, idleSheet, jumpSheet, attackSheet, charBody);
				levelTwoInit(charBody);
				break;
		}
		DrawFPS(5, 5);
		EndDrawing();
	}
	CloseWindow();
    return 0;
}
