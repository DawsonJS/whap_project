#include "title.h"

int TitleScreenInit(struct TitleScreen *title)
{
    title->background = LoadTexture("images/backSheet.png");
    title->titleText = LoadTexture("images/titleScreen.png");
    title->buttonGFX = LoadTexture("images/startButtonSheet.png");

    title->button = (struct Button *) malloc(sizeof(struct Button));
    if (title->button == NULL)
    {
        return -1;
    }
    title->button->dims = (Rectangle) 
    {
        .x = SCREENWIDTH / 2 - title->buttonGFX.width / 2,
        .y = SCREENHEIGHT / 2 - title->buttonGFX.height / 3 / 2,
        .width = title->buttonGFX.width,
        .height = title->buttonGFX.height / 3
    };
    title->button->state = 0;
    title->button->function = DoNothing;

    title->music = LoadMusicStream("sounds/celtic_impulse.ogg");
    SetMusicVolume(title->music, 0.6);
    PlayMusicStream(title->music);
    return 0;
}

void TitleScreenTerm(struct TitleScreen *titleScreen)
{
    UnloadTexture(titleScreen->background);
    UnloadTexture(titleScreen->titleText);
    UnloadTexture(titleScreen->buttonGFX);
    free(titleScreen->button);
    UnloadMusicStream(titleScreen->music);
    free(titleScreen);
}

void TitleScreenUpdate(struct TitleScreen *title)
{
    DrawTexture(title->background, 0, 0, WHITE);

    static Vector2 textPos = {.x = (SCREENWIDTH / 5), .y = 544};
    if(IsKeyPressed(KEY_ENTER) && textPos.y > 20)
    {
        textPos.y = 20;
    }
    else if(textPos.y > 20)
    {
        textPos.y -= 1;
    }
    DrawTexture(title->titleText, textPos.x, textPos.y, WHITE);

    if(!(IsMusicPlaying(title->music))) 
    {
        PlayMusicStream(title->music);
    }
    UpdateMusicStream(title->music);

    ButtonCheck(title->button);
    DrawTextureRec(title->buttonGFX, 
            (Rectangle) {.x = 0, 
            .y = title->button->state * (title->buttonGFX.height / 3),
            .width = title->buttonGFX.width, 
            .height = title->buttonGFX.height / 3}, 
            (Vector2) {title->button->dims.y, title->button->dims.x}, 
            WHITE);
}

// checks if user clicks inside bounds of button->dims and performs function *
void ButtonCheck(struct Button *button)
{
    Vector2 mousePoint = GetMousePosition();
    button->state = 0;
    if(CheckCollisionPointRec(mousePoint, button->dims))
    {
        if(IsMouseButtonDown(MOUSE_LEFT_BUTTON))
        {
            button->state = 2;
        }
        else
        {
            button->state = 1;
        }
        if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) 
        {
            button->function();
        }
    }
    else
    {
        button->state = 0;
    }
}

void DoNothing()
{
    int x;
    x = 1;
    x += 1;
}
