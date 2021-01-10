#ifndef TITLE_H_
#define TITLE_H_

#include <stdlib.h>
#include "raylib.h"

#include "constants.h"

struct Button {
    Rectangle dims;
    int state;
    void (*function) ();
};

struct TitleScreen {
    Texture2D background;
    Texture2D titleText;
    Texture2D buttonGFX;
    struct Button *button;
    Music music;
};

// pass in allocated TitleScreen struct to initialize variables inside
int TitleScreenInit(struct TitleScreen *title);
// pass in allocated TitleScreen struct to unload variables and free() struct
void TitleScreenTerm(struct TitleScreen *title);
// pass in initialized TitleScreen struct and update screen each frame
void TitleScreenUpdate(struct TitleScreen *title);

void ButtonCheck(struct Button *button);

void DoNothing();


#endif
