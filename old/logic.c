#include "logic.h"

void fadeToBlack() // Cut to black to give some background information
{
    if (!isBlack)
    {
        ClearBackground(BLACK);
        isBlack = TRUE;
    }
    switch (gameCheck)
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
        case 7:
            fadesToBlackIndex = 3;
            break;
        case 2:
            fadesToBlackIndex = 4;
            break;
        case 4:
            fadesToBlackIndex = 4;
            break;
        case 6:
            fadesToBlackIndex = 4;
            break;
    }
    Rectangle container = {SCREENWIDTH / 8, SCREENHEIGHT / 6, 600, 500};
    if (!((textCounter / 10) >= strlen(fadestoBlack[fadesToBlackIndex])))
    {
        textCounter += 6;
        if(!(IsSoundPlaying(cursorSound))) PlaySound(cursorSound);
    }
    DrawTextRec(GetFontDefault(), SubText(fadestoBlack[fadesToBlackIndex], 0, 
        textCounter / 10), (Rectangle)container, 15, 2, true, RAYWHITE);
    if (IsKeyPressed(KEY_ENTER) && fadesToBlackIndex != 4) 
    {
        isBlack = FALSE;
        textCounter = 0;
        gameCheck++;
        isContinue = FALSE;
        pageNum = 1;
        textStart = 0;
    }
    else if (IsKeyPressed(KEY_ENTER) && fadesToBlackIndex == 4)
    {
        screenNum = 0;
        isText = FALSE;
        textCounter = 0;
        backPos.x = 0;
        playerHealth = maxPlayerHealth;
        deathFrame = 0;
    }
}
