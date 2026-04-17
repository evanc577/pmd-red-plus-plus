#include "romhack_options_menu.h"

#include "input.h"
#include "memory.h"
#include "menu_input.h"
#include "music_util.h"
#include "text_1.h"
#include "text_2.h"
#include "text_3.h"

static EWRAM_INIT RomhackDataOptionsMenu *sMenu = {NULL};

ALIGNED(4) static const u8 sRomhackOptions[] = "Romhack Options";
ALIGNED(4) static const u8 sStringDebug1[] = "DEBUG 1";

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

bool8 CreateRomhackOptionsDisplayScreen(RomhackDataV0 *romhackData) {
    sMenu = MemoryAlloc(sizeof(RomhackDataOptionsMenu), MEMALLOC_GROUP_8);
    sMenu->data = romhackData;
    ResetTouchScreenMenuInput(&sMenu->touch);
    sMenu->windowId = 0;
    sMenu->currWindowTemplate = &sMenu->windowTemplates.id[0];
    RestoreSavedWindows(&sMenu->windowTemplates);
    sMenu->windowTemplates.id[sMenu->windowId] = sWindowTemplate;
    sub_8012D08(sMenu->currWindowTemplate, 1);
    ResetUnusedInputStruct();
    ShowWindows(&sMenu->windowTemplates, TRUE, TRUE);
    CreateMenuOnWindow(&sMenu->input, 1, 1, sMenu->windowId);
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
    /*     case INPUT_A_BUTTON: */
    /*         PlayMenuSoundEffect(MENU_SFX_ACCEPT); */
    /*         return 3; */
    /*     case INPUT_DPAD_LEFT: */
    /*         if (sMenu->input.menuIndex == 0) { */
    /*             if (sMenu->optionsMenu->windowColor == WINDOW_COLOR_BLUE) */
    /*                 sMenu->optionsMenu->windowColor = WINDOW_COLOR_GREEN; */
    /*             else */
    /*                 sMenu->optionsMenu->windowColor--; */
    /**/
    /*             PlayMenuSoundEffect(MENU_SFX_NAVIGATE); */
    /*             flag = TRUE; */
    /*         } */
    /*         break; */
    /*     case INPUT_DPAD_RIGHT: */
    /*         if (sMenu->input.menuIndex == 0) { */
    /*             if (sMenu->optionsMenu->windowColor > WINDOW_COLOR_RED) */
    /*                 sMenu->optionsMenu->windowColor = WINDOW_COLOR_BLUE; */
    /*             else */
    /*                 sMenu->optionsMenu->windowColor++; */
    /**/
    /*             PlayMenuSoundEffect(MENU_SFX_NAVIGATE); */
    /*             flag = TRUE; */
    /*         } */
    /*         break; */
    }
    MenuCursorUpdate(&sMenu->input, TRUE);
    return 0;
}

static void CreateOptionsMenu(void) {
    /* u32 length; */
    u32 y;

    CallPrepareTextbox_8008C54(sMenu->windowId);
    sub_80073B8(sMenu->windowId);
    PrintStringOnWindow(16, 0, sRomhackOptions, sMenu->windowId, 0);

    y = GetMenuEntryYCoord(&sMenu->input, 0);
    PrintStringOnWindow(8, y, sStringDebug1, sMenu->windowId, 0);

    sub_80073E0(sMenu->windowId);
}
