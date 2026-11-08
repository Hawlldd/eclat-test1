void InitTilesetAnim_PetipresHouses(void);

static const u32 sTiles_PetipresSecondary[] = INCBIN_U32("data/tilesets/secondary/petipres/tiles.bin");
static const u16 ALIGNED(4) sPalettes_PetipresSecondary[] = INCBIN_U16("data/tilesets/secondary/petipres/palettes.bin");
static const u16 sMetatiles_PetipresSecondary[] = INCBIN_U16("data/tilesets/secondary/petipres/metatiles.bin");
static const u32 sAttributes_PetipresSecondary[] = INCBIN_U32("data/tilesets/secondary/petipres/metatile_attributes.bin");

const struct Tileset gTileset_PetipresSecondary =
{
    .isSecondary = TRUE,
    .tiles = sTiles_PetipresSecondary,
    .palettes = (const u16 (*)[16])sPalettes_PetipresSecondary,
    .metatiles = sMetatiles_PetipresSecondary,
    .metatileAttributes = (const u16 *)sAttributes_PetipresSecondary,
};

static const u32 sTiles_PetipresHousesPrimary[] = INCBIN_U32("data/tilesets/primary/petipres_houses/tiles.bin");
static const u16 ALIGNED(4) sPalettes_PetipresHousesPrimary[] = INCBIN_U16("data/tilesets/primary/petipres_houses/palettes.bin");
static const u16 sMetatiles_PetipresHousesPrimary[] = INCBIN_U16("data/tilesets/primary/petipres_houses/metatiles.bin");
static const u32 sAttributes_PetipresHousesPrimary[] = INCBIN_U32("data/tilesets/primary/petipres_houses/metatile_attributes.bin");

const struct Tileset gTileset_PetipresHousesPrimary =
{
    .isSecondary = FALSE,
    .tiles = sTiles_PetipresHousesPrimary,
    .palettes = (const u16 (*)[16])sPalettes_PetipresHousesPrimary,
    .metatiles = sMetatiles_PetipresHousesPrimary,
    .metatileAttributes = (const u16 *)sAttributes_PetipresHousesPrimary,
};

static const u32 sTiles_PetipresHousesSecondary[] = INCBIN_U32("data/tilesets/secondary/petipres_houses/tiles.bin");
static const u16 ALIGNED(4) sPalettes_PetipresHousesSecondary[] = INCBIN_U16("data/tilesets/secondary/petipres_houses/palettes.bin");
static const u16 sMetatiles_PetipresHousesSecondary[] = INCBIN_U16("data/tilesets/secondary/petipres_houses/metatiles.bin");
static const u32 sAttributes_PetipresHousesSecondary[] = INCBIN_U32("data/tilesets/secondary/petipres_houses/metatile_attributes.bin");

const struct Tileset gTileset_PetipresHousesSecondary =
{
    .isSecondary = TRUE,
    .tiles = sTiles_PetipresHousesSecondary,
    .palettes = (const u16 (*)[16])sPalettes_PetipresHousesSecondary,
    .metatiles = sMetatiles_PetipresHousesSecondary,
    .metatileAttributes = (const u16 *)sAttributes_PetipresHousesSecondary,
    .callback = InitTilesetAnim_PetipresHouses,
};

static const u32 sTiles_PetipresMartSecondary[] = INCBIN_U32("data/tilesets/secondary/petipres_mart/tiles.bin");
static const u16 ALIGNED(4) sPalettes_PetipresMartSecondary[] = INCBIN_U16("data/tilesets/secondary/petipres_mart/palettes.bin");
static const u16 sMetatiles_PetipresMartSecondary[] = INCBIN_U16("data/tilesets/secondary/petipres_mart/metatiles.bin");
static const u32 sAttributes_PetipresMartSecondary[] = INCBIN_U32("data/tilesets/secondary/petipres_mart/metatile_attributes.bin");

const struct Tileset gTileset_PetipresMartSecondary =
{
    .isSecondary = TRUE,
    .tiles = sTiles_PetipresMartSecondary,
    .palettes = (const u16 (*)[16])sPalettes_PetipresMartSecondary,
    .metatiles = sMetatiles_PetipresMartSecondary,
    .metatileAttributes = (const u16 *)sAttributes_PetipresMartSecondary,
};
