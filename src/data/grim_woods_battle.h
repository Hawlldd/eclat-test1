static const u32 sGrimWoodsGrassBattleTiles[] = INCBIN_U32("graphics/battle_environment/grim_woods/grass_tiles.lz.bin");
static const u32 sGrimWoodsGrassBattleMap[] = INCBIN_U32("graphics/battle_environment/grim_woods/grass_map.lz.bin");
static const u32 sGrimWoodsGrassBattleEntryTiles[] = INCBIN_U32("graphics/battle_environment/grim_woods/grass_entry_tiles.lz.bin");
static const u32 sGrimWoodsGrassBattleEntryMap[] = INCBIN_U32("graphics/battle_environment/grim_woods/grass_entry_map.lz.bin");
static const u16 sGrimWoodsGrassDayBattlePalette[] = INCBIN_U16("graphics/battle_environment/grim_woods/grass_palette_day.gbapal.bin");
static const u16 sGrimWoodsGrassEveningBattlePalette[] = INCBIN_U16("graphics/battle_environment/grim_woods/grass_palette_evening.gbapal.bin");
static const u16 sGrimWoodsGrassNightBattlePalette[] = INCBIN_U16("graphics/battle_environment/grim_woods/grass_palette_night.gbapal.bin");

static const u32 sGrimWoodsPlainBattleTiles[] = INCBIN_U32("graphics/battle_environment/grim_woods/plain_tiles.lz.bin");
static const u32 sGrimWoodsPlainBattleMap[] = INCBIN_U32("graphics/battle_environment/grim_woods/plain_map.lz.bin");
static const u32 sGrimWoodsPlainBattleEntryTiles[] = INCBIN_U32("graphics/battle_environment/grim_woods/plain_entry_tiles.lz.bin");
static const u32 sGrimWoodsPlainBattleEntryMap[] = INCBIN_U32("graphics/battle_environment/grim_woods/plain_entry_map.lz.bin");
static const u16 sGrimWoodsPlainDayBattlePalette[] = INCBIN_U16("graphics/battle_environment/grim_woods/plain_palette_day.gbapal.bin");
static const u16 sGrimWoodsPlainEveningBattlePalette[] = INCBIN_U16("graphics/battle_environment/grim_woods/plain_palette_evening.gbapal.bin");
static const u16 sGrimWoodsPlainNightBattlePalette[] = INCBIN_U16("graphics/battle_environment/grim_woods/plain_palette_night.gbapal.bin");

static const struct BattleEnvironment sGrimWoodsGrassDayBattleGraphics =
{
    .entry = {sGrimWoodsGrassBattleEntryTiles, sGrimWoodsGrassBattleEntryMap},
    .background = {sGrimWoodsGrassBattleTiles, sGrimWoodsGrassBattleMap},
    .palette = sGrimWoodsGrassDayBattlePalette,
};

static const struct BattleEnvironment sGrimWoodsGrassEveningBattleGraphics =
{
    .entry = {sGrimWoodsGrassBattleEntryTiles, sGrimWoodsGrassBattleEntryMap},
    .background = {sGrimWoodsGrassBattleTiles, sGrimWoodsGrassBattleMap},
    .palette = sGrimWoodsGrassEveningBattlePalette,
};

static const struct BattleEnvironment sGrimWoodsGrassNightBattleGraphics =
{
    .entry = {sGrimWoodsGrassBattleEntryTiles, sGrimWoodsGrassBattleEntryMap},
    .background = {sGrimWoodsGrassBattleTiles, sGrimWoodsGrassBattleMap},
    .palette = sGrimWoodsGrassNightBattlePalette,
};

static const struct BattleEnvironment sGrimWoodsPlainDayBattleGraphics =
{
    .entry = {sGrimWoodsPlainBattleEntryTiles, sGrimWoodsPlainBattleEntryMap},
    .background = {sGrimWoodsPlainBattleTiles, sGrimWoodsPlainBattleMap},
    .palette = sGrimWoodsPlainDayBattlePalette,
};

static const struct BattleEnvironment sGrimWoodsPlainEveningBattleGraphics =
{
    .entry = {sGrimWoodsPlainBattleEntryTiles, sGrimWoodsPlainBattleEntryMap},
    .background = {sGrimWoodsPlainBattleTiles, sGrimWoodsPlainBattleMap},
    .palette = sGrimWoodsPlainEveningBattlePalette,
};

static const struct BattleEnvironment sGrimWoodsPlainNightBattleGraphics =
{
    .entry = {sGrimWoodsPlainBattleEntryTiles, sGrimWoodsPlainBattleEntryMap},
    .background = {sGrimWoodsPlainBattleTiles, sGrimWoodsPlainBattleMap},
    .palette = sGrimWoodsPlainNightBattlePalette,
};
