static const WindowTemplate sDefaultWindowTemplate = {
    .flags = 0,
    .type = 3,
    .pos = { 0, 0 },
    .width = 0, .height = 0,
    .totalHeight = 0,
    .unk12 = 0,
    .header = NULL,
};

static const WindowHeader sWindowHeader = {
    .count = 1,
    .currId = 0,
    .width = 7,
    .f3 = 0,
};

static const WindowTemplate sWindowTemplate = {
    .flags = 0,
    .type = 6,
    .pos = { 2, 2 },
    .width = 10, .height = 6,
    .totalHeight = 6,
    .unk12 = 0,
    .header = &sWindowHeader
};

ALIGNED(4) static const u8 sOthers[] = _("Others");
ALIGNED(4) static const u8 sChangeSettingsPrompt[] = _("Change settings?");
ALIGNED(4) static const u8 sGameOptions[] = _("Game Options");
ALIGNED(4) static const u8 sHints[] = _("Hints");

ALIGNED(4) static const u8 sRomhackOptions[] = _("Romhack Options");
