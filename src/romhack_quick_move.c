#include "romhack_quick_move.h"
#include "dungeon_action.h"
#include "dungeon_menu_team.h"
#include "dungeon_misc.h"
#include "dungeon_move_util.h"
#include "dungeon_range.h"
#include "dungeon_strings.h"
#include "dungeon_util.h"
#include "dungeon_vram.h"
#include "input.h"
#include "items.h"
#include "menu_input.h"
#include "moves.h"
#include "romhack_util.h"
#include "string_format.h"
#include "structs/menu.h"
#include "text_1.h"
#include "text_2.h"
#include "text_3.h"

EWRAM_INIT MenuInputStruct sInput = {};

#define NUM_MENU_ITEMS (MAX_MON_MOVES + 1)

static const WindowHeader sWindowHeader = {
    .count = 1,
    .currId = 0,
    .width = 12,
    .f3 = 0,
};

const WindowTemplates gQuickMoveWindow =
{
    .id = {
        [0] = {
            .type = WINDOW_TYPE_WITH_HEADER,
            .pos = { 6, 3 },
            .width = 18,
            .height = (NUM_MENU_ITEMS * DEFAULT_MENU_ENTRY_HEIGHT) / 8 + 3,
            .totalHeight = (NUM_MENU_ITEMS * DEFAULT_MENU_ENTRY_HEIGHT) / 8 + 3,
            .unk12 = 0,
            .header = &sWindowHeader,
        },
        [1] = WIN_TEMPLATE_DUMMY,
        [2] = WIN_TEMPLATE_DUMMY,
        [3] = WIN_TEMPLATE_DUMMY,
    },
};

static void PrintOnMenu();
static void PrintMoves(u32 windowId, Entity *entity);

void ShowRomhackQuickMoveScreen() {
    bool8 dismissed = FALSE;
    Entity *leader = GetLeader();

    while (TRUE) {
        SetLeaderActionToNothing(TRUE);
        gTeamMenuChosenId = -1;
        PrintOnMenu();
        TryPointCameraToMonster(GetLeader(), 0);
        while (TRUE) {
            DungeonRunFrameActions(0x1D);
            if (0 == (gRealInputs.held & L_BUTTON)) {
                dismissed = TRUE;
                break;
            } else if (gRealInputs.pressed & (A_BUTTON | B_BUTTON | START_BUTTON | SELECT_BUTTON)) {
                TryAttackType attackType;
                if (gRealInputs.pressed & A_BUTTON) {
                    attackType = TRY_ATTACK_TYPE_MOVE_0;
                } else if (gRealInputs.pressed & B_BUTTON) {
                    attackType = TRY_ATTACK_TYPE_MOVE_1;
                } else if (gRealInputs.pressed & START_BUTTON) {
                    attackType = TRY_ATTACK_TYPE_MOVE_2;
                } else {
                    attackType = TRY_ATTACK_TYPE_MOVE_3;
                }
                if (TRY_ATTACK_RESULT_CANCEL != TryAttack(attackType, leader)) {
                    dismissed = TRUE;
                    break;
                }
            } else if (gRealInputs.pressed & R_BUTTON) {
                if (TRY_ATTACK_RESULT_CANCEL != TryAttack(TRY_ATTACK_TYPE_SET_ITEM, leader)) {
                    dismissed = TRUE;
                    break;
                }
            }
        }
        DungeonRunFrameActions(0x1D);

        // B button
        if (dismissed) {
            break;
        }

        ResetRepeatTimers();
        ResetUnusedInputStruct();
    }

    sub_803EAF0(0, NULL);
    ResetRepeatTimers();
    ResetUnusedInputStruct();
}

static void PrintOnMenu()
{
    Entity *entity = GetLeader();
    const u32 windowId = 0;

    sInput.menuIndex = 0;
    sInput.currPageEntries = NUM_MENU_ITEMS;
    sInput.entriesPerPage = NUM_MENU_ITEMS;
    sInput.currPage = 0;
    sInput.unk4 = 0;
    sInput.firstEntryY = 16;
    sInput.leftRightArrowsPos.x = 0;
    sInput.leftRightArrowsPos.y = 0;
    sInput.windowId = 0;
    sInput.unk14.x = 0;
    ResetTouchScreenMenuInput(&sInput.touchScreen);
    sub_803EAF0(DUNGEON_WINDOW_TYPE_QUICK_MOVE, NULL);
    sub_80137B0(&sInput, 0);

    sub_80073B8(0);

    SubstitutePlaceholderStringTags(gFormatBuffer_Monsters[0], entity, 0);
    PrintFormattedStringOnWindow(16, 0, gUnknown_80FE978, 0, '\0');

    PrintMoves(windowId, entity);

    sub_80073E0(0);
}

static void PrintMoves(u32 windowId, Entity *entity) {
    // Print moves
    s32 i;
    const u8 *const icons[MAX_MON_MOVES] = {
        _("{A_BUTTON}"),
        _("{B_BUTTON}"),
        _("θ"),
        _("ι"),
    };
    MoveBufferStruct movStruct = {
        .style = BUFFER_MOVE_POSITIONED_PP,
        .xPPCoord = X_PP_COORD_DEFAULT,
        .redColor = FALSE,
        .useRedColorForChargingMoves = FALSE,
    };

    for (i = 0; i < MAX_MON_MOVES; ++i) {
        Move *move = &GetEntInfo(entity)->moves.moves[i];
        if (MoveFlagExists(move)) {
            s32 x, y;
            movStruct.redColor = (CanMonsterUseMove(entity, move, TRUE) == FALSE);
            BufferMoveName(gFormatBuffer_Items[0], move, &movStruct);
            y = GetMenuEntryYCoord(&sInput, i);
            if MOVE_FLAG_LINK_CHAIN(move) {
                x = 13;
            } else {
                x = 8;
                if (i != 0) {
                    AddDoubleUnderScoreHighlight(windowId, 5, y - 2, 130, 7);
                }
                PrintFormattedStringOnWindow(5, y, icons[i], windowId, '\0');
            }
            x += 10;
            PrintFormattedStringOnWindow(x, y, _("{MOVE_ITEM_0}"), windowId, '\0');
        }
    }

    // Print set item
    for (i = 0; i < INVENTORY_SIZE; i++) {
        if (ItemExists(&gTeamInventoryRef->teamItems[i]) && ItemSet(&gTeamInventoryRef->teamItems[i])) {
            break;
        }
    }
    if (i < INVENTORY_SIZE) {
        s32 y = GetMenuEntryYCoord(&sInput, MAX_MON_MOVES);
        unkStruct_8090F58 flags = { .unk8 = 1 };    // Need this to show quantity
        sub_8090E14(gFormatBuffer_Items[0], &gTeamInventoryRef->teamItems[i], &flags); 
        PrintFormattedStringOnWindow(5, y, _("{R_BUTTON}"), windowId, '\0');
        PrintFormattedStringOnWindow(18, y, _("{MOVE_ITEM_0}"), windowId, '\0');
    }
}
