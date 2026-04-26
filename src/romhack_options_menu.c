#include "romhack_options_menu.h"

#include "code_800D090.h"
#include "global.h"
#include "input.h"
#include "items.h"
#include "memory.h"
#include "menu_input.h"
#include "music_util.h"
#include "romhack_strings.h"
#include "structs/save.h"
#include "text_1.h"
#include "text_2.h"
#include "text_3.h"
#include <stddef.h>

static EWRAM_INIT RomhackDataOptionsMenu *sMenu = {NULL};

ALIGNED(4) static const u8 sStringDebug1[] = "DEBUG 1";

const RomhackOption sRomhackOptions[] = {
    {
        .name = _("{color YELLOW}Use {L_BUTTON} and {R_BUTTON} to change values{RESET}"),
        .type = ROMHACK_OPTION_TEXT_ONLY,
        .dataOffset = 0,
    },
    {
        .name = "Friend area cost",
        .type = ROMHACK_OPTION_MULTIPLIER,
        .dataOffset = offsetof(RomhackData, friendAreaCostMult),
    },
    {
        .name = "Item cost",
        .type = ROMHACK_OPTION_MULTIPLIER,
        .dataOffset = offsetof(RomhackData, itemCostMult),
    },
    {
        .name = "Gummi IQ effect",
        .type = ROMHACK_OPTION_MULTIPLIER,
        .dataOffset = offsetof(RomhackData, gummiIqMult),
    },
    {
        .name = "No excl. Pokemon",
        .type = ROMHACK_OPTION_ON_OFF,
        .dataOffset = offsetof(RomhackData, noExclusivePokemon),
    },
};

static const WindowTemplate sDefaultWindowTemplate = {
    .flags = 0,
    .type = WINDOW_TYPE_NORMAL,
    .pos = { 0, 0 },
    .width = 0, .height = 0,
    .totalHeight = 0,
    .unk12 = 0,
    .header = NULL,
};

static const WindowHeader sWindowHeader = {
    .count = 1,
    .currId = 0,
    .width = 12,
    .f3 = 0,
};

static const WindowTemplate sWindowTemplate = {
    .flags = 0,
    .type = WINDOW_TYPE_WITH_HEADER,
    .pos = { 2, 2 },
    .width = 24, .height = 6,
    .totalHeight = 18,
    .unk12 = 0,
    .header = &sWindowHeader
};

static RomhackMultiplier *GetOptionMultiplier(RomhackData *data, const RomhackOption *const option);
static bool8 *GetOptionOnOff(RomhackData *data, const RomhackOption *const option);
static void UpdateHeightHeader(void);
static void CreateOptionsMenu(void);

typedef enum MultiplierUpdate {
    MULTIPLIER_INCREASE,
    MULTIPLIER_DECREASE,
} MultiplierUpdate;
static bool8 UpdateMultiplier(RomhackMultiplier *value, MultiplierUpdate update);
static bool8 ToggleOnOff(bool8 *value);

bool8 CreateRomhackOptionsDisplayScreen(RomhackData *romhackData) {
    sMenu = MemoryAlloc(sizeof(RomhackDataOptionsMenu), MEMALLOC_GROUP_8);
    sMenu->data = romhackData;
    sMenu->menuHeaderWindow.m.menuWinId = 0;
    sMenu->menuHeaderWindow.m.menuWindow = &sMenu->menuHeaderWindow.m.windows.id[0];
    RestoreSavedWindows(&sMenu->menuHeaderWindow.m.windows);
    sMenu->menuHeaderWindow.m.windows.id[sMenu->menuHeaderWindow.m.menuWinId] = sWindowTemplate;
    sMenu->menuHeaderWindow.m.menuWindow->header = &sMenu->menuHeaderWindow.header;
    ResetUnusedInputStruct();
    ShowWindows(&sMenu->menuHeaderWindow.m.windows, TRUE, TRUE);
    CreateMenuOnWindow(&sMenu->menuHeaderWindow.m.input, ARRAY_COUNT(sRomhackOptions), 10, sMenu->menuHeaderWindow.m.menuWinId);
    UpdateHeightHeader();

    CreateOptionsMenu();
    return TRUE;
}

void DestroyRomhackOptionsDisplayScreen() {
    if (sMenu != NULL) {
        MemoryFree(sMenu);
        sMenu = NULL;
    }
}
static void UpdateHeightHeader(void) {
    sMenu->menuHeaderWindow.header.count = 1;
    sMenu->menuHeaderWindow.header.currId = 0;
    sMenu->menuHeaderWindow.header.width = 12;
    sMenu->menuHeaderWindow.header.f3 = 0;
    ResetUnusedInputStruct();
    ShowWindows(&sMenu->menuHeaderWindow.m.windows, TRUE, TRUE);

    UPDATE_MENU_WINDOW_HEIGHT(sMenu->menuHeaderWindow.m);
}

s32 HandleRomhackDataScreenInput(void) {
    u32 optionIdx = sMenu->menuHeaderWindow.m.input.currPage *
        sMenu->menuHeaderWindow.m.input.entriesPerPage +
        sMenu->menuHeaderWindow.m.input.menuIndex;
    bool8 updated = FALSE;
    s32 menuInput = GetMenuInput();
    switch (menuInput) {
        case INPUT_B_BUTTON:
            PlayMenuSoundEffect(MENU_SFX_BACK);
            return 2;
        case INPUT_A_BUTTON:
            PlayMenuSoundEffect(MENU_SFX_ACCEPT);
            return 3;
        case INPUT_L_BUTTON:
            if (sRomhackOptions[optionIdx].type == ROMHACK_OPTION_MULTIPLIER) {
                updated = UpdateMultiplier(GetOptionMultiplier(sMenu->data, &sRomhackOptions[optionIdx]), MULTIPLIER_DECREASE);
            } else if (sRomhackOptions[optionIdx].type == ROMHACK_OPTION_ON_OFF) {
                updated = ToggleOnOff(GetOptionOnOff(sMenu->data, &sRomhackOptions[optionIdx]));
            }
            break;
        case INPUT_R_BUTTON: {
            if (sRomhackOptions[optionIdx].type == ROMHACK_OPTION_MULTIPLIER) {
                updated = UpdateMultiplier(GetOptionMultiplier(sMenu->data, &sRomhackOptions[optionIdx]), MULTIPLIER_INCREASE);
            } else if (sRomhackOptions[optionIdx].type == ROMHACK_OPTION_ON_OFF) {
                updated = ToggleOnOff(GetOptionOnOff(sMenu->data, &sRomhackOptions[optionIdx]));
            }
            break;
        }
    }
    if (0 != MenuCursorUpdate(&sMenu->menuHeaderWindow.m.input, TRUE) || updated) {
        UpdateHeightHeader();
        CreateOptionsMenu();
        return 1;
    }
    return 0;
}

static void CreateOptionsMenu(void) {
    /* u32 length; */
    u32 i;

    CallPrepareTextbox_8008C54(sMenu->menuHeaderWindow.m.menuWinId);
    sub_80073B8(sMenu->menuHeaderWindow.m.menuWinId);

    PrintStringOnWindow(16, 0, gStringRomhackOptions, sMenu->menuHeaderWindow.m.menuWinId, 0);

    for (i = 0; i < sMenu->menuHeaderWindow.m.input.currPageEntries; ++i) {
        u32 optionIdx = sMenu->menuHeaderWindow.m.input.currPage * sMenu->menuHeaderWindow.m.input.entriesPerPage + i;
        u32 y = GetMenuEntryYCoord(&sMenu->menuHeaderWindow.m.input, i);
        switch (sRomhackOptions[optionIdx].type) {
            case ROMHACK_OPTION_TEXT_ONLY: {
                PrintStringOnWindow(8, y, sRomhackOptions[optionIdx].name, sMenu->menuHeaderWindow.m.menuWinId, 0);
                break;
            }
            case ROMHACK_OPTION_MULTIPLIER: {
                u8 buffer[256];
                u32 value = GetOptionMultiplier(sMenu->data, &sRomhackOptions[optionIdx])->value;
                sprintfStatic(buffer, _("%s{MOVE_X_POSITION}%c%d%%{RESET}"), sRomhackOptions[optionIdx].name, 150, value);
                PrintStringOnWindow(8, y, buffer, sMenu->menuHeaderWindow.m.menuWinId, 0);
                break;
            }
            case ROMHACK_OPTION_ON_OFF: {
                u8 buffer[256];
                bool8 value = *GetOptionOnOff(sMenu->data, &sRomhackOptions[optionIdx]);
                sprintfStatic(buffer, _("%s{MOVE_X_POSITION}%c%s{RESET}"), sRomhackOptions[optionIdx].name, 150, (value == TRUE ? "ON" : "OFF"));
                PrintStringOnWindow(8, y, buffer, sMenu->menuHeaderWindow.m.menuWinId, 0);
                break;
            }
        }
    }
    sub_80073E0(sMenu->menuHeaderWindow.m.menuWinId);
}

static bool8 UpdateMultiplier(RomhackMultiplier *value, MultiplierUpdate update) {
    switch (update) {
        case MULTIPLIER_INCREASE:
            if (value->value >= 10000) {
                PlayMenuSoundEffect(MENU_SFX_FAIL);
                return FALSE;
            } 

            PlayMenuSoundEffect(MENU_SFX_TOGGLE);
            if (value->value >= 2000) {
                value->value += 1000;
            } else if (value->value >= 200) {
                value->value += 100;
            } else {
                value->value += 10;
            }
            return TRUE;
        case MULTIPLIER_DECREASE:
            if (value->value <= 0) {
                PlayMenuSoundEffect(MENU_SFX_FAIL);
                return FALSE;
            }

            PlayMenuSoundEffect(MENU_SFX_TOGGLE);
            if (value->value <= 200){
                value->value -= 10;
            } else if (value->value <= 2000) {
                value->value -= 100;
            } else {
                value->value -= 1000;
            }
            return TRUE;
    }
}

static bool8 ToggleOnOff(bool8 *value) {
    if (*value == TRUE) {
        *value = FALSE;
    } else {
        *value = TRUE;
    }
    PlayMenuSoundEffect(MENU_SFX_TOGGLE);
    return TRUE;
}

static RomhackMultiplier *GetOptionMultiplier(RomhackData *data, const RomhackOption *const option) {
    u32 offset = option->dataOffset;
    void *ptr = ((void *)data) + offset;
    return (RomhackMultiplier *)ptr;
}

static bool8 *GetOptionOnOff(RomhackData *data, const RomhackOption *const option) {
    u32 offset = option->dataOffset;
    void *ptr = ((void *)data) + offset;
    return (bool8 *)ptr;
}
