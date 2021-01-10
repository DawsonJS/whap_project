#include "raylib.h"
#include "title.h"

int main()
{
    InitWindow(SCREENWIDTH, SCREENHEIGHT, "WHAP Project");
    SetTargetFPS(60);
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitAudioDevice();
    struct TitleScreen *title = (struct TitleScreen *) malloc(sizeof(struct TitleScreen));
    if (title == NULL)
    {
        return -1;
    }
    TitleScreenInit(title);
    while(!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawFPS(5, 5);
        TitleScreenUpdate(title);
        EndDrawing();
    }
    TitleScreenTerm(title);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
