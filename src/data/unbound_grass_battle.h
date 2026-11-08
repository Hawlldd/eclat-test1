static const u32 sUnboundGrassBattleTiles[] = INCBIN_U32("graphics/battle_environment/unbound_grass/tiles.lz.bin");
static const u32 sUnboundGrassBattleMap[] = INCBIN_U32("graphics/battle_environment/unbound_grass/map.lz.bin");
static const u32 sUnboundGrassBattleEntryTiles[] = INCBIN_U32("graphics/battle_environment/unbound_grass/entry_tiles.lz.bin");
static const u32 sUnboundGrassBattleEntryMap[] = INCBIN_U32("graphics/battle_environment/unbound_grass/entry_map.lz.bin");
static const u16 sUnboundGrassBattlePalette[] = INCBIN_U16("graphics/battle_environment/unbound_grass/palette.gbapal.bin");

static const struct BattleEnvironment sUnboundGrassBattleGraphics =
{
    .entry = {sUnboundGrassBattleEntryTiles, sUnboundGrassBattleEntryMap},
    .background = {sUnboundGrassBattleTiles, sUnboundGrassBattleMap},
    .palette = sUnboundGrassBattlePalette,
};
