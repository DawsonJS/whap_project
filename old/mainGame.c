#include "logic.h"

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
	//Bosses
	Image cyclopsWalk = LoadImage("images/cyclopsWalkSheet.png");
	ImageResize(&cyclopsWalk, (cyclopsWalk.width * SCALAR), (cyclopsWalk.height * SCALAR));
	Texture2D cyclopsWalkSheet = LoadTextureFromImage(cyclopsWalk);
	Image cyclopsDeath = LoadImage("images/cyclopsDeathSheet.png");
	ImageResize(&cyclopsDeath, (cyclopsDeath.width * SCALAR), (cyclopsDeath.height * SCALAR));
	Texture2D cyclopsDeathSheet = LoadTextureFromImage(cyclopsDeath);
	Image flaminSkull = LoadImage("images/skullWalkSheet.png");
	ImageResize(&flaminSkull, (flaminSkull.width * SCALAR), (flaminSkull.height * SCALAR));
	Texture2D flaminSkullSheet = LoadTextureFromImage(flaminSkull);
	Image miniMino = LoadImage("images/miniMinotaurWalkSheet.png");
	ImageResize(&miniMino, (miniMino.width * SCALAR), (miniMino.height * SCALAR));
	Texture2D miniMinotaurWalkSheet = LoadTextureFromImage(miniMino);
	Image miniNoMino = LoadImage("images/miniMinotaurDeathSheet.png");
	ImageResize(&miniNoMino, (miniNoMino.width * SCALAR), (miniNoMino.height * SCALAR));
	Texture2D miniMinotaurDeathSheet = LoadTextureFromImage(miniNoMino);
	
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
	UnloadImage(cyclopsWalk);
	UnloadImage(cyclopsDeath);
	UnloadImage(flaminSkull);
	UnloadImage(miniMino);
	UnloadImage(miniNoMino);
	
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
	
	InitAudioDevice();
	Music titleMusic = LoadMusicStream("sounds/celtic_impulse.ogg");
	Music levelZeroMusic = LoadMusicStream("sounds/myst.ogg");
	Music levelOneMusic = LoadMusicStream("sounds/industrial_revolution.ogg");
	Music levelTwoMusic = LoadMusicStream("sounds/miris_magic_dance.ogg");
	Music bossMusic = LoadMusicStream("sounds/medieval_myths_19.ogg");
	SetMusicVolume(titleMusic, 0.6);
	SetMusicVolume(levelZeroMusic, 0.6);
	SetMusicVolume(levelOneMusic, 0.6);
	SetMusicVolume(levelTwoMusic, 0.6);
	SetMusicVolume(bossMusic, 0.6);
	Sound hitSound = LoadSound("sounds/hitSound.ogg");
	Sound runSound = LoadSound("sounds/runSound.ogg");
	cursorSound = LoadSound("sounds/cursorSound.ogg");
	SetSoundVolume(cursorSound, 0.015);
	SetSoundVolume(hitSound, 0.05);

	void textBoxDisplay(char message[600], Vector2 Position)
	{ // Displays text box for each character
		Rectangle container = {Position.x - 125, Position.y - 50, 225, 50};
		
		if(!((textCounter / 10) >= 120)) {textCounter += 6; if(!(IsSoundPlaying(cursorSound)) && !((textCounter / 10) >= strlen(message))) PlaySound(cursorSound);}
		else if(isContinue && !(textCounter >= (1200 * pageNum))) {textCounter += 6; if(!(IsSoundPlaying(cursorSound)) && !((textCounter / 10) >= (1200 * pageNum))) PlaySound(cursorSound);}
		
		DrawRectangleLinesEx(container, 3, BLACK); 
		DrawTextRec(GetFontDefault(), SubText(message, textStart, textCounter/10),
			(Rectangle){container.x + 4, container.y + 4, container.width - 4, container.height - 4},
			10, 1, true, RAYWHITE);
		if(IsKeyPressed(KEY_ENTER) && textCounter >= (strlen(message) * 10)) {textCounter = 0; isText = FALSE; isContinue = FALSE; pageNum = 1; textStart = 0;}
		else if(IsKeyPressed(KEY_ENTER) && !(textCounter >= (strlen(message) * 10))) {isContinue = TRUE; pageNum++; textStart += 110;}
	}
	void initNonPlayerChar(nonPlayerChar NPC, Vector2 npcPosition, PhysicsBody player)
	{ // Draws and places NPCs into levels
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
	void displayHealthBar(float percentHealth, Vector2 objectPosition)
	{ // Displays health bar over boss
		Rectangle barOHealth = {objectPosition.x, objectPosition.y - 10, 96, 10};
		DrawRectangleLinesEx(barOHealth, 3, BLACK);
		DrawRectangleRec((Rectangle){objectPosition.x + 3, objectPosition.y - 7, (float)(90 * percentHealth), 4}, RED);
	}
	enemyBoss initEnemyBoss(enemyBoss bossEnemy, PhysicsBody player)
	{ // Initializes Boss Enemy for each level
		float percentHealth = (float)(bossEnemy.actualHealth / bossEnemy.maxHealth);
		displayHealthBar(percentHealth, (Vector2)bossEnemy.enemyPosition);
		if(enemyFrame >= 60) enemyFrame = 0;
		if(bossEnemy.isDead)
		{
			DrawTextureRec(bossEnemy.deathSheet, 
				(Rectangle){0 + (float)(bossEnemy.deathSheet.width / bossEnemy.deathFrames) * 
				deathFrame, 0 + (bossEnemy.deathSheet.height / 2) * enemyIsLeft, 
				(float)(bossEnemy.deathSheet.width / bossEnemy.deathFrames), (float)(bossEnemy.deathSheet.height / 2)}, (Vector2)bossEnemy.enemyPosition, WHITE);
			deathFrame ++;
		}
		else
		{
			DrawTextureRec(bossEnemy.walkSheet, 
				(Rectangle){0 + (float)(bossEnemy.walkSheet.width / bossEnemy.walkFrames) * 
				(enemyFrame / (60 / bossEnemy.walkFrames)), 0 + (bossEnemy.walkSheet.height / 2) * enemyIsLeft, 
				(float)(bossEnemy.walkSheet.width / bossEnemy.walkFrames), (float)(bossEnemy.walkSheet.height / 2)}, (Vector2)bossEnemy.enemyPosition, WHITE);
			if(bossEnemy.enemyPosition.x >= (SCREENWIDTH - 100)) enemyIsLeft = TRUE;
			if(bossEnemy.enemyPosition.x <= 20) enemyIsLeft = FALSE;
			if(enemyIsLeft) bossEnemy.enemyPosition.x -= ENEMYVELOCITY;
			else bossEnemy.enemyPosition.x += ENEMYVELOCITY;
		}
		Rectangle enemyHitBox = {bossEnemy.enemyPosition.x, bossEnemy.enemyPosition.y, (bossEnemy.walkSheet.width / bossEnemy.walkFrames), (bossEnemy.walkSheet.height / 2)};
		if(CheckCollisionPointRec(player->position, enemyHitBox))
		{
			if(isAttack) bossEnemy.actualHealth -= 1;
			if(bossEnemy.actualHealth <= 0) bossEnemy.isDead = TRUE;
			if((float)((float)enemyFrame / (float)2) == 0) playerHealth -= 1;
		}
		enemyFrame ++;
		return bossEnemy;
	}
	int charInput(PhysicsBody player) 
	{ // Handles Input from User and Turns it into Variable
		int key;
		if(IsKeyPressed(KEY_K) && isAttack == FALSE)
		{
			key = KEY_K;
			PlaySound(hitSound);
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
				if(!(IsSoundPlaying(runSound))) PlaySound(runSound);
				if(IsKeyDown(KEY_LEFT_CONTROL)) {player->velocity.x = VELOCITY * 2;}
				else {player->velocity.x = VELOCITY;}
				return 1;
				break;
			case KEY_LEFT:
				isLeft = TRUE;
				if(!(IsSoundPlaying(runSound))) PlaySound(runSound);
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
			titlePos.y -= 1;
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
					textCounter = 0;
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
					textCounter = 0;
					break;
				case 2:
					DestroyPhysicsBody(platformTwo);
					DestroyPhysicsBody(platformThree);
					UnloadTexture(trueWizard.spriteSheet);
					levelZero[screenNum - 1] = FALSE;
					isText = FALSE;
					textCounter = 0;
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
					textCounter = 0;
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
					textCounter = 0;
					break;
				case 2:
					DestroyPhysicsBody(platformTwo);
					DestroyPhysicsBody(platformThree);
					UnloadTexture(trueWizard.spriteSheet);
					levelZero[screenNum + 1] = FALSE;
					isText = FALSE;
					textCounter = 0;
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
					textCounter = 0;
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
					textCounter = 0;
					break;
				case 2:
					DestroyPhysicsBody(platformTwo);
					DestroyPhysicsBody(platformThree);
					UnloadTexture(trueWizard.spriteSheet);
					levelZero[screenNum + 1] = FALSE;
					isText = FALSE;
					textCounter = 0;
					break;
			}
		}
	}
	void levelZeroInit(PhysicsBody player, Texture2D cyclopsWalkSheet, Texture2D cyclopsDeathSheet)
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
					trueWizard = (nonPlayerChar){LoadTexture("images/wizardSheet.png"), 10, "Hello young adventurer! Thank you for meeting me here! I apologize for the strange cyclops you had to fight, but you did good! That was a man from the future named Adolf Hitler, a truly nasty man who can only see his way. You see, there are times when people and objects slip through the fabrics of reality, only to become exaggerated versions of themselves until they are defeated and sent back. Do not fret, however,with my aid you will be in great care! Now, onward to the Industrial Revolution! Press K to Continue"};
					cyclopsBoss = (enemyBoss){cyclopsWalkSheet, cyclopsDeathSheet, 12, 9, FALSE, (Vector2){672, 510}, 250, 250};
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
				if(!(IsMusicPlaying(levelZeroMusic))) PlayMusicStream(levelZeroMusic);
				UpdateMusicStream(levelZeroMusic);
				break;
			case 1:
				if(scrollinMan == FALSE)
				{
					initNonPlayerChar(oldMan, (Vector2){380, 522}, player);
					initNonPlayerChar(oldWoman, (Vector2){320, 518}, player);
				}
				if(!(IsMusicPlaying(levelZeroMusic))) PlayMusicStream(levelZeroMusic);
				UpdateMusicStream(levelZeroMusic);
				break;
			case 2:
				if(scrollinMan == FALSE)
				{
					if(cyclopsBoss.isDead && deathFrame == 9)
					{
						if(!(IsMusicPlaying(levelZeroMusic))) PlayMusicStream(levelZeroMusic);
						UpdateMusicStream(levelZeroMusic);
						initNonPlayerChar(trueWizard, (Vector2){672, 484}, player);
						if(IsKeyPressed(KEY_K) && (isText == TRUE)) 
						{
							screenNum = 0;
							levelZeroDeInit();
							isText = FALSE;
							textCounter = 0;
							player->position.x = 50;
							player->velocity = (Vector2){0, 0};
							SetPhysicsBodyRotation(player, 0);
							backPos.x = 0;
							deathFrame = 0;
							playerHealth = maxPlayerHealth;
							gameCheck++;
						}
					}
					else
					{
						if(!(IsMusicPlaying(bossMusic))) PlayMusicStream(bossMusic);
						UpdateMusicStream(bossMusic);
						cyclopsBoss = initEnemyBoss(cyclopsBoss, player);
						if(playerHealth <= 0) 
                        {
                            
                            charBody->position.x = 50;
                            charBody->velocity = (Vector2){0, 0};
                            SetPhysicsBodyRotation(charBody, 0);
                            fadeToBlack();
                        }
						displayHealthBar((float)(playerHealth / maxPlayerHealth), (Vector2)GetPhysicsShapeVertex(player, 3));
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
					UnloadTexture(oldWoman.spriteSheet);
					UnloadTexture(colonialMan.spriteSheet);
					levelOne[screenNum - 1] = FALSE;
					isText = FALSE;
					break;
				case 1:
					DestroyPhysicsBody(platformFive);
					DestroyPhysicsBody(twoBarrels);
					DestroyPhysicsBody(oneBarrel);
					UnloadTexture(beardMan.spriteSheet);
					UnloadTexture(childMan.spriteSheet);
					levelOne[screenNum - 1] = FALSE;
					isText = FALSE;
					break;
				case 2:
					DestroyPhysicsBody(twoBoxes);
					DestroyPhysicsBody(oneBox);
					UnloadTexture(trueWizard.spriteSheet);
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
					UnloadTexture(oldWoman.spriteSheet);
					UnloadTexture(colonialMan.spriteSheet);
					levelOne[screenNum + 1] = FALSE;
					isText = FALSE;
					break;
				case 1:
					DestroyPhysicsBody(platformFive);
					DestroyPhysicsBody(twoBarrels);
					DestroyPhysicsBody(oneBarrel);
					UnloadTexture(beardMan.spriteSheet);
					UnloadTexture(childMan.spriteSheet);
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
					UnloadTexture(oldWoman.spriteSheet);
					UnloadTexture(colonialMan.spriteSheet);
					levelOne[screenNum + 2] = FALSE;
					isText = FALSE;
					break;
				case 1:
					DestroyPhysicsBody(platformFive);
					DestroyPhysicsBody(twoBarrels);
					DestroyPhysicsBody(oneBarrel);
					UnloadTexture(beardMan.spriteSheet);
					UnloadTexture(childMan.spriteSheet);
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
					colonialMan = (nonPlayerChar){LoadTexture("images/colonialMan.png"), 4, "If one more puff of smoke comes out, I'm jumping!"};
					oldWoman = (nonPlayerChar){LoadTexture("images/woman-idle.png"), 7, "That poor man's family is known for giving up. . ."};
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
					beardMan = (nonPlayerChar){LoadTexture("images/bearded-idle.png"), 5, "Innovation is bad for business!"};
					childMan = (nonPlayerChar){LoadTexture("images/ordMan.png"), 5, "Please, sir, may I have some more?"};
					levelOne[screenNum] = TRUE;
					break;
				case 2:
					levelOneDeInit();
					twoBoxes = CreatePhysicsBodyRectangle((Vector2){143, 547}, 73, 33, 10);
					oneBox = CreatePhysicsBodyRectangle((Vector2){142, 514}, 39, 33, 10);
					twoBoxes->enabled = false;
					oneBox->enabled = false;
					trueWizard = (nonPlayerChar){LoadTexture("images/wizardSheet.png"), 10, "Ah, there you are! I was beginning to wonder what all the commotion was about. This creature was quite strange, hmm? It seems this one was from the same time period as the previous. It appears to have been Benito Mussolini, fitting, as they were both angry bald men. Well, are you ready for the next trip? We are going to these creatures' time, World War II! Press K to Continue"};
					flaminSkullBoss = (enemyBoss){flaminSkullSheet, flaminSkullSheet, 16, 16, FALSE, (Vector2){672, 484}, 250, 250};
					levelOne[screenNum] = TRUE;
					break;
			}
		}
		
		switch(screenNum)
		{
			case 0:
				if(scrollinMan == FALSE)
				{
					initNonPlayerChar(oldWoman, (Vector2){395, 518}, player);
					initNonPlayerChar(colonialMan, (Vector2){640, 428}, player);
				}
				if(!(IsMusicPlaying(levelOneMusic))) PlayMusicStream(levelOneMusic);
				UpdateMusicStream(levelOneMusic);
				break;
			case 1:
				if(scrollinMan == FALSE)
				{
					initNonPlayerChar(beardMan, (Vector2){110, 518}, player);
					initNonPlayerChar(childMan, (Vector2){220, 482 - childMan.spriteSheet.height}, player);
				}
				if(!(IsMusicPlaying(levelOneMusic))) PlayMusicStream(levelOneMusic);
				UpdateMusicStream(levelOneMusic);
				break;
			case 2:
				if(scrollinMan == FALSE)
				{
					if(flaminSkullBoss.isDead && deathFrame == 16)
					{
						if(!(IsMusicPlaying(levelOneMusic))) PlayMusicStream(levelOneMusic);
						UpdateMusicStream(levelOneMusic);
						initNonPlayerChar(trueWizard, (Vector2){672, 484}, player);
						if(IsKeyPressed(KEY_K) && (isText == TRUE)) 
						{
							screenNum = 0;
							levelOneDeInit();
							isText = FALSE;
							textCounter = 0;
							player->position.x = 50;
							player->velocity = (Vector2){0, 0};
							SetPhysicsBodyRotation(player, 0);
							backPos.x = 0;
							deathFrame = 0;
							playerHealth = maxPlayerHealth;
							gameCheck++;
						}
					}
					else
					{
						if(!(IsMusicPlaying(bossMusic))) PlayMusicStream(bossMusic);
						UpdateMusicStream(bossMusic);
						displayHealthBar((float)(playerHealth / maxPlayerHealth), (Vector2)GetPhysicsShapeVertex(player, 3));
						if(playerHealth <= 0) 
                        {
                            charBody->position.x = 50;
                            charBody->velocity = (Vector2){0, 0};
                            SetPhysicsBodyRotation(charBody, 0);
                            fadeToBlack();
                        }
						flaminSkullBoss = initEnemyBoss(flaminSkullBoss, player);
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
					UnloadTexture(beardMan.spriteSheet);
					UnloadTexture(childMan.spriteSheet);
					isText = FALSE;
					break;
				case 1:
					DestroyPhysicsBody(fiveBoxes);
					DestroyPhysicsBody(threeBoxes);
					DestroyPhysicsBody(topBox);
					UnloadTexture(popo1.spriteSheet);
					UnloadTexture(popo2.spriteSheet);
					levelTwo[screenNum - 1] = FALSE;
					isText = FALSE;
					break;
				case 2:
					UnloadTexture(trueWizard.spriteSheet);
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
					UnloadTexture(beardMan.spriteSheet);
					UnloadTexture(childMan.spriteSheet);
					levelTwo[screenNum + 1] = FALSE;
					isText = FALSE;
					break;
				case 1:
					DestroyPhysicsBody(fiveBoxes);
					DestroyPhysicsBody(threeBoxes);
					DestroyPhysicsBody(topBox);
					UnloadTexture(popo1.spriteSheet);
					UnloadTexture(popo2.spriteSheet);
					levelTwo[screenNum + 1] = FALSE;
					isText = FALSE;
					break;
				case 2:
					UnloadTexture(trueWizard.spriteSheet);
					levelTwo[screenNum + 1] = FALSE;
					isText = FALSE;
					break;
			}
		}
		else if(levelTwo[screenNum + 2])
		{
			switch(screenNum + 2)
			{
				case 0:
					DestroyPhysicsBody(platformSix);
					DestroyPhysicsBody(twoBoxes);
					DestroyPhysicsBody(oneBox);
					levelTwo[screenNum + 2] = FALSE;
					UnloadTexture(beardMan.spriteSheet);
					UnloadTexture(childMan.spriteSheet);
					levelTwo[screenNum + 2] = FALSE;
					isText = FALSE;
					break;
				case 1:
					DestroyPhysicsBody(fiveBoxes);
					DestroyPhysicsBody(threeBoxes);
					DestroyPhysicsBody(topBox);
					UnloadTexture(popo1.spriteSheet);
					UnloadTexture(popo2.spriteSheet);
					levelTwo[screenNum + 2] = FALSE;
					isText = FALSE;
					break;
				case 2:
					UnloadTexture(trueWizard.spriteSheet);
					levelTwo[screenNum + 2] = FALSE;
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
					beardMan = (nonPlayerChar){LoadTexture("images/bearded-idle.png"), 5, "No one respects my cooking business!"};
					childMan = (nonPlayerChar){LoadTexture("images/ordMan.png"), 5, "I may not look it, but I'm a cryptographer."};
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
					Image popo = LoadImage("images/popoIdle.png");
					ImageResize(&popo, (popo.width * 2), (popo.height * 2));
					popo1 = (nonPlayerChar){LoadTextureFromImage(popo), 7, "We are in a investigation of Alan Turing."};
					popo2 = (nonPlayerChar){LoadTextureFromImage(popo), 7, "We have reason to believe he might be a homosexual."};
					UnloadImage(popo);
					levelTwo[screenNum] = TRUE;
					break;
				case 2:
					levelTwoDeInit();
					trueWizard = (nonPlayerChar){LoadTexture("images/wizardSheet.png"), 10, "Welcome back! I saw you defeated another time anomaly! I have no clue who this might be, it's possible it could be from a farther time than I know. Well, anyway, I hope this has given you closure to your preconceived notions of humanity. It seems no matter what you throw at our species, wide disease, great wars, we shall prevail and evolve into even better than before. I guess the moral of the story is . . . When you hit rock bottom all you can go is up. Or to just have fun! Thanks for playing! Press K to Continue"};
					miniMinotaurBoss = (enemyBoss){miniMinotaurWalkSheet, miniMinotaurDeathSheet, 8, 6, FALSE, (Vector2){672, 494}, 250, 250};
					levelTwo[screenNum] = TRUE;
					break;
			}
		}
		
		switch(screenNum)
		{
			case 0:
				if(scrollinMan == FALSE)
				{
					initNonPlayerChar(beardMan, (Vector2){518, 564 - beardMan.spriteSheet.height}, player);
					initNonPlayerChar(childMan, (Vector2){270, 465 - childMan.spriteSheet.height}, player);
				}
				if(!(IsMusicPlaying(levelTwoMusic))) PlayMusicStream(levelTwoMusic);
				UpdateMusicStream(levelTwoMusic);
				break;
			case 1:
				if(scrollinMan == FALSE)
				{
					initNonPlayerChar(popo1, (Vector2){125, 564 - popo1.spriteSheet.height}, player);
					initNonPlayerChar(popo2, (Vector2){255, 564 - popo1.spriteSheet.height}, player);
				}
				if(!(IsMusicPlaying(levelTwoMusic))) PlayMusicStream(levelTwoMusic);
				UpdateMusicStream(levelTwoMusic);
				break;
			case 2:
				if(scrollinMan == FALSE)
				{
					if(miniMinotaurBoss.isDead && deathFrame == 6)
					{
						if(!(IsMusicPlaying(levelTwoMusic))) PlayMusicStream(levelTwoMusic);
						UpdateMusicStream(levelTwoMusic);
						initNonPlayerChar(trueWizard, (Vector2){672, 484}, player);
						if(IsKeyPressed(KEY_K) && (isText == TRUE)) 
						{
							screenNum = 0;
							levelTwoDeInit();
							isText = FALSE;
							textCounter = 0;
							player->position.x = 50;
							player->velocity = (Vector2){0, 0};
							SetPhysicsBodyRotation(player, 0);
							backPos.x = 0;
							deathFrame = 0;
							playerHealth = maxPlayerHealth;
							gameCheck++;
						}
					}
					else
					{
						if(!(IsMusicPlaying(bossMusic))) PlayMusicStream(bossMusic);
						UpdateMusicStream(bossMusic);
						displayHealthBar((float)(playerHealth / maxPlayerHealth), (Vector2)GetPhysicsShapeVertex(player, 3));
						if(playerHealth <= 0) 
                        {
                            charBody->position.x = 50;
                            charBody->velocity = (Vector2){0, 0};
                            SetPhysicsBodyRotation(charBody, 0);
                            fadeToBlack();
                        }
						miniMinotaurBoss = initEnemyBoss(miniMinotaurBoss, player);
					}
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
				if(!(IsMusicPlaying(titleMusic))) PlayMusicStream(titleMusic);
				ClearBackground(RAYWHITE);
				DrawTexture(backGround, backPos.x, backPos.y, WHITE);
				UpdateMusicStream(titleMusic);
				if(!titleDisplay(titleScreen))
				{
					buttonFunction(startButtonSheet, (Vector2){SCREENWIDTH / 2 - startButtonSheet.width / 2, SCREENHEIGHT / 2 - startButtonSheet.height / 3 / 2}, 1);
				}
				break;
			case 1:
				fadeToBlack();
				if(!(IsMusicPlaying(levelZeroMusic))) PlayMusicStream(levelZeroMusic);
				UpdateMusicStream(levelZeroMusic);
				break;
			case 2:
				backParallax(runSheet, levelZeroBack, charBody);
				if(scrollinMan == FALSE) drawChar(runSheet, idleSheet, jumpSheet, attackSheet, charBody);
				levelZeroInit(charBody, cyclopsWalkSheet, cyclopsDeathSheet);
				break;
			case 3:
				if(!(IsMusicPlaying(levelOneMusic))) PlayMusicStream(levelOneMusic);
				UpdateMusicStream(levelOneMusic);
				fadeToBlack();
				break;
			case 4:
				backParallax(runSheet, levelOneBack, charBody);
				if(scrollinMan == FALSE) drawChar(runSheet, idleSheet, jumpSheet, attackSheet, charBody);
				levelOneInit(charBody);
				break;
			case 5:
				if(!(IsMusicPlaying(levelTwoMusic))) PlayMusicStream(levelTwoMusic);
				UpdateMusicStream(levelTwoMusic);
				fadeToBlack();
				break;
			case 6:
				backParallax(runSheet, levelTwoBack, charBody);
				if(scrollinMan == FALSE) drawChar(runSheet, idleSheet, jumpSheet, attackSheet, charBody);
				levelTwoInit(charBody);
				break;
			case 7:
				if(!(IsMusicPlaying(titleMusic))) PlayMusicStream(titleMusic);
				UpdateMusicStream(titleMusic);
				fadeToBlack();
				break;
			case 8:
				CloseAudioDevice();
				EndDrawing();
				CloseWindow();
				return 0;
				break;
		}
		DrawFPS(5, 5);
		EndDrawing();
	}
	CloseAudioDevice();
	CloseWindow();
    return 0;
}
