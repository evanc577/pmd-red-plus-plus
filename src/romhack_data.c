#include "romhack_data.h"

EWRAM_INIT RomhackData gRomhackData = {};

void initializeRomhackData(RomhackData *data) {
    data->version = 0;
}

bool8 RomhackDataNotChanged(RomhackData *newData) {
    return TRUE;
}
