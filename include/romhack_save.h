#ifndef GUARD_ROMHACK_SAVE_H
#define GUARD_ROMHACK_SAVE_H

#include "gba/gba.h"

s32 SaveRomhackData(u8 *array_ptr, s32 size);
s32 RestoreRomhackData(u8 *array_ptr, s32 size);
inline s32 RomhackSaveSize();

#endif // GUARD_ROMHACK_SAVE_H
