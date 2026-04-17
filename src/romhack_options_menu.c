#include "romhack_options_menu.h"

#include "input.h"
#include "memory.h"
#include "menu_input.h"
#include "music_util.h"
#include "text_1.h"
#include "text_2.h"
#include "text_3.h"

static EWRAM_INIT RomhackDataOptionsMenu *sMenu = {NULL};

#include "data/romhack_options_menu.h"

static void CreateOptionsMenu(void);

bool8 CreateRomhackOptionsDisplayScreen(RomhackData *romhackData) {
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
        default:
            return 0;
    }
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
