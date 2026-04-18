#ifndef GUARD_ROMHACK_OPTIONS_MENU_H
#define GUARD_ROMHACK_OPTIONS_MENU_H

#include "gba/gba.h"
#include "romhack_data.h"
#include "structs/menu.h"

typedef struct RomhackDataOptionsMenu {
    RomhackData *data;
    MenuHeaderWindow menuHeaderWindow;
} RomhackDataOptionsMenu;

typedef enum RomhackOptionType {
    ROMHACK_OPTION_TEXT_ONLY,
    ROMHACK_OPTION_MULTIPLIER,
    ROMHACK_OPTION_ON_OFF,
} RomhackOptionType;

typedef struct RomhackOption {
    const char *const name;
    const RomhackOptionType type;
    u32 dataOffset;
} RomhackOption;

bool8 CreateRomhackOptionsDisplayScreen(RomhackData *romhackData);
void DestroyRomhackOptionsDisplayScreen();
s32 HandleRomhackDataScreenInput(void);

#endif // GUARD_ROMHACK_OPTIONS_MENU_H
