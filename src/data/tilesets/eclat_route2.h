static const u32 sTiles_EclatRockyPathSecondary[] = INCBIN_U32("data/tilesets/secondary/eclat_rocky_path/tiles.bin");
static const u16 ALIGNED(4) sPalettes_EclatRockyPathSecondary[] = INCBIN_U16("data/tilesets/secondary/eclat_rocky_path/palettes.bin");
static const u16 sMetatiles_EclatRockyPathSecondary[] = INCBIN_U16("data/tilesets/secondary/eclat_rocky_path/metatiles.bin");
static const u32 sAttributes_EclatRockyPathSecondary[] = INCBIN_U32("data/tilesets/secondary/eclat_rocky_path/metatile_attributes.bin");
const struct Tileset gTileset_EclatRockyPathSecondary = {
    .isSecondary = TRUE,
    .tiles = sTiles_EclatRockyPathSecondary, .palettes = (const u16 (*)[16])sPalettes_EclatRockyPathSecondary,
    .metatiles = sMetatiles_EclatRockyPathSecondary, .metatileAttributes = (const u16 *)sAttributes_EclatRockyPathSecondary,
};

static const u32 sTiles_EclatRoute2GateSecondary[] = INCBIN_U32("data/tilesets/secondary/eclat_route2_gate/tiles.bin");
static const u16 ALIGNED(4) sPalettes_EclatRoute2GateSecondary[] = INCBIN_U16("data/tilesets/secondary/eclat_route2_gate/palettes.bin");
static const u16 sMetatiles_EclatRoute2GateSecondary[] = INCBIN_U16("data/tilesets/secondary/eclat_route2_gate/metatiles.bin");
static const u32 sAttributes_EclatRoute2GateSecondary[] = INCBIN_U32("data/tilesets/secondary/eclat_route2_gate/metatile_attributes.bin");
const struct Tileset gTileset_EclatRoute2GateSecondary = {
    .isSecondary = TRUE,
    .tiles = sTiles_EclatRoute2GateSecondary, .palettes = (const u16 (*)[16])sPalettes_EclatRoute2GateSecondary,
    .metatiles = sMetatiles_EclatRoute2GateSecondary, .metatileAttributes = (const u16 *)sAttributes_EclatRoute2GateSecondary,
};

static const u32 sTiles_EclatRoute2CenterSecondary[] = INCBIN_U32("data/tilesets/secondary/eclat_route2_center/tiles.bin");
static const u16 ALIGNED(4) sPalettes_EclatRoute2CenterSecondary[] = INCBIN_U16("data/tilesets/secondary/eclat_route2_center/palettes.bin");
static const u16 sMetatiles_EclatRoute2CenterSecondary[] = INCBIN_U16("data/tilesets/secondary/eclat_route2_center/metatiles.bin");
static const u32 sAttributes_EclatRoute2CenterSecondary[] = INCBIN_U32("data/tilesets/secondary/eclat_route2_center/metatile_attributes.bin");
const struct Tileset gTileset_EclatRoute2CenterSecondary = {
    .isSecondary = TRUE,
    .tiles = sTiles_EclatRoute2CenterSecondary, .palettes = (const u16 (*)[16])sPalettes_EclatRoute2CenterSecondary,
    .metatiles = sMetatiles_EclatRoute2CenterSecondary, .metatileAttributes = (const u16 *)sAttributes_EclatRoute2CenterSecondary,
};

static const u32 sTiles_PeacefulWoodsSecondary[] = INCBIN_U32("data/tilesets/secondary/peaceful_woods/tiles.bin");
static const u16 ALIGNED(4) sPalettes_PeacefulWoodsSecondary[] = INCBIN_U16("data/tilesets/secondary/peaceful_woods/palettes.bin");
static const u16 sMetatiles_PeacefulWoodsSecondary[] = INCBIN_U16("data/tilesets/secondary/peaceful_woods/metatiles.bin");
static const u32 sAttributes_PeacefulWoodsSecondary[] = INCBIN_U32("data/tilesets/secondary/peaceful_woods/metatile_attributes.bin");
const struct Tileset gTileset_PeacefulWoodsSecondary = {
    .isSecondary = TRUE,
    .tiles = sTiles_PeacefulWoodsSecondary, .palettes = (const u16 (*)[16])sPalettes_PeacefulWoodsSecondary,
    .metatiles = sMetatiles_PeacefulWoodsSecondary, .metatileAttributes = (const u16 *)sAttributes_PeacefulWoodsSecondary,
};
