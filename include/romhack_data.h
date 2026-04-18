#ifndef GUARD_ROMHACK_DATA_H
#define GUARD_ROMHACK_DATA_H

#include "gba/gba.h"
#include "structs/save.h"

extern RomhackData gRomhackData;

void InitializeRomhackData();
bool8 LoadRomhackSaveData(struct UnkStruct_sub_8011DAC *save);
bool8 RomhackDataNotChanged(RomhackData *newData);

#endif // GUARD_ROMHACK_DATA_H
