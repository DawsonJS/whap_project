#ifndef LOGIC_H_INCLUDED
#define LOGIC_H_INCLUDED

#include <stdio.h>
#include <math.h>
#include <string.h>
#include "raylib.h"
#define PHYSAC_IMPLEMENTATION
#define PHYSAC_NO_THREADS
#include "physac.h"
#define VERSION "Time Tales 1.0"
#define TRUE 1
#define FALSE 0
#define VELOCITY 0.15f
#define ENEMYVELOCITY 2
#define BACKVELOCITY 10
#define SCREENWIDTH 800
#define SCREENHEIGHT 600
#define SCALAR 1.5

char fadestoBlack[5][1300] = {("Welcome to Time Tales v1.0!\nThis game begins at the height of the Black Death, plague, whatever you \nwould like to call it, circa mid to late 14th century. You, the noble knight \nturned adventurer, seek to end this miserable time for all involved. After \nmeeting a mysterious man who goes by 'The Wizard,' you agree to meet him \nby the well at the edge of town. He tells you nothing can be done of the \npresent, but that hope can be had for the future. He promises to perform his mysterious magic and take you to the lands of a distant time to give you closure.\nControls:\nA - Move Left\nD - Move Right\nK - Slice & Dice / Exit Level\nSpace - Jump\nPress Enter to Continue"), ("On your journey, The Wizard explains humanity's advancements through time. The Industrial Revolution, marked by the latter half of the 18th century, \nushered in a wave of new innovations replacing manual labor with so called \ndevices named machines. These machines increased several industries \nten-fold. For example, Richard Arkwright's advent of the water frame \ngreatly improved the cotton spinning process and was followed by a \nplethora of cotton mills when the patent expired. Continually, steam power \nand the steam engine, invented by James Watt, allowed for these machines \nto work without direct access to water. Thus, innovation and \nindustrialization led to the expansion and economic boom of Great Britain.\nKey Concept 5.1\n\nAfter you arrive, The Wizard says he has business to attend to and to \nmeet him by another well at the edge of town. You decide to explore this \nmysterious time period for yourself.\nPress Enter to Continue"), ("When asked about World War II, The Wizard begins with the horrors of \nhuman advancement of weaponry and warfare. The use of chemicals and \ntrench warfare along with the invention of flying and beastial travel \nmachines made World War I known as the Great War. However, it was not \nlong after instability broke out once again. Bitter Germany, under the \nformer-Cyclops Adolf Hitler, invaded Poland while still under supervision \nfrom other kingdoms. This led to war being declared upon Germany by both \nFrance and Britain in 1939. Within the gap between the two Great Wars, new machines and mysterious devices had been either developed or continued to be developed during this time. The Wizard continued, saying a major beginning for humanity occurred in a place named Bletchley Park. It was at this place where the first machine called a 'computer' is regarded to have been built. Alan Turing is accredited to developing a major part of the Bombe, a \ncryptographic deciphering device used to decrypt the codes of German \ntransmissions. This technology marked the beginning of true computational \nmechanics.\nKey Concept 6.1\n\nThe Wizard then tells you to meet him at the conveniently placed well at the edge of the park.\nPress Enter to Continue"), ("Time Tales v1.0\n\nProgramming by Dawson Schraiber\n\nArtwork provided by various sources, I do not own any of it.\n\nMusic:\nKevin Macleod\nAndreas Ericson\n\nThank you for playing my game! It was a blast making it!\n\nPress Enter to Exit Time Tales"), ("GAME OVER\nPress Enter to Try Again")};

//Global Variables
int groundLev = 495;
Vector2 charPos = {10, 495};
Vector2 backPos = {0, 0};
int currentFrame = 0;
int npcFrame = 0;
int attackFrame = 0;
int enemyFrame = 0;
int deathFrame = 0;
int textCounter = 0;
int isLeft = FALSE;
int enemyIsLeft = FALSE;
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
int attackDamage = 1;
float playerHealth = 15;
float maxPlayerHealth = 15;
int isContinue = FALSE;
int pageNum = 1;
int textStart = 0;
int isMusicPlaying = FALSE;

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
	char textBox[600];
} nonPlayerChar;

nonPlayerChar beardMan;
nonPlayerChar colonialMan;
nonPlayerChar oldMan;
nonPlayerChar oldWoman;
nonPlayerChar trueWizard;
nonPlayerChar childMan;
nonPlayerChar popo1;
nonPlayerChar popo2;

typedef struct
{
	Texture2D walkSheet;
	Texture2D deathSheet;
	int walkFrames;
	int deathFrames;
	int isDead;
	Vector2 enemyPosition;
	float actualHealth;
    float maxHealth;
} enemyBoss;
enemyBoss cyclopsBoss;
enemyBoss flaminSkullBoss;
enemyBoss miniMinotaurBoss;

Sound cursorSound;

void fadeToBlack(); // Cut to black to give some background information
#endif
