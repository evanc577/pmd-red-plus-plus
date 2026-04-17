#include "romhack_data.h"
#include "memory.h"
#include <string.h>

EWRAM_INIT RomhackData gRomhackData = {};

extern const char *gRomhackName;

static RomhackMultiplier DefaultRomhackMultiplier();

const RomhackOption gRomhackOptions[] = {
    {
        .name = "Friend area cost",
        .type = ROMHACK_OPTION_MULTIPLIER,
        .data = (void *)(&gRomhackData.friendAreaCostMult),
    },
    {
        .name = "Gummi IQ eff",
        .type = ROMHACK_OPTION_MULTIPLIER,
        .data = (void *)(&gRomhackData.gummiIqMult),
    },
};

void InitializeRomhackData() {
    gRomhackData.version = 0;
    gRomhackData.friendAreaCostMult = DefaultRomhackMultiplier();
    gRomhackData.gummiIqMult = DefaultRomhackMultiplier();
}

bool8 RomhackDataNotChanged(RomhackData *newData) {
    return TRUE;
}

static RomhackMultiplier DefaultRomhackMultiplier() {
    RomhackMultiplier mult ={
        .value = 100,
    };
    return mult;
}

bool8 LoadRomhackSaveData(struct UnkStruct_sub_8011DAC *save) {
    u32 version;
    s32 i;
    // Read gameInternalName, if it is not "ROM_________HACK", assume we need to create a new save.
    for (i = 0; i < sizeof(save->gameInternalName); ++i) {
        if (gRomhackName[i] != save->gameInternalName[i]) {
            InitializeRomhackData();
            return TRUE;
        }
    }

    // Read the save version, which is always the first field
    version = *(u32 *)(save->savedRomhackData);
    switch (version) {
        case 0:
            MemoryCopy8(&gRomhackData, save->savedRomhackData, sizeof(RomhackDataV0));
            break;
        case 1:
            MemoryCopy8(&gRomhackData, save->savedRomhackData, sizeof(RomhackDataV1));
            break;
        default:
            return FALSE;
    }

    // Update fields between versions
    switch (version) {
        case 0:
            gRomhackData.friendAreaCostMult = DefaultRomhackMultiplier();
            gRomhackData.gummiIqMult = DefaultRomhackMultiplier();
            // fallthrough
        case 1:
        default:
            break;
    }
    gRomhackData.version = 1;
    return TRUE;
}
