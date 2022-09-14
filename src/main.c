#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <gameglobals.h>
#include <minesgui.h>

// Main function
int main(int argc, char **argv) {
    // Initialise game window and audio device
    InitWindow(800, 600, "Mines!");
    SetTargetFPS(60);

    InitAudioDevice();

    // Get & set the window icon
    Image img_winIcon = LoadImage("icon32.png");
    SetWindowIcon(img_winIcon);

    // Load resources
    MGUI_LoadTextures();

    g_TexMenuBG     = LoadTexture("./resources/textures/splash.png");
    g_TexFieldCells = LoadTexture("./resources/textures/fieldcells.png");
    g_TexTopCells   = LoadTexture("./resources/textures/topcells.png");
    g_TexStatusBar  = LoadTexture("./resources/textures/statusbar.png");

    g_SndExplode    = LoadSound("./resources/sounds/explode.ogg");
    g_SndWin        = LoadSound("./resources/sounds/win.ogg");

    // Game global setup
    gen_field(25, 15, 50);

    // Game loop
    while(!WindowShouldClose() && !g_bShouldQuit) {
        // Drawing
        BeginDrawing();
        ClearBackground(WHITE);
        
        // We stored the behaviour of each game state in functions, so we check
        // to see which function we should call for the current game state.
        switch(g_GameState) {
            case GAMESTATE_MENU:
                do_titlemenu();
                break;
            case GAMESTATE_MINEFIELD:
                do_minefield();
                break;
        }

        EndDrawing();
    }

    // Unload resources
    MGUI_UnloadTextures();

    UnloadImage(img_winIcon);

    UnloadTexture(g_TexMenuBG);
    UnloadTexture(g_TexFieldCells);
    UnloadTexture(g_TexTopCells);
    UnloadTexture(g_TexStatusBar);

    UnloadSound(g_SndExplode);
    UnloadSound(g_SndWin);

    // Close audio device and window
    CloseAudioDevice();
    CloseWindow();

    return 0;
}

// Count cell neighbours - Returns # of surrounding mines; 9 if on mine.
int get_cell_number(int x, int y) {
    if((x < 0) || (y < 0)) return -1;
    if((x >= g_iFieldWidth) || (y >= g_iFieldHeight)) return -1;

    // Count mines
    int mines   = 0;
    int initpos = (y * g_iFieldWidth) + x;
    
    if(g_pField[initpos] == FIELDCELL_MINE) return 9; // Return 9 if on mine.

    // Check each cell surroudning from the starting cell
    for(int xoff = -1; xoff < 2; xoff++) {
        for(int yoff = -1; yoff < 2; yoff++) {
            int pos = ((y+yoff) * g_iFieldWidth) + (x + xoff); // Calculate the index into the array

            if( // Skip out of bounds cells
                ((x+xoff) < 0) || ((y+yoff) < 0) ||
                ((x+xoff) >= g_iFieldWidth) || ((y+yoff) >= g_iFieldHeight)
            ) {
                continue;
            }else {
                if(g_pField[pos] == FIELDCELL_MINE) mines++;
            }
        }
    }

    return mines;
}

// Reveal empty cells
void reveal_empty_cells(int x, int y) {
    int idx = (y * g_iFieldWidth) + x;

    if((x < 0) || (y < 0)) return;
    if((x >= g_iFieldWidth) || (y >= g_iFieldWidth)) return;

    int cell_number = get_cell_number(x, y);

    if((cell_number == 0) && (g_pTop[idx] != TOPCELL_NONE)) {
        g_pTop[idx] = TOPCELL_NONE;

        reveal_empty_cells(x, y - 1);
        reveal_empty_cells(x, y + 1);
        reveal_empty_cells(x - 1, y);
        reveal_empty_cells(x + 1, y);
    }else if(get_cell_number(x, y) != -1){
        g_pTop[idx] = TOPCELL_NONE;
    }

    return;
}

// Minefield generation
void gen_field(int width, int height, int mines) {
    SetWindowSize(fmax(width * 32, 800), fmax(height * 32 + 120, 600));

    // Set globals
    g_iFieldWidth   = width;
    g_iFieldHeight  = height;
    g_iFieldMines   = mines;

    g_bGameOver = false;
    g_iClickedCells = 0;

    // Free each array if already allocated
    if(g_pField != 0)   free(g_pField);
    if(g_pTop != 0)     free(g_pTop);

    // Allocate arrays
    g_pField    = calloc((g_iFieldWidth * g_iFieldHeight), sizeof(FieldCell_t));
    g_pTop      = calloc((g_iFieldWidth * g_iFieldHeight), sizeof(TopCell_t));

    // Initialise arrays
    for(int i = 0; i < g_iFieldWidth * g_iFieldHeight; i++) {
        g_pField[i] = FIELDCELL_EMPTY;
        g_pTop[i]   = TOPCELL_BLANK;
    }

    // Generate mines
    int placed_mines = 0;

    while(placed_mines != mines) { // Ensure correct number of mines
        int rx  = GetRandomValue(0, g_iFieldWidth - 1);
        int ry  = GetRandomValue(0, g_iFieldHeight - 1);
        int i   = (ry * g_iFieldWidth) + rx;

        if(g_pField[i] != FIELDCELL_MINE) {  // Only place mine if space is free
            g_pField[i] = FIELDCELL_MINE;
            placed_mines++;
        }
    }

    // Place numbers
    for(int x = 0; x < g_iFieldWidth; x++) {
        for(int y = 0; y < g_iFieldHeight; y++) {
            int idx = (y * g_iFieldWidth) + x; // Calculate the index into the array

            g_pField[idx] = get_cell_number(x, y);
        }
    }
}

// Title menu update & drawing
void do_titlemenu() {
    // Set window size
    SetWindowSize(800, 600);

    // Reset some globals
    g_bDoTimer = false;
    g_iRoundTimer = 0;
    g_iRoundStart = 0;
    g_bGameWon = false;
    g_bGameOver = false;
    g_StatusFace = STATUSFACE_HAPPY;

    // Draw background
    DrawTexture(g_TexMenuBG, 0, 0, WHITE);

    // Preset buttons
    DrawText("10x10 | 10 Mines", 280, 284 + 8, 20, (Color) {0x00, 0x00, 0x00, 0x7f});
    if(MGUI_Button((Rectangle) {128.0f, 284.0f, 144.0f, 32.0f}, "BEGINNER")) {
        gen_field(10, 10, 10);

        g_iRoundStart = GetTime();
        g_iClickedCells = 0;
        g_GameState = GAMESTATE_MINEFIELD;
    }

    DrawText("25x15 | 50 Mines", 280, 284 + 48 + 8, 20, (Color) {0x00, 0x00, 0x00, 0x7f});
    if(MGUI_Button((Rectangle) {128.0f, 284.0f + 48.0f, 144.0f, 32.0f}, "DIFFICULT")) {
        gen_field(25, 15, 50);

        g_iRoundStart = GetTime();
        g_iClickedCells = 0;
        g_GameState = GAMESTATE_MINEFIELD;
    }

    DrawText("30x15 | 99 Mines", 280, 284 + 96 + 8, 20, (Color) {0x00, 0x00, 0x00, 0x7f});
    if(MGUI_Button((Rectangle) {128.0f, 284.0f + 96.0f, 144.0f, 32.0f}, "EXPERT")) {
        gen_field(30, 15, 99);

        g_iRoundStart = GetTime();
        g_iClickedCells = 0;
        g_GameState = GAMESTATE_MINEFIELD;
    }

    // Custom region
    if(MGUI_Button((Rectangle) {464.0f, 380.0f, 208.0f, 32.0f}, "CUSTOM")) {
        gen_field(g_iFieldWidth, g_iFieldHeight, g_iFieldMines);

        g_iRoundStart = GetTime();
        g_iClickedCells = 0;
        g_GameState = GAMESTATE_MINEFIELD;
    }

    int maxmines = (g_iFieldWidth * g_iFieldHeight) - 1;

    DrawText("W", 512 - 20, 290, 20, (Color) {0x00, 0x00, 0x00, 0x7f});
    MGUI_NumberEntry(512, 284, &g_iFieldWidth, 5, 50);

    DrawText("H", 608 - 20, 290, 20, (Color) {0x00, 0x00, 0x00, 0x7f});
    MGUI_NumberEntry(608, 284, &g_iFieldHeight, 5, 25);

    DrawText("Mines", 608 - 60, 338, 20, (Color) {0x00, 0x00, 0x00, 0x7f});
    MGUI_NumberEntry(608, 332, &g_iFieldMines, 1, Clamp(maxmines, 1, 199));

    if(MGUI_Button((Rectangle) {328.0f, 284.0f + 256.0f, 144.0f, 32.0f}, "QUIT")) {
        g_bShouldQuit = true;
    }
}

// Minefield update & drawing
void do_minefield() {
    // Draw panels
    DrawRectangle       (0, 0, GetScreenWidth(), 120,     (Color){0xff, 0xff, 0xff, 0xff});   // Solid BG - Top
    DrawRectangleLines  (0, 0, GetScreenWidth(), 120,     (Color){0x55, 0x55, 0x55, 0xff});   // Border - Top, Outside
    DrawRectangleLines  (1, 1, GetScreenWidth() - 2, 118, (Color){0xaa, 0xaa, 0xaa, 0xff});   // Border - Top, Inside

    DrawRectangle       (0, 120, 800, 480,  (Color){0xaa, 0xaa, 0xaa, 0xff});   // Solid BG - Bottom
    DrawRectangleLines  (0, 120, 800, 480,  (Color){0x75, 0x75, 0x75, 0xff});   // Border - Bottom, Outside
    DrawRectangleLines  (1, 121, 798, 478,  (Color){0x8a, 0x8a, 0x8a, 0xff});   // Border - Bottom, Inside

    // Draw status bar
    DrawTextureRec( // Status face
        g_TexStatusBar,
        g_pRecStatusFaces[g_StatusFace],
        (Vector2) {GetScreenWidth() / 2 - 126.0f, 12.0f},
        WHITE
    );

    DrawTextureRec( // Mines counter - Icon
        g_TexStatusBar,
        g_pRecStatusNumber_Mine,
        (Vector2) {32.0f, 36.0f},
        WHITE
    );

    DrawTextureTiled( // Mines counter - Zeros
        g_TexStatusBar,
        g_pRecStatusNumber_Blank,
        (Rectangle) {64.0f, 36.0f, 96.0f, 56.0f},
        (Vector2) {0.0f, 0.0f},
        0.0f, 1.0f, WHITE
    );

    // Mines counter - Numbers
    char *txt_minecounter = calloc(1, 4);
    sprintf(txt_minecounter, "%*i", 3, (int) Clamp(g_iFieldMines - g_iPlacedFlags, 0, 9999));

    DrawTextureRec(g_TexStatusBar, g_pRecStatusNumbers[txt_minecounter[0] - '0'], (Vector2) {64.0f, 36.0f}, WHITE);
    DrawTextureRec(g_TexStatusBar, g_pRecStatusNumbers[txt_minecounter[1] - '0'], (Vector2) {96.0f, 36.0f}, WHITE);
    DrawTextureRec(g_TexStatusBar, g_pRecStatusNumbers[txt_minecounter[2] - '0'], (Vector2) {128.0f, 36.0f}, WHITE);

    free(txt_minecounter);

    // Time counter - Zeros
    DrawTextureTiled( // Mines counter - Zeros
        g_TexStatusBar,
        g_pRecStatusNumber_Blank,
        (Rectangle) {GetScreenWidth() - 160.0f, 36.0f, 128.0f, 56.0f},
        (Vector2) {0.0f, 0.0f},
        0.0f, 1.0f, WHITE
    );

    // Time counter - Numbers
    if(g_bDoTimer) g_iRoundTimer = GetTime() - g_iRoundStart;

    char *txt_timer = calloc(1, 5);
    sprintf(txt_timer, "%*i", 4, (int) Clamp(g_iRoundTimer, 0, 9999));

    DrawTextureRec(g_TexStatusBar, g_pRecStatusNumbers[txt_timer[0] - '0'], (Vector2) {GetScreenWidth() - 160.0f, 36.0f}, WHITE);
    DrawTextureRec(g_TexStatusBar, g_pRecStatusNumbers[txt_timer[1] - '0'], (Vector2) {GetScreenWidth() - 128.0f, 36.0f}, WHITE);
    DrawTextureRec(g_TexStatusBar, g_pRecStatusNumbers[txt_timer[2] - '0'], (Vector2) {GetScreenWidth() - 96.0f, 36.0f}, WHITE);
    DrawTextureRec(g_TexStatusBar, g_pRecStatusNumbers[txt_timer[3] - '0'], (Vector2) {GetScreenWidth() - 64.0f, 36.0f}, WHITE);

    free(txt_timer);

    if(MGUI_Button((Rectangle) {GetScreenWidth() / 2 - 18.0f, 12.0f, 144.0f, 32.0f}, "NEW GAME")) {
        gen_field(g_iFieldWidth, g_iFieldHeight, g_iFieldMines);

        g_iRoundStart = GetTime();
        g_iRoundTimer = 0;
        g_bDoTimer = false;
        g_StatusFace = STATUSFACE_HAPPY;
        g_bGameOver = false;
        g_bGameWon = false;
    }

    if(MGUI_Button((Rectangle) {GetScreenWidth() / 2 - 18.0f, 52.0f, 144.0f, 32.0f}, "MENU")) {
        g_GameState = GAMESTATE_MENU;
    }

    // Draw minefield
    for(int x = 0; x < g_iFieldWidth; x++) {
        for(int y = 0; y < g_iFieldHeight; y++) {
            int fieldcell_value = g_pField[(y * g_iFieldWidth) + x];
            int topcell_value   = g_pTop[(y * g_iFieldWidth) + x];

            // Draw field cell
            DrawTexturePro (
                g_TexFieldCells,
                (Rectangle) {
                    fieldcell_value * 32.0f, 0.0f,  // Calculate the cell tile's position in the texture
                    32.0f, 32.0f
                },
                (Rectangle) {
                    x * 32.0f, 120.0f + y * 32.0f,
                    32.0f, 32.0f
                },
                (Vector2) {0.0f, 0.0f},
                0.0f,
                WHITE
            );

            // Draw top cell
            DrawTexturePro (
                g_TexTopCells,
                (Rectangle) {
                    topcell_value * 32.0f, 0.0f,  // Calculate the cell tile's position in the texture
                    32.0f, 32.0f
                },
                (Rectangle) {
                    x * 32.0f, 120.0f + y * 32.0f,
                    32.0f, 32.0f
                },
                (Vector2) {0.0f, 0.0f},
                0.0f,
                WHITE
            );
        }
    }

    int revealed_cells = 0;

    // Count stats
    if(!g_bGameOver) {
        g_iPlacedFlags = 0;

        for(int i = 0; i < g_iFieldWidth * g_iFieldHeight; i++) {
            if(g_pTop[i] == TOPCELL_NONE) revealed_cells++;
            if(g_pTop[i] == TOPCELL_FLAG) g_iPlacedFlags++;
        }
    }

    // Check if player won
    if((revealed_cells == (g_iFieldWidth * g_iFieldHeight) - g_iFieldMines) && (!g_bGameWon)) {
        g_bGameWon = true;
        g_bDoTimer = false;
        g_StatusFace = STATUSFACE_COOL;
        PlaySound(g_SndWin);
    }

    // Main logic
    int selection_x     = floor((float)GetMouseX() / 32.0f);
    int selection_y     = floor(((float)GetMouseY() - 120.0f) / 32.0f);
    int selection_pos   = (selection_y * g_iFieldWidth) + selection_x;

    if(
        (GetMouseY() > 120) &&
        (GetMouseY() < g_iFieldHeight * 32 + 120) &&
        (GetMouseX() < g_iFieldWidth * 32.0f) &&
        (!(g_bGameWon || g_bGameOver))
    ) {
        // Drawing selection
        DrawRectangleLines(
            selection_x * 32.0f,
            selection_y * 32.0f + 120.0f,
            32.0f, 32.0f,
            (Color) {0x00, 0x71, 0x00, 0xff}
        );

        DrawRectangleLines(
            selection_x * 32.0f + 1.0f,
            selection_y * 32.0f + 121.0f,
            30.0f, 30.0f,
            (Color) {0x00, 0xaa, 0x00, 0xff}
        );

        // Click actions
        if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            if(g_bDoTimer) g_StatusFace = STATUSFACE_SURPRISED;
        }else {
            g_StatusFace = STATUSFACE_HAPPY;
        }

        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {   // When a player clicks to reveal a mine
            if(g_pTop[selection_pos] == TOPCELL_FLAG) { // If there is a flag, remove it rather than revealing the cell below
                g_pTop[selection_pos] = TOPCELL_BLANK;
            }else if(g_pTop[selection_pos] == TOPCELL_BLANK){ // Othwerwise...
                // Generate new fields until the place they clicked contains no mines.
                while((g_pField[selection_pos] == FIELDCELL_MINE) && g_iClickedCells == 0) {
                    gen_field(g_iFieldWidth, g_iFieldHeight, g_iFieldMines);
                }

                if(!g_bDoTimer) g_iRoundStart = GetTime();
                g_bDoTimer = true;
                g_iClickedCells++;
                
                // Handle each condition for the cell below
                if(g_pField[selection_pos] == FIELDCELL_MINE) { // When they click on a mine...
                    g_bGameOver = true;
                    g_bDoTimer = false;
                    g_StatusFace = STATUSFACE_DEAD;

                    // Iterate through each mine on the field
                    for(int i = 0; i < g_iFieldWidth * g_iFieldHeight; i++) {
                        // Reveal each mine
                        if(g_pField[i] == FIELDCELL_MINE){
                            g_pTop[i] = TOPCELL_NONE;
                        }

                        // Handle flag placements
                        if(g_pTop[i] == TOPCELL_FLAG) {
                            g_pTop[i] = (g_pField[i] == FIELDCELL_MINE) ?   // Did they flag a mine correctly?
                                            TOPCELL_NONE :      // If so, reveal the mine...
                                            TOPCELL_WRONGFLAG;  // Otherwise, show the mistake.
                        }
                    }

                    g_pTop[selection_pos] = TOPCELL_CURMINE; // Highlight the cell they clicked

                    PlaySound(g_SndExplode); // Create immersive gameplay
                }else if(g_pField[selection_pos] == FIELDCELL_EMPTY) { // When they click on a cell without numbers...
                    reveal_empty_cells(selection_x, selection_y);
                }else { // When they click on a cell with numbers below
                    g_pTop[selection_pos] = TOPCELL_NONE; // Reveal the cell
                }
            }
        }else if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
            g_bDoTimer = true;

            if(g_pTop[selection_pos] == TOPCELL_BLANK) {
                g_pTop[selection_pos] = TOPCELL_FLAG;
            }else if(g_pTop[selection_pos] == TOPCELL_FLAG){
                g_pTop[selection_pos] = TOPCELL_BLANK;
            }
        }
    }
}