#ifndef GUARD_ROMHACK_UTIL_H
#define GUARD_ROMHACK_UTIL_H

#include "structs/dungeon_entity.h"
typedef enum TryAttackType {
    TRY_ATTACK_TYPE_MOVE_0 = 0,
    TRY_ATTACK_TYPE_MOVE_1 = 1,
    TRY_ATTACK_TYPE_MOVE_2 = 2,
    TRY_ATTACK_TYPE_MOVE_3 = 3,
    TRY_ATTACK_TYPE_SET_MOVE,
    TRY_ATTACK_TYPE_SET_ITEM,
} TryAttackType;

typedef enum TryAttackResult {
    TRY_ATTACK_RESULT_MOVE_0 = 0,
    TRY_ATTACK_RESULT_MOVE_1 = 1,
    TRY_ATTACK_RESULT_MOVE_2 = 2,
    TRY_ATTACK_RESULT_MOVE_3 = 3,
    TRY_ATTACK_RESULT_STRUGGLE,
    TRY_ATTACK_RESULT_SET_ITEM,
    TRY_ATTACK_RESULT_CANCEL,
} TryAttackResult;

TryAttackResult TryAttack(TryAttackType attackType, Entity *entity);

#endif // GUARD_ROMHACK_UTIL_H
