#ifndef GUARD_STRUCTS_SAVE
#define GUARD_STRUCTS_SAVE

typedef struct RomhackMultiplier {
    u32 value;
} RomhackMultiplier;

typedef struct RomhackDataV0 {
    u32 version;
} RomhackDataV0;

typedef struct RomhackDataV1 {
    u32 version;
    RomhackMultiplier friendAreaCostMult;
    RomhackMultiplier gummiIqMult;
} RomhackDataV1;
typedef RomhackDataV1 RomhackData;

// size: 0x57D4
struct UnkStruct_sub_8011DAC
{
    u8 fill000[0x4];
    u8 unk004[0x400];
    u8 gameInternalName[0x10]; // has "POKE_DUNGEON__05"
    u32 checksum;
    u32 unk418;
    u32 unk41C;
    u32 RngState;
    u32 savedRecruitedPokemon;
    u32 unk428;
    u8 fill42C[0x4];
    u32 savedTeamInventory;
    u32 savedRescueTeamInfo;
    u32 savedFriendAreas;
    u32 unk43C;
    u32 unk440;
    u32 savedMailInfo;
    u8 unk448[0x538C];
    u8 savedRomhackData[sizeof(RomhackData)];
};

#endif
