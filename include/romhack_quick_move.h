#ifndef GUARD_ROMHACK_QUICK_MOVE_H
#define GUARD_ROMHACK_QUICK_MOVE_H

#include "gba/gba.h"
#include "structs/menu.h"

typedef enum RomhackQuickMoveSel {
    ROMHACK_QUICK_MOVE_CANCEL,
    ROMHACK_QUICK_MOVE_0,
    ROMHACK_QUICK_MOVE_1,
    ROMHACK_QUICK_MOVE_2,
    ROMHACK_QUICK_MOVE_3,
} RomhackQuickMoveSel;

typedef struct RomhackQuickMoveMenu {
    MenuHeaderWindow menuHeaderWindow;
} RomhackQuickMoveMenu;

extern const WindowTemplates gQuickMoveWindow;

void ShowRomhackQuickMoveScreen();

#endif // GUARD_ROMHACK_QUICK_MOVE_H
