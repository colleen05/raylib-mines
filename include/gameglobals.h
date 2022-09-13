#ifndef RAYLIBMINES_GAMEGLOBALS_H
#define RAYLIBMINES_GAMEGLOBALS_H

#include <raylib.h>
#include <stdint.h>

typedef uint8_t bool_t;

// Global gamestate
typedef enum {
    GAMESTATE_MENU,
    GAMESTATE_MINEFIELD
} GameState_t;

GameState_t g_GameState = GAMESTATE_MENU;

// Cell types
typedef enum {
    FIELDCELL_EMPTY,
    FIELDCELL_ONE,
    FIELDCELL_TWO,
    FIELDCELL_THREE,
    FIELDCELL_FOUR,
    FIELDCELL_FIVE,
    FIELDCELL_SIX,
    FIELDCELL_SEVEN,
    FIELDCELL_EIGHT,
    FIELDCELL_MINE,
    _FIELDCELL_COUNT
} FieldCell_t;

typedef enum {
    TOPCELL_NONE,
    TOPCELL_BLANK,
    TOPCELL_FLAG,
    TOPCELL_CURMINE,
    TOPCELL_WRONGFLAG
} TopCell_t;

typedef enum {
    STATUSFACE_HAPPY,
    STATUSFACE_SURPRISED,
    STATUSFACE_COOL,
    STATUSFACE_DEAD
} StatusFace_t;

// Game globals
Texture g_TexMenuBG     = {0};
Texture g_TexFieldCells = {0};
Texture g_TexTopCells   = {0};
Texture g_TexStatusBar  = {0};

Sound   g_SndExplode    = {0};
Sound   g_SndWin        = {0};

int g_iFieldWidth   = 25;
int g_iFieldHeight  = 15;
int g_iFieldMines   = 50;
int g_iClickedCells = 0;
int g_iPlacedFlags  = 0;
int g_iRoundStart   = 0;
int g_iRoundTimer   = 0;

StatusFace_t g_StatusFace = STATUSFACE_HAPPY;

bool g_bGameOver    = false;
bool g_bGameWon     = false;
bool g_bDoTimer     = false;
bool g_bShouldQuit  = false;

FieldCell_t *g_pField   = 0;
TopCell_t   *g_pTop     = 0;

// Status bar slices
Rectangle g_pRecStatusFaces[4] = {
    (Rectangle) {0.0f, 0.0f, 96.0f, 96.0f},
    (Rectangle) {96.0f, 0.0f, 96.0f, 96.0f},
    (Rectangle) {0.0f, 96.0f, 96.0f, 96.0f},
    (Rectangle) {96.0f, 96.0f, 96.0f, 96.0f}
};

Rectangle g_pRecStatusNumbers[10] = {
    (Rectangle) {224.0f, 0.0f, 32.0f, 56.0f},
    (Rectangle) {256.0f, 0.0f, 32.0f, 56.0f},
    (Rectangle) {288.0f, 0.0f, 32.0f, 56.0f},
    (Rectangle) {192.0f, 56.0f, 32.0f, 56.0f},
    (Rectangle) {224.0f, 56.0f, 32.0f, 56.0f},
    (Rectangle) {256.0f, 56.0f, 32.0f, 56.0f},
    (Rectangle) {288.0f, 56.0f, 32.0f, 56.0f},
    (Rectangle) {192.0f, 112.0f, 32.0f, 56.0f},
    (Rectangle) {224.0f, 112.0f, 32.0f, 56.0f},
    (Rectangle) {256.0f, 112.0f, 32.0f, 56.0f}
};

Rectangle g_pRecStatusNumber_Blank = (Rectangle) {192.0f, 0.0f, 32.0f, 56.0f};
Rectangle g_pRecStatusNumber_Colon = (Rectangle) {288.0f, 112.0f, 32.0f, 56.0f};
Rectangle g_pRecStatusNumber_Mine  = (Rectangle) {320.0f, 0.0f, 32.0f, 56.0f};

// Delcare functions
int  get_cell_number    (int x, int y);
void reveal_empty_cells (int x, int y);
void gen_field          (int width, int height, int mines);
void do_titlemenu       ();
void do_minefield       ();

#endif