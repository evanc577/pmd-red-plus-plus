#ifndef GUARD_ROMHACK_DATA_H
#define GUARD_ROMHACK_DATA_H

#include "gba/gba.h"

typedef struct RomhackData {
    u32 version;
} RomhackData;

extern RomhackData gRomhackData;

void initializeRomhackData(RomhackData *data);
bool8 RomhackDataNotChanged(RomhackData *newData);

#endif // GUARD_ROMHACK_DATA_H
