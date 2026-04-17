#include "structs/str_text.h"

static const WindowTemplate sUnknown_80DC020 = {
    .flags = 0,
    .type = WINDOW_TYPE_NORMAL,
    .pos = { 0, 0 },
    .width = 0, .height = 0,
    .totalHeight = 0,
    .unk12 = 0,
    .header = NULL
};

static const WindowHeader sUnknown_80DC038 = {
    .count = 1,
    .currId = 0,
    .width = 10,
    .f3 = 0
};
static const WindowTemplate sUnknown_80DC03C = {
    .flags = 0,
    .type = WINDOW_TYPE_WITH_HEADER,
    .pos = { 2, 2 },
    .width = 24, .height = 16,
    .totalHeight = 18,
    .unk12 = 0,
    .header = &sUnknown_80DC038
};

ALIGNED(4) static const u8 sGameOptions[] = _("Game Options");
ALIGNED(4) static const u8 sUnknown_80DC064[] = _("Windows{MOVE_X_POSITION_BY_80}Blue{MOVE_X_POSITION_BY_120}Red{MOVE_X_POSITION_BY_160}Green");
ALIGNED(4) static const u8 sBlue[] = _("Blue");
ALIGNED(4) static const u8 sRed[] = _("Red");
ALIGNED(4) static const u8 sGreen[] = _("Green");
