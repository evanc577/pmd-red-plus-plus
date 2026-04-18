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

static void CreateOptionsMenu(void);

bool8 CreateRomhackOptionsDisplayScreen(RomhackData *romhackData) {
    sMenu = MemoryAlloc(sizeof(RomhackDataOptionsMenu), MEMALLOC_GROUP_8);
    sMenu->data = romhackData;
    ResetTouchScreenMenuInput(&sMenu->touch);
    sMenu->windowId = 0;
    sMenu->currWindowTemplate = &sMenu->windowTemplates.id[0];
    RestoreSavedWindows(&sMenu->windowTemplates);
    sMenu->windowTemplates.id[sMenu->windowId] = sWindowTemplate;
    sub_8012D08(sMenu->currWindowTemplate, ARRAY_COUNT(sRomhackOptions));
    ResetUnusedInputStruct();
    ShowWindows(&sMenu->windowTemplates, TRUE, TRUE);
    CreateMenuOnWindow(&sMenu->input, ARRAY_COUNT(sRomhackOptions), 10, sMenu->windowId);
    CreateOptionsMenu();
    return TRUE;
}

void DestroyRomhackOptionsDisplayScreen() {
    if (sMenu != NULL) {
        MemoryFree(sMenu);
        sMenu = NULL;
    }
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
    MenuCursorUpdate(&sMenu->input, TRUE);
    return 0;
}

static void CreateOptionsMenu(void) {
    /* u32 length; */
    u32 i;
    u32 y[ARRAY_COUNT(sRomhackOptions)];

    for (i = 0; i < ARRAY_COUNT(y); ++i) {
        y[i] = GetMenuEntryYCoord(&sMenu->input, i);
    }
    sub_80073B8(sMenu->windowId);
    PrintStringOnWindow(16, 0, sStringRomhackOptions, sMenu->windowId, 0);
    for (i = 0; i < ARRAY_COUNT(y); ++i) {
        PrintStringOnWindow(8, y[i], sRomhackOptions[i].name, sMenu->windowId, 0);
    }
    sub_80073E0(sMenu->windowId);
}
