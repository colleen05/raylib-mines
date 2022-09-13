#ifndef RAYLIBMINES_MINESGUI_H
#define RAYLIBMINES_MINESGUI_H

#include <raylib.h>
#include <raymath.h>
#include <stdbool.h>

Texture g_TexUI_ArrowUp     = {0};
Texture g_TexUI_ArrowDown   = {0};

void MGUI_LoadTextures() {
    g_TexUI_ArrowUp = LoadTexture("./resources/textures/ui_arrow_up.png");
    g_TexUI_ArrowDown = LoadTexture("./resources/textures/ui_arrow_down.png");
}

void MGUI_UnloadTextures() {
    UnloadTexture(g_TexUI_ArrowUp);
    UnloadTexture(g_TexUI_ArrowDown);
}

int MGUI_Button(Rectangle bounds, const char *text) {
    int clicked = 0;
    Color   bg = {0xff, 0xff, 0xff, 0xff},
            c3 = {0xdf, 0xdf, 0xdf, 0xff},
            c2 = {0xba, 0xba, 0xba, 0xff},
            c1 = {0x9a, 0x9a, 0x9a, 0xff},
            c0 = {0x45, 0x45, 0x45, 0xff};


    if(CheckCollisionPointRec(GetMousePosition(), bounds)) {
        bg = (Color) {0xdf, 0xdf, 0xdf, 0xff};
        c3 = (Color) {0xba, 0xba, 0xba, 0xff};
        c2 = (Color) {0x9a, 0x9a, 0x9a, 0xff};
        c1 = (Color) {0x75, 0x75, 0x75, 0xff};
        c0 = (Color) {0x45, 0x45, 0x45, 0xff};

        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) clicked = 1;
    }

    // Draw base
    DrawRectangleRec((Rectangle) { bounds.x, bounds.y, bounds.width, bounds.height },           c0);
    DrawRectangleRec((Rectangle) { bounds.x+1, bounds.y+1, bounds.width-2, bounds.height-2 },   c1);
    DrawRectangleRec((Rectangle) { bounds.x+2, bounds.y+2, bounds.width-4, bounds.height-4 },   c2);
    DrawRectangleRec((Rectangle) { bounds.x+3, bounds.y+3, bounds.width-6, bounds.height-6 },   c3);
    DrawRectangleRec((Rectangle) { bounds.x+4, bounds.y+4, bounds.width-8, bounds.height-8 },   bg);

    // Draw text
    Vector2 textbounds = MeasureTextEx(GetFontDefault(), text, 20, 2.0f);

    DrawTextEx(
        GetFontDefault(), text,
        (Vector2) {
            bounds.x + (bounds.width - textbounds.x) / 2.0f,
            bounds.y + (bounds.height - textbounds.y) / 2.0f,
        },
        20, 2.0f,
        BLACK
    );

    return clicked;
}

void MGUI_NumberEntry(int x, int y, int *value, int min, int max) {
    if(value == 0) return;

    // Define bounds
    Rectangle bounds = {
        (float)x, (float)y,
        64.0f, 32.0f
    };

    Rectangle topbtn_bounds = {
        x + 41, (float)y+1,
        22.0f, 15.0f
    };

    Rectangle bottombtn_bounds = {
        x + 41, (float)y+16,
        22.0f, 15.0f
    };

    // Button colours
    const Color btncol_c0_normal = (Color) {0x9a, 0x9a, 0x9a, 0xff};
    const Color btncol_c1_normal = (Color) {0xdf, 0xdf, 0xdf, 0xff};
    const Color btncol_bg_normal = (Color) {0xff, 0xff, 0xff, 0xff};
    const Color btncol_ar_normal = (Color) {0x45, 0x45, 0x45, 0xff};

    const Color btncol_c0_shaded = (Color) {0x9a, 0x9a, 0x9a, 0xff};
    const Color btncol_c1_shaded = (Color) {0xdf, 0xdf, 0xdf, 0xff};
    const Color btncol_bg_shaded = (Color) {0xdf, 0xdf, 0xdf, 0xff};
    const Color btncol_ar_shaded = (Color) {0x9a, 0x9a, 0x9a, 0xff};

    const Color btncol_c0_hover = (Color) {0x9a, 0x9a, 0x9a, 0xff};
    const Color btncol_c1_hover = (Color) {0xba, 0xba, 0xba, 0xff};
    const Color btncol_bg_hover = (Color) {0xdf, 0xdf, 0xdf, 0xff};

    Color btncol_c0 = btncol_c0_normal;
    Color btncol_c1 = btncol_c1_normal;
    Color btncol_bg = btncol_bg_normal;
    Color btncol_ar = btncol_ar_normal;

    // Draw base
    DrawRectangleRec(
        (Rectangle) { bounds.x, bounds.y, bounds.width, bounds.height },
        (Color) {0x45, 0x45, 0x45, 0xff}
    );

    DrawRectangleRec(
        (Rectangle) { bounds.x+1, bounds.y+1, bounds.width-2, bounds.height-2 },
        (Color) {0x9a, 0x9a, 0x9a, 0xff}
    );

    DrawRectangleRec(
        (Rectangle) { bounds.x+2, bounds.y+2, bounds.width-4, bounds.height-4 },
        (Color) {0xdf, 0xdf, 0xdf, 0xff}
    );

    // Draw separator
    DrawLine(x+40, y+1, x+40, y+30, (Color) {0x9a, 0x9a, 0x9a, 0xff});
    DrawLine(x+41, y, x+41, y+32, (Color) {0x45, 0x45, 0x45, 0xff});
    DrawLine(x+42, y+1, x+42, y+30, (Color) {0x9a, 0x9a, 0x9a, 0xff});

    // Top button
    if(CheckCollisionPointRec(GetMousePosition(), topbtn_bounds)) {
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) *value += 1;

        btncol_bg = btncol_bg_hover;
        btncol_c0 = btncol_c0_hover;
        btncol_c1 = btncol_c1_hover;
    }else {
        btncol_bg = btncol_bg_normal;
        btncol_c0 = btncol_c0_normal;
        btncol_c1 = btncol_c1_normal;
    }

    btncol_ar = btncol_ar_normal;
    if(*value >= max) {
        btncol_bg = btncol_bg_shaded;
        btncol_c0 = btncol_c0_shaded;
        btncol_c1 = btncol_c1_shaded;
        btncol_ar = btncol_ar_shaded;
    }

    DrawRectangleRec(topbtn_bounds, btncol_c0);
    DrawRectangleRec((Rectangle) { topbtn_bounds.x + 1, topbtn_bounds.y + 1, topbtn_bounds.width - 2, topbtn_bounds.height - 2}, btncol_c1);
    DrawRectangleRec((Rectangle) { topbtn_bounds.x + 2, topbtn_bounds.y + 2, topbtn_bounds.width - 4, topbtn_bounds.height - 4}, btncol_bg);

    DrawTexture(g_TexUI_ArrowUp, topbtn_bounds.x, topbtn_bounds.y, btncol_ar);

    // Bottom button
    if(CheckCollisionPointRec(GetMousePosition(), bottombtn_bounds)) {
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) { *value -= 1; }

        btncol_bg = btncol_bg_hover;
        btncol_c0 = btncol_c0_hover;
        btncol_c1 = btncol_c1_hover;
    }else {
        btncol_bg = btncol_bg_normal;
        btncol_c0 = btncol_c0_normal;
        btncol_c1 = btncol_c1_normal;
    }

    btncol_ar = btncol_ar_normal;
    if(*value <= min) {
        btncol_bg = btncol_bg_shaded;
        btncol_c0 = btncol_c0_shaded;
        btncol_c1 = btncol_c1_shaded;
        btncol_ar = btncol_ar_shaded;
    }

    DrawRectangleRec(bottombtn_bounds, btncol_c0);
    DrawRectangleRec((Rectangle) { bottombtn_bounds.x + 1, bottombtn_bounds.y + 1, bottombtn_bounds.width - 2, bottombtn_bounds.height - 2}, btncol_c1);
    DrawRectangleRec((Rectangle) { bottombtn_bounds.x + 2, bottombtn_bounds.y + 2, bottombtn_bounds.width - 4, bottombtn_bounds.height - 4}, btncol_bg);

    DrawTexture(g_TexUI_ArrowDown, bottombtn_bounds.x, bottombtn_bounds.y, btncol_ar);

    // Scrolling
    if(CheckCollisionPointRec(GetMousePosition(), (Rectangle) {(float)x, (float)y, 64.0f, 32.0f})) {
        *value += GetMouseWheelMove();
    }

    // Clamping number
    *value = Clamp(*value, min, max);

    // Draw number
    char *txt = calloc(1, 4);
    sprintf(txt, "%i", *value);

    DrawText(txt, x+10, y+7, 20, BLACK);
    free(txt);
}

#endif