#include "romhack_data.h"

EWRAM_INIT RomhackDataV0 gRomhackData = {};

void InitializeRomhackData() {
    gRomhackData.version = 0;
}

bool8 RomhackDataNotChanged(RomhackDataV0 *newData) {
    return TRUE;
}
