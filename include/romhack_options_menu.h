#ifndef GUARD_ROMHACK_OPTIONS_MENU_H
#define GUARD_ROMHACK_OPTIONS_MENU_H

#include "gba/gba.h"
#include "romhack_data.h"
#include "structs/menu.h"

typedef struct RomhackDataOptionsMenu {
    RomhackData *data;
    MenuHeaderWindow menuHeaderWindow;
} RomhackDataOptionsMenu;

bool8 CreateRomhackOptionsDisplayScreen(RomhackData *romhackData);
void DestroyRomhackOptionsDisplayScreen();
s32 HandleRomhackDataScreenInput(void);

#endif // GUARD_ROMHACK_OPTIONS_MENU_H
