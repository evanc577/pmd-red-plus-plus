#include "global.h"

ALIGNED(4) static const u8 sRomhackOptions[] = _("Romhack Options");
ALIGNED(4) static const u8 sStringDebug1[] = _("DEBUG 1");

static const WindowTemplate sDefaultWindowTemplate = {
    .flags = 0,
    .type = WINDOW_TYPE_NORMAL,
    .pos = { 0, 0 },
    .width = 0, .height = 0,
    .totalHeight = 0,
    .unk12 = 0,
    .header = NULL,
};

static const WindowHeader sWindowHeader = {
    .count = 1,
    .currId = 0,
    .width = 12,
    .f3 = 0,
};

static const WindowTemplate sWindowTemplate = {
    .flags = 0,
    .type = WINDOW_TYPE_WITH_HEADER,
    .pos = { 2, 2 },
    .width = 24, .height = 6,
    .totalHeight = 18,
    .unk12 = 0,
    .header = &sWindowHeader
};
