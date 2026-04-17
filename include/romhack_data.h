#ifndef GUARD_ROMHACK_DATA_H
#define GUARD_ROMHACK_DATA_H

#include "gba/gba.h"

typedef struct RomhackDataV0 {
    u32 version;
} RomhackDataV0;

extern RomhackDataV0 gRomhackData;

void InitializeRomhackData();

bool8 RomhackDataNotChanged(RomhackDataV0 *newData);

#endif // GUARD_ROMHACK_DATA_H
