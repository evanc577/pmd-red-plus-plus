#include "romhack_util.h"
#include "dungeon_action.h"
#include "dungeon_message.h"
#include "dungeon_strings.h"
#include "dungeon_util.h"
#include "moves.h"

TryAttackResult TryAttack(TryAttackType attackType, Entity *entity) {
    EntityInfo *entInfo = GetEntInfo(entity);
    s32 moveIdx, j;
    bool8 canUseMove;

    if (attackType == TRY_ATTACK_TYPE_SET_ITEM) {
        s32 i;
        for (i = 0; i < INVENTORY_SIZE; i++) {
            if (ItemExists(&gTeamInventoryRef->teamItems[i]) && ItemSet(&gTeamInventoryRef->teamItems[i])) {
                SetLeaderActionFields(ACTION_THROW_ITEM_PLAYER);
                entInfo->action.actionParameters[0].actionUseIndex = i +1;
                entInfo->action.actionParameters[0].itemPos.x = 0;
                entInfo->action.actionParameters[0].itemPos.y = 0;
                break;
            }
        }
        if (entInfo->action.action != 0) {
            return TRY_ATTACK_RESULT_SET_ITEM;
        }
        return TRY_ATTACK_RESULT_CANCEL;
    } else if (attackType == TRY_ATTACK_TYPE_SET_MOVE) {
        // Check if any moves are SET
        for (moveIdx = 0; moveIdx < MAX_MON_MOVES; moveIdx++) {
            if (MoveFlagExists(&entInfo->moves.moves[moveIdx]) && MoveFlagSet(&entInfo->moves.moves[moveIdx])) {
                break;
            }
        }
        if (moveIdx == MAX_MON_MOVES) {
            LogMessageByIdWithPopupCheckUser_Async(entity, gUnknown_80F8A28);
            return TRY_ATTACK_RESULT_CANCEL;
        }
    } else {
        // Move index was explicitly requested
        moveIdx = attackType;
        if (!MoveFlagExists(&entInfo->moves.moves[moveIdx])) {
            return TRY_ATTACK_RESULT_CANCEL;
        }
    }

    // If all moves have no PP left, use struggle
    for (j = 0; j < MAX_MON_MOVES; j++) {
        if (MoveFlagExists(&entInfo->moves.moves[j])) {
            if (entInfo->moves.moves[j].PP != 0)
                break;
        }
    }
    if (j == MAX_MON_MOVES) {
        SetMonsterActionFields(&entInfo->action, ACTION_STRUGGLE);
        return TRY_ATTACK_RESULT_STRUGGLE;
    }

    // Disallow using moves if it isn't the first in a chain
    if (entInfo->moves.moves[moveIdx].moveFlags & MOVE_FLAG_SUBSEQUENT_IN_LINK_CHAIN) {
        return TRY_ATTACK_RESULT_CANCEL;
    }

    // Only allow move if at least one move in the chain has PP
    canUseMove = FALSE;
    for (j = moveIdx; j < MAX_MON_MOVES; j++) {
        if (j != moveIdx && !(entInfo->moves.moves[j].moveFlags & MOVE_FLAG_SUBSEQUENT_IN_LINK_CHAIN)) {
            break;
        }
        if (entInfo->moves.moves[j].PP != 0) {
            canUseMove = TRUE;
            break;
        }
    }

    if (!canUseMove) {
        LogMessageByIdWithPopupCheckUser_Async(entity, gUnknown_80F8A4C);
        return TRY_ATTACK_RESULT_CANCEL;
    } else {
        SetMonsterActionFields(&entInfo->action, ACTION_USE_MOVE_PLAYER);
        entInfo->action.actionParameters[0].actionUseIndex = GetTeamMemberEntityIndex(entity);
        entInfo->action.actionParameters[1].actionUseIndex = moveIdx;
        return moveIdx;
    }
}
