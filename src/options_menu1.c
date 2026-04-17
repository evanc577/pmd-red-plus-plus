#include "global.h"
#include "globaldata.h"
#include "code_8099360.h"
#include "common_strings.h"
#include "hints_menu1.h"
#include "hints_menu2.h"
#include "input.h"
#include "memory.h"
#include "menu_input.h"
#include "options_menu1.h"
#include "options_menu2.h"
#include "string_format.h"
#include "text_1.h"

enum
{
    MENU_OPTION_DEFAULT = 1,
    MENU_OPTION_HINTS,
    MENU_OPTION_GAMEOPTIONS,
    MENU_OPTION_YES,
    MENU_OPTION_NO,
    MENU_OPTION_ROMHACK_OPTIONS,
};

enum OptionsMenuStates
{
    OPTIONS_MENU_INIT,
    OPTIONS_MENU_MAIN,
    OPTIONS_MENU_EXIT,
    OPTIONS_MENU_PRE_HINT_SELECTION,
    OPTIONS_MENU_HINT_SELECTION,
    OPTIONS_MENU_DISPLAY_HINT,
    OPTIONS_MENU_UNKNOWN_6,
    OPTIONS_MENU_CONFIRM_NEW_OPTIONS,
    OPTIONS_MENU_ROMHACK_OPTIONS,
};

static EWRAM_INIT OptionsMenu1State *sOptionsMenu1State = {NULL};

#include "data/options_menu1.h"

static void CreateChangeSettingsConfirmMenu(void);
static void CreateOthersMenu(void);
static void CreateRomhackOptionsMenu(void);
static void HandleChangeSettingsMenu(void);
static void HandleOthersMenu(void);
static void SetOptionsMenuState(u32 newState);
static void SetRomhackOptionsMenuState(u32 newState);

static void CreateAllMenus(void);
static void GameOptionsMenuTransition(void);
static void sub_801E088(void);
static void sub_801E0E0(void);
static void sub_801E0FC(void);

bool8 sub_801DCC4(void)
{
    sOptionsMenu1State = MemoryAlloc(sizeof(OptionsMenu1State), MEMALLOC_GROUP_8);
    SetOptionsMenuState(OPTIONS_MENU_INIT);
    return TRUE;
}

u32 sub_801DCE8(void)
{
    switch (sOptionsMenu1State->state) {
        case OPTIONS_MENU_EXIT:
            return 3;
        case OPTIONS_MENU_INIT:
        case OPTIONS_MENU_MAIN:
            HandleOthersMenu();
            break;
        case OPTIONS_MENU_PRE_HINT_SELECTION:
        case OPTIONS_MENU_HINT_SELECTION:
            sub_801E088();
            break;
        case OPTIONS_MENU_DISPLAY_HINT:
            sub_801E0E0();
            break;
        case 6:
            sub_801E0FC();
            break;
        case OPTIONS_MENU_CONFIRM_NEW_OPTIONS:
            HandleChangeSettingsMenu();
            break;
        default:
            break;
    }

    return 0;
}

void sub_801DD50(void)
{
    if (sOptionsMenu1State != NULL) {
        MemoryFree(sOptionsMenu1State);
        sOptionsMenu1State = NULL;
    }
}

static void SetOptionsMenuState(u32 newState)
{
    sOptionsMenu1State->state = newState;
    CreateAllMenus();
    GameOptionsMenuTransition();
}

static void SetRomhackOptionsMenuState(u32 newState)
{
    sOptionsMenu1State->state = newState;
    CreateAllMenus();
    GameOptionsMenuTransition();
}

static void CreateAllMenus(void)
{
    s32 i;

    RestoreSavedWindows(&sOptionsMenu1State->window_templates);

    switch (sOptionsMenu1State->state) {
        case OPTIONS_MENU_INIT:
            CreateOthersMenu();

            for (i = 0; i < 8; i++) {
                if (sOptionsMenu1State->unkAC[i] == 0) {
                    sOptionsMenu1State->menuAction = sOptionsMenu1State->menuItems[i].menuAction;
                    break;
                }
            }

            for(i = 0; i < 4; i++)
                sOptionsMenu1State->window_templates.id[i] = sDefaultWindowTemplate;

            sOptionsMenu1State->window_templates.id[0] = sWindowTemplate;
            sub_8012CAC(&sOptionsMenu1State->window_templates.id[0], sOptionsMenu1State->menuItems);
            break;
        case OPTIONS_MENU_MAIN:
            CreateOthersMenu();

            for (i = 0; i < 4; i++)
                sOptionsMenu1State->window_templates.id[i] = sDefaultWindowTemplate;

            sOptionsMenu1State->window_templates.id[0] = sWindowTemplate;
            sub_8012CAC(&sOptionsMenu1State->window_templates.id[0], sOptionsMenu1State->menuItems);
            break;
        default:
            for (i = 0; i < 4; i++)
                sOptionsMenu1State->window_templates.id[i] = sDefaultWindowTemplate;
            break;
    }

    ResetUnusedInputStruct();
    ShowWindows(&sOptionsMenu1State->window_templates, TRUE, TRUE);
}

static void GameOptionsMenuTransition(void)
{
    switch (sOptionsMenu1State->state) {
        case OPTIONS_MENU_INIT:
        case OPTIONS_MENU_MAIN:
            sOptionsMenu1State->menu.unk0 = sOthers;
            sub_8012D60(&sOptionsMenu1State->menu, sOptionsMenu1State->menuItems, 0, sOptionsMenu1State->unkAC, sOptionsMenu1State->menuAction, 0);
            break;
        case OPTIONS_MENU_PRE_HINT_SELECTION:
            sub_801E3F0(0);
            break;
        case OPTIONS_MENU_HINT_SELECTION:
            CreateHintSelectionScreen(1);
            break;
        case OPTIONS_MENU_DISPLAY_HINT:
            CreateHintDisplayScreen(sOptionsMenu1State->chosenHintIndex);
            break;
        case OPTIONS_MENU_UNKNOWN_6:
            // Load our current options?
            sOptionsMenu1State->newOptions = *gGameOptionsRef;
            CreateOthersDisplayScreen(&sOptionsMenu1State->newOptions);
            break;
        case OPTIONS_MENU_CONFIRM_NEW_OPTIONS:
            CreateChangeSettingsConfirmMenu();
            CreateMenuDialogueBoxAndPortrait(sChangeSettingsPrompt, 0, 4, sOptionsMenu1State->menuItems, 0, 4, 0, 0, 32);
            break;
        case OPTIONS_MENU_ROMHACK_OPTIONS:
            CreateRomhackOptionsMenu();
            break;
    }
}

static void CreateOthersMenu(void)
{
    s32 loopMax;

    MemoryFill16(sOptionsMenu1State->unkAC, 0, sizeof(sOptionsMenu1State->unkAC));
    loopMax = 0;

    sOptionsMenu1State->menuItems[loopMax].text = sGameOptions;
    sOptionsMenu1State->menuItems[loopMax].menuAction = MENU_OPTION_GAMEOPTIONS;

    loopMax++;
    sOptionsMenu1State->menuItems[loopMax].text = sRomhackOptions;
    sOptionsMenu1State->menuItems[loopMax].menuAction = MENU_OPTION_ROMHACK_OPTIONS;

    loopMax++;
    sOptionsMenu1State->menuItems[loopMax].text = sHints;
    sOptionsMenu1State->menuItems[loopMax].menuAction = MENU_OPTION_HINTS;

    loopMax++;
    sOptionsMenu1State->menuItems[loopMax].text = NULL;
    sOptionsMenu1State->menuItems[loopMax].menuAction = MENU_OPTION_DEFAULT;
}

static void CreateChangeSettingsConfirmMenu(void)
{
    s32 loopMax;

    MemoryFill16(sOptionsMenu1State->unkAC, 0, sizeof(sOptionsMenu1State->unkAC));
    loopMax = 0;

    sOptionsMenu1State->menuItems[loopMax].text = gCommonYes[0];
    sOptionsMenu1State->menuItems[loopMax].menuAction = MENU_OPTION_YES;

    loopMax++;
    sOptionsMenu1State->menuItems[loopMax].text = gCommonNo[0];
    sOptionsMenu1State->menuItems[loopMax].menuAction = MENU_OPTION_NO;

    loopMax++;
    sOptionsMenu1State->menuItems[loopMax].text = NULL;
    sOptionsMenu1State->menuItems[loopMax].menuAction = MENU_OPTION_DEFAULT;
}

static void CreateRomhackOptionsMenu(void)
{
    s32 LoopMax;
    (void)LoopMax;
}

static void HandleOthersMenu(void)
{
    s32 menuAction;

    menuAction = 0;

    if (sub_8012FD8(&sOptionsMenu1State->menu) == 0) {
        sub_8013114(&sOptionsMenu1State->menu, &menuAction);
        sOptionsMenu1State->menuAction = menuAction;
    }

    switch (menuAction) {
        case MENU_OPTION_HINTS:
            SetOptionsMenuState(OPTIONS_MENU_PRE_HINT_SELECTION);
            break;
        case MENU_OPTION_GAMEOPTIONS:
            SetOptionsMenuState(OPTIONS_MENU_UNKNOWN_6);
            break;
        case MENU_OPTION_ROMHACK_OPTIONS:
            SetRomhackOptionsMenuState(OPTIONS_MENU_ROMHACK_OPTIONS);
            break;
        case MENU_OPTION_DEFAULT:
            SetOptionsMenuState(OPTIONS_MENU_EXIT);
            break;
    }
}

static void sub_801E088(void)
{
    switch (sub_801E474(TRUE)) {
        case 3:
        case 4:
            sOptionsMenu1State->chosenHintIndex = GetChosenHintIndex();
            SetOptionsMenuState(OPTIONS_MENU_DISPLAY_HINT);
            break;
        case 2:
            sub_801E54C();
            SetOptionsMenuState(OPTIONS_MENU_MAIN);
            break;
        case 0:
        case 1:
        default:
            break;
    }
}

static void sub_801E0E0(void)
{
    switch (HandleHintDisplayScreenInput()) {
        case 3:
        case 2:
            DestroyHintDisplayScreen();
            SetOptionsMenuState(OPTIONS_MENU_HINT_SELECTION);
            break;
        case 0:
        case 1:
        default:
            break;
    }
}

static void sub_801E0FC(void)
{
    switch (sub_801E218()) {
        case 2:
        case 3:
            sub_801E2C4();

            // Check to see if the options changed?
            if (GameOptionsNotChange(&sOptionsMenu1State->newOptions))
                SetOptionsMenuState(OPTIONS_MENU_MAIN);
            else
                SetOptionsMenuState(OPTIONS_MENU_CONFIRM_NEW_OPTIONS);
            break;
        case 0:
        case 1:
            break;
    }
}

static void HandleChangeSettingsMenu(void)
{
    s32 menuAction;

    if (sub_80144A4(&menuAction))
        return;

    switch (menuAction) {
        case MENU_OPTION_YES:
            // Save our option changes??
            *gGameOptionsRef = sOptionsMenu1State->newOptions;
            SetWindowBGColor();
            sub_8099690(0);
            SetOptionsMenuState(OPTIONS_MENU_MAIN);
            break;
        case MENU_OPTION_DEFAULT:
        case MENU_OPTION_NO:
            SetOptionsMenuState(OPTIONS_MENU_MAIN);
            break;
    }
}
