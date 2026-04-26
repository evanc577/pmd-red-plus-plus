#include "romhack_data.h"
#include "memory.h"
#include <string.h>

EWRAM_INIT RomhackData gRomhackData = {};

extern const char *gRomhackName;

static RomhackMultiplier DefaultRomhackMultiplier();

void InitializeRomhackData() {
    gRomhackData.version = 0;
    gRomhackData.friendAreaCostMult = DefaultRomhackMultiplier();
    gRomhackData.gummiIqMult = DefaultRomhackMultiplier();
    gRomhackData.itemCostMult = DefaultRomhackMultiplier();
    gRomhackData.noExclusivePokemon = FALSE;
    gRomhackData.guaranteedLegendaryRecruit = FALSE;
    gRomhackData.hungerRate = DefaultRomhackMultiplier();
    gRomhackData.expMult = DefaultRomhackMultiplier();
}

bool8 RomhackDataNotChanged(RomhackData *newData) {
    // Poor man's memcmp
    u32 i;
    for (i = 0; i < sizeof(RomhackData); ++i) {
        u8 b1 = *(((u8 *)&gRomhackData) + i);
        u8 b2 = *(((u8 *)newData) + i);
        if (b1 != b2) {
            return FALSE;
        }
    }
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

    InitializeRomhackData();

    // Read gameInternalName, if it is not "ROM_________HACK", assume we need to create a new save.
    for (i = 0; i < sizeof(save->gameInternalName); ++i) {
        if (gRomhackName[i] != save->gameInternalName[i]) {
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
        case 2:
            MemoryCopy8(&gRomhackData, save->savedRomhackData, sizeof(RomhackDataV2));
            break;
        default:
            return FALSE;
    }

    gRomhackData.version = 2;
    return TRUE;
}
