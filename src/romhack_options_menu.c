#include "romhack_options_menu.h"

#include "global.h"
#include "input.h"
#include "memory.h"
#include "menu_input.h"
#include "music_util.h"
#include "text_1.h"
#include "text_2.h"
#include "text_3.h"

static EWRAM_INIT RomhackDataOptionsMenu *sMenu = {NULL};

ALIGNED(4) static const u8 sStringRomhackOptions[] = "Romhack Options";
ALIGNED(4) static const u8 sStringDebug1[] = "DEBUG 1";

const RomhackOption sRomhackOptions[] = {
    {
        .name = "Friend area cost",
        .type = ROMHACK_OPTION_MULTIPLIER,
        .data = (void *)(&gRomhackData.friendAreaCostMult),
    },
    {
        .name = "Friend area cost",
        .type = ROMHACK_OPTION_MULTIPLIER,
        .data = (void *)(&gRomhackData.friendAreaCostMult),
    },
    {
        .name = "Friend area cost",
        .type = ROMHACK_OPTION_MULTIPLIER,
        .data = (void *)(&gRomhackData.friendAreaCostMult),
    },
    {
        .name = "Friend area cost",
        .type = ROMHACK_OPTION_MULTIPLIER,
        .data = (void *)(&gRomhackData.friendAreaCostMult),
    },
    {
        .name = "Friend area cost",
        .type = ROMHACK_OPTION_MULTIPLIER,
        .data = (void *)(&gRomhackData.friendAreaCostMult),
    },
    {
        .name = "Friend area cost",
        .type = ROMHACK_OPTION_MULTIPLIER,
        .data = (void *)(&gRomhackData.friendAreaCostMult),
    },
    {
        .name = "Friend area cost",
        .type = ROMHACK_OPTION_MULTIPLIER,
        .data = (void *)(&gRomhackData.friendAreaCostMult),
    },
    {
        .name = "Friend area cost",
        .type = ROMHACK_OPTION_MULTIPLIER,
        .data = (void *)(&gRomhackData.friendAreaCostMult),
    },
    {
        .name = "Friend area cost",
        .type = ROMHACK_OPTION_MULTIPLIER,
        .data = (void *)(&gRomhackData.friendAreaCostMult),
    },
    {
        .name = "Friend area cost",
        .type = ROMHACK_OPTION_MULTIPLIER,
        .data = (void *)(&gRomhackData.friendAreaCostMult),
    },
    {
        .name = "Friend area cost",
        .type = ROMHACK_OPTION_MULTIPLIER,
        .data = (void *)(&gRomhackData.friendAreaCostMult),
    },
    {
        .name = "Friend area cost",
        .type = ROMHACK_OPTION_MULTIPLIER,
        .data = (void *)(&gRomhackData.friendAreaCostMult),
    },
    {
        .name = "Friend area cost",
        .type = ROMHACK_OPTION_MULTIPLIER,
        .data = (void *)(&gRomhackData.friendAreaCostMult),
    },
    {
        .name = "Friend area cost",
        .type = ROMHACK_OPTION_MULTIPLIER,
        .data = (void *)(&gRomhackData.friendAreaCostMult),
    },
    {
        .name = "Friend area cost",
        .type = ROMHACK_OPTION_MULTIPLIER,
        .data = (void *)(&gRomhackData.friendAreaCostMult),
    },
    {
        .name = "Friend area cost",
        .type = ROMHACK_OPTION_MULTIPLIER,
        .data = (void *)(&gRomhackData.friendAreaCostMult),
    },
    {
        .name = "Friend area cost",
        .type = ROMHACK_OPTION_MULTIPLIER,
        .data = (void *)(&gRomhackData.friendAreaCostMult),
    },
    {
        .name = "Gummi IQ eff",
        .type = ROMHACK_OPTION_MULTIPLIER,
        .data = (void *)(&gRomhackData.gummiIqMult),
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

static void UpdateHeightHeader(void);
static void CreateOptionsMenu(void);

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
    switch (GetMenuInput()) {
        case INPUT_B_BUTTON:
        PlayMenuSoundEffect(MENU_SFX_BACK);
            return 2;
        case INPUT_A_BUTTON:
            PlayMenuSoundEffect(MENU_SFX_ACCEPT);
            return 3;
    }
    if (0 != MenuCursorUpdate(&sMenu->menuHeaderWindow.m.input, TRUE)) {
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

    PrintStringOnWindow(16, 0, sStringRomhackOptions, sMenu->menuHeaderWindow.m.menuWinId, 0);

    for (i = 0; i < sMenu->menuHeaderWindow.m.input.currPageEntries; ++i) {
        u32 optionIdx = sMenu->menuHeaderWindow.m.input.currPage * sMenu->menuHeaderWindow.m.input.entriesPerPage + i;
        u32 y = GetMenuEntryYCoord(&sMenu->menuHeaderWindow.m.input, i);
        PrintStringOnWindow(8, y, sRomhackOptions[optionIdx].name, sMenu->menuHeaderWindow.m.menuWinId, 0);
    }
    sub_80073E0(sMenu->menuHeaderWindow.m.menuWinId);
}
