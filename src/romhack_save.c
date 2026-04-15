#include "romhack_save.h"

#include "data_serializer.h"
#include "romhack_data.h"

s32 SaveRomhackData(u8 *array_ptr, s32 size) {
    DataSerializer backup;
    InitBitWriter(&backup, array_ptr, size);

    WriteBits(&backup, &gRomhackData.version, sizeof(gRomhackData.version));
    
    FinishBitSerializer(&backup);
    return backup.count;
}

s32 RestoreRomhackData(u8 *array_ptr, s32 size) {
    DataSerializer backup;
    InitBitReader(&backup, array_ptr, size);

    ReadBits(&backup, &gRomhackData.version, sizeof(gRomhackData.version));

    FinishBitSerializer(&backup);
    return backup.count;
}

s32 RomhackSaveSize() {
    return sizeof(gRomhackData.version);
}
