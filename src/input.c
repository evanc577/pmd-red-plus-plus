#include "global.h"
#include "globaldata.h"
#include "code_800C9CC.h"
#include "input.h"

#define JUNK_INIT 0x4A14C1
#define JUNK_UPDATE 0x54A1C41

EWRAM_DATA Inputs gRealInputs = {0}; // R=20255F0 | B=20F5CC0
static EWRAM_DATA UnusedInputStruct sUnusedInputsRelated = {0}; // R=2025600 | B=020F5CD0
static EWRAM_DATA u32 sUnusedScrambledInputJunk[3] = {0}; // R=202562C | B=020F5C88
static EWRAM_DATA Inputs sBufferedInputs = {0}; // R=2025638 | B=020F5C90
static EWRAM_DATA Inputs sCurrentInputs = {0}; // R=2025648 | B=020F5CB0
static EWRAM_DATA Inputs sPrevInputs = {0}; // R=2025658 | B=020F5CA0
static EWRAM_DATA InputTimers sInputTimers = {0}; // R=2025668 | B=020F5C8C

static void UpdateRepeat(u16 buttonMask, u16 *heldButton, s32 *timer);
static void UpdateShortPress(u16 buttonMask, s16 *timer);

// arm9.bin::0200754C
void InitInput(void)
{
    gRealInputs.held = 0;
    gRealInputs.pressed = 0;
    gRealInputs.repeated = 0;
    gRealInputs.shortPress = 0;

    sBufferedInputs.held = 0;
    sBufferedInputs.pressed = 0;
    sBufferedInputs.repeated = 0;
    sBufferedInputs.shortPress = 0;

    sUnusedScrambledInputJunk[0] = JUNK_INIT;

    sUnusedInputsRelated.unk20 = 0;
    sUnusedInputsRelated.unk0 = 0xFFFF; // probably a mask
    sUnusedInputsRelated.unk2 = -1;
    sUnusedInputsRelated.unk4 = -1;
    sUnusedInputsRelated.unk6 = -1;
    sUnusedInputsRelated.unk1C = -1;
    sUnusedInputsRelated.unk1E = -1;
    sUnusedInputsRelated.unk8 = -1;
    sUnusedInputsRelated.unkA = -1;
    sUnusedInputsRelated.unkC = -1;
    sUnusedInputsRelated.unkE = -1;
    sUnusedInputsRelated.unk10 = -1;
    sUnusedInputsRelated.unk12 = -1;
    sUnusedInputsRelated.unk14 = -1;
    sUnusedInputsRelated.unk16 = -1;
    sUnusedInputsRelated.unk28 = 0;
    sUnusedInputsRelated.unk29 = 0;

    sInputTimers.holdTimerB = 0;
    sInputTimers.holdTimerR = 0;
}

// arm9.bin::02007200
void LoadBufferedInputs(void)
{
    gRealInputs = sBufferedInputs;

    sBufferedInputs.held = 0;
    sBufferedInputs.pressed = 0;
    sBufferedInputs.repeated = 0;
    sBufferedInputs.shortPress = 0;

    sUnusedInputsRelated.unk0 = 0xFFFF;
    sUnusedInputsRelated.unk2 = -1;
    sUnusedInputsRelated.unk4 = -1;
    sUnusedInputsRelated.unk6 = -1;
    sUnusedInputsRelated.unk8 = -1;
    sUnusedInputsRelated.unkA = -1;
    sUnusedInputsRelated.unkC = -1;
    sUnusedInputsRelated.unkE = -1;
    sUnusedInputsRelated.unk10 = -1;
    sUnusedInputsRelated.unk12 = -1;
    sUnusedInputsRelated.unk14 = -1;
    sUnusedInputsRelated.unk16 = -1;

    sUnusedInputsRelated.unk28 = 0;
    sUnusedInputsRelated.unk29 = 0;

    // This is way different in blue
}

// TODO: 2 funcs in blue. Not sure which they are in red, if they exist
// arm9.bin::020071CC
// arm9.bin::02007198

UNUSED static bool8 sub_80048B8(void)
{
    return FALSE;
}

bool8 sub_80048BC(void)
{
    return FALSE;
}

UNUSED static bool8 sub_80048C0(void)
{
    return FALSE;
}

UNUSED static bool8 sub_80048C4(void)
{
    return FALSE;
}

bool8 sub_80048C8(void)
{
    return FALSE;
}

UNUSED static bool8 sub_80048CC(void)
{
    return FALSE;
}

// arm9.bin::0200715C
void ResetRepeatTimers(void)
{
    gRealInputs.repeated = 0;
    sCurrentInputs.repeatTimerDpad = 0;
    sCurrentInputs.repeatTimerLButton = 0;
    sCurrentInputs.repeatTimerRButton = 0;
    sCurrentInputs.heldDpad = 0;
    sCurrentInputs.heldLButton = 0;
    sCurrentInputs.heldRButton = 0;
    sInputTimers.holdTimerB = 999;
    sInputTimers.holdTimerR = 999;
}

// arm9.bin::02007130
void UnpressButtons(void)
{
    gRealInputs.pressed = 0;
    sBufferedInputs.pressed = 0;
    sCurrentInputs.pressed = 0;
}

// arm9.bin::020070D4
void ResetUnusedInputStruct(void)
{
    sUnusedInputsRelated.unk20 = 5;
    sUnusedInputsRelated.unk24 = 0;
    sUnusedInputsRelated.unk0 = 0xFFFF;
    sUnusedInputsRelated.unk2 = -1;
    sUnusedInputsRelated.unk4 = -1;
    sUnusedInputsRelated.unk6 = -1;
    sUnusedInputsRelated.unk8 = -1;
    sUnusedInputsRelated.unkA = -1;
    sUnusedInputsRelated.unkC = -1;
    sUnusedInputsRelated.unkE = -1;
    sUnusedInputsRelated.unk10 = -1;
    sUnusedInputsRelated.unk12 = -1;
    sUnusedInputsRelated.unk14 = -1;
    sUnusedInputsRelated.unk16 = -1;

    sUnusedInputsRelated.unk28 = 0;
    sUnusedInputsRelated.unk29 = 0;
}

// arm9.bin::02006EC0
void UpdateInput(void)
{
    sPrevInputs = sCurrentInputs;

    ReadKeyInput(&sCurrentInputs);

    // Pressed inputs are have changed from unheld to held on this frame
    sCurrentInputs.pressed = (sPrevInputs.held ^ sCurrentInputs.held) & sCurrentInputs.held;

    // Handle repeated inputs
    sCurrentInputs.repeated = 0;
    UpdateRepeat(DPAD_ANY, &sCurrentInputs.heldDpad, &sCurrentInputs.repeatTimerDpad);
    UpdateRepeat(L_BUTTON, &sCurrentInputs.heldLButton, &sCurrentInputs.repeatTimerLButton);
    UpdateRepeat(R_BUTTON, &sCurrentInputs.heldRButton, &sCurrentInputs.repeatTimerRButton);

    // Handle short inputs
    sCurrentInputs.shortPress = 0;
    UpdateShortPress(B_BUTTON, &sInputTimers.holdTimerB);
    UpdateShortPress(R_BUTTON, &sInputTimers.holdTimerR);

    sBufferedInputs.held |= sCurrentInputs.held;
    sBufferedInputs.pressed |= sCurrentInputs.pressed;
    sBufferedInputs.repeated |= sCurrentInputs.repeated;
    sBufferedInputs.shortPress |= sCurrentInputs.shortPress;

    sUnusedScrambledInputJunk[0] *= sCurrentInputs.held | JUNK_UPDATE;
}

static void UpdateRepeat(u16 buttonMask, u16 *heldButton, s32 *timer) {
    if (sCurrentInputs.held) {
        if ((*heldButton & buttonMask) == (sCurrentInputs.held & buttonMask)) {
            // If the currently held input did not change, increment timer up to 50
            if (*timer < 50) {
                (*timer)++;
            }
        } else {
            // If the held input changed, reset the timer
            *heldButton = sCurrentInputs.held & buttonMask;
            *timer = 1;
        }
    } else {
        *timer = 0;
        *heldButton = 0;
    }

    if (*timer == 1) {
        // The first repeat fires immediately
        sCurrentInputs.repeated |= (*heldButton & buttonMask) | sCurrentInputs.pressed;
    } else if (*timer == 48) {
        // Subsequent repeats fire faster
        *timer = 43;
        sCurrentInputs.repeated |= (*heldButton & buttonMask) | sCurrentInputs.pressed;
    }
}

static void UpdateShortPress(u16 buttonMask, s16 *timer) {
    if (sCurrentInputs.held & buttonMask) {
        if (*timer < 100) {
            (*timer)++;
        }
    } else if (1 < *timer && *timer < 12) {
        sCurrentInputs.shortPress |= buttonMask;
        *timer = 0;
    } else {
        *timer = 0;
    }
}
