#ifndef GUARD_ROMHACK_DATA_H
#define GUARD_ROMHACK_DATA_H

#include "gba/gba.h"
#include "structs/save.h"

extern RomhackData gRomhackData;

void InitializeRomhackData();
bool8 LoadRomhackSaveData(struct UnkStruct_sub_8011DAC *save);
bool8 RomhackDataNotChanged(RomhackData *newData);

typedef enum RomhackOptionType {
    ROMHACK_OPTION_TEXT_ONLY,
    ROMHACK_OPTION_MULTIPLIER,
    ROMHACK_OPTION_ON_OFF,
} RomhackOptionType;

typedef struct RomhackOption {
    const char *const name;
    const RomhackOptionType type;
    void *const data;
} RomhackOption;

#endif // GUARD_ROMHACK_DATA_H
