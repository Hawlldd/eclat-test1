#include "global.h"
#include "sprite.h"
#include "event_object_movement.h"
#include "field_camera.h"
#include "fieldmap.h"
#include "constants/event_objects.h"
#include "constants/maps.h"

static void SpriteCB_Route2Flyer(struct Sprite *sprite);
static const struct OamData sFlyerOam = {
    .shape = SPRITE_SHAPE(32x32), .size = SPRITE_SIZE(32x32), .priority = 1,
};
static const union AnimCmd sFlyerLeft[] = {
    ANIMCMD_FRAME(0, 10), ANIMCMD_FRAME(16, 10), ANIMCMD_JUMP(0),
};
static const union AnimCmd sFlyerRight[] = {
    ANIMCMD_FRAME(0, 10, .hFlip = TRUE), ANIMCMD_FRAME(16, 10, .hFlip = TRUE), ANIMCMD_JUMP(0),
};
static const union AnimCmd *const sFlyerAnims[] = {sFlyerRight, sFlyerLeft};

static const u32 sFlyer65[] = INCBIN_U32("graphics/eclat_intro/route2/flyer_65.bin");
static const u32 sFlyer98[] = INCBIN_U32("graphics/eclat_intro/route2/flyer_98.bin");
static const u32 sFlyer9A[] = INCBIN_U32("graphics/eclat_intro/route2/flyer_9a.bin");

static const struct SpriteSheet sFlyerSheet65 = {
    .data = sFlyer65, .size = sizeof(sFlyer65), .tag = GFXTAG_ECLAT_FLYER_65,
};
static const struct SpriteSheet sFlyerSheet98 = {
    .data = sFlyer98, .size = sizeof(sFlyer98), .tag = GFXTAG_ECLAT_FLYER_98,
};
static const struct SpriteSheet sFlyerSheet9A = {
    .data = sFlyer9A, .size = sizeof(sFlyer9A), .tag = GFXTAG_ECLAT_FLYER_9A,
};

static const struct SpriteTemplate sFlyerTemplate65 = {
    .tileTag = GFXTAG_ECLAT_FLYER_65, .paletteTag = OBJ_EVENT_PAL_TAG_PETIPRES_04,
    .oam = &sFlyerOam, .anims = sFlyerAnims, .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable, .callback = SpriteCB_Route2Flyer,
};
static const struct SpriteTemplate sFlyerTemplate98 = {
    .tileTag = GFXTAG_ECLAT_FLYER_98, .paletteTag = OBJ_EVENT_PAL_TAG_PETIPRES_03,
    .oam = &sFlyerOam, .anims = sFlyerAnims, .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable, .callback = SpriteCB_Route2Flyer,
};
static const struct SpriteTemplate sFlyerTemplate9A = {
    .tileTag = GFXTAG_ECLAT_FLYER_9A, .paletteTag = OBJ_EVENT_PAL_TAG_PETIPRES_04,
    .oam = &sFlyerOam, .anims = sFlyerAnims, .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable, .callback = SpriteCB_Route2Flyer,
};
static const struct {
    u8 map, x, y, range;
    const struct SpriteTemplate *template;
} sRoutes[] = {
    { MAP_NUM(MAP_ECLAT_ROUTE2), 23, 9, 9, &sFlyerTemplate65 },
    { MAP_NUM(MAP_ECLAT_ROUTE2), 22, 10, 9, &sFlyerTemplate65 },
    { MAP_NUM(MAP_ECLAT_ROUTE2), 15, 31, 9, &sFlyerTemplate65 },
    { MAP_NUM(MAP_ECLAT_ROUTE2), 14, 30, 9, &sFlyerTemplate65 },
    { MAP_NUM(MAP_ECLAT_ROUTE2), 45, 13, 9, &sFlyerTemplate65 },
    { MAP_NUM(MAP_ECLAT_ROUTE2), 43, 24, 9, &sFlyerTemplate65 },
    { MAP_NUM(MAP_ECLAT_ROUTE2), 42, 25, 9, &sFlyerTemplate65 },
    { MAP_NUM(MAP_ECLAT_ROUTE2), 53, 0, 9, &sFlyerTemplate98 },
    { MAP_NUM(MAP_ECLAT_ROUTE2), 41, 1, 9, &sFlyerTemplate98 },
    { MAP_NUM(MAP_ECLAT_ROCKY_PATH), 24, 14, 9, &sFlyerTemplate9A },
    { MAP_NUM(MAP_ECLAT_ROCKY_PATH), 23, 15, 9, &sFlyerTemplate9A },
    { MAP_NUM(MAP_ECLAT_ROCKY_PATH), 23, 13, 9, &sFlyerTemplate9A },
    { MAP_NUM(MAP_ECLAT_ROCKY_PATH), 15, 21, 9, &sFlyerTemplate9A },
    { MAP_NUM(MAP_ECLAT_ROCKY_PATH), 14, 22, 9, &sFlyerTemplate9A },
    { MAP_NUM(MAP_ECLAT_ROCKY_PATH), 15, 23, 9, &sFlyerTemplate9A },
    { MAP_NUM(MAP_ECLAT_ROCKY_PATH), 56, 8, 9, &sFlyerTemplate9A },
    { MAP_NUM(MAP_ECLAT_ROCKY_PATH), 52, 25, 9, &sFlyerTemplate9A },
    { MAP_NUM(MAP_ECLAT_ROCKY_PATH), 56, 10, 9, &sFlyerTemplate9A },
    { MAP_NUM(MAP_ECLAT_ROCKY_PATH), 62, 18, 9, &sFlyerTemplate9A },
    { MAP_NUM(MAP_ECLAT_ROCKY_PATH), 62, 16, 9, &sFlyerTemplate9A },
    { MAP_NUM(MAP_ECLAT_ROCKY_PATH), 57, 9, 9, &sFlyerTemplate9A },
    { MAP_NUM(MAP_ECLAT_ROCKY_PATH), 63, 17, 9, &sFlyerTemplate9A },
    { MAP_NUM(MAP_ECLAT_ROCKY_PATH), 52, 27, 9, &sFlyerTemplate9A },
    { MAP_NUM(MAP_ECLAT_ROCKY_PATH), 53, 26, 9, &sFlyerTemplate9A },
    { MAP_NUM(MAP_ECLAT_ROCKY_PATH), 13, 30, 9, &sFlyerTemplate9A },
    { MAP_NUM(MAP_ECLAT_ROCKY_PATH), 13, 28, 9, &sFlyerTemplate9A },
    { MAP_NUM(MAP_ECLAT_ROCKY_PATH), 14, 29, 9, &sFlyerTemplate9A },
};

static void SpriteCB_Route2Flyer(struct Sprite *sprite)
{
    u32 route = sprite->data[0];
    u8 currentMap = gSaveBlock1Ptr->location.mapNum;
    u8 routeMap = sRoutes[route].map;

    if (routeMap != currentMap && !(route < 9 && (currentMap == MAP_NUM(MAP_ROUTE102) || currentMap == MAP_NUM(MAP_ECLAT_ROUTE2))))
    {
        DestroySprite(sprite);
        return;
    }

    s16 distance = sRoutes[route].range * 16;
    if (sprite->data[1])
    {
        sprite->data[2]--;
        if (sprite->data[2] <= 0)
        {
            sprite->data[2] = 0;
            sprite->data[1] = 0;
            StartSpriteAnim(sprite, 0);
        }
    }
    else
    {
        sprite->data[2]++;
        if (sprite->data[2] >= distance)
        {
            sprite->data[2] = distance;
            sprite->data[1] = 1;
            StartSpriteAnim(sprite, 1);
        }
    }
    GetMapCoordsFromSpritePos(sRoutes[route].x + MAP_OFFSET - sRoutes[route].range,
                              sRoutes[route].y + MAP_OFFSET, &sprite->x, &sprite->y);
    sprite->x += 8 + sprite->data[2];
    s16 x = sprite->x + gSpriteCoordOffsetX;
    s16 y = sprite->y + gSpriteCoordOffsetY;
    sprite->invisible = x < -16 || x > DISPLAY_WIDTH + 16 || y < -16 || y > DISPLAY_HEIGHT + 16;
}

static void EnsureFlyerSheetLoaded(u16 tileTag)
{
    if (GetSpriteTileStartByTag(tileTag) == 0xFFFF)
    {
        if (tileTag == GFXTAG_ECLAT_FLYER_65)
            LoadSpriteSheet(&sFlyerSheet65);
        else if (tileTag == GFXTAG_ECLAT_FLYER_98)
            LoadSpriteSheet(&sFlyerSheet98);
        else if (tileTag == GFXTAG_ECLAT_FLYER_9A)
            LoadSpriteSheet(&sFlyerSheet9A);
    }
}

static void StartFlyers(u8 map)
{
    for (u32 route = 0; route < ARRAY_COUNT(sRoutes); route++)
    {
        u8 routeMap = sRoutes[route].map;
        if (routeMap != map && !(route < 9 && (map == MAP_NUM(MAP_ROUTE102) || map == MAP_NUM(MAP_ECLAT_ROUTE2))))
            continue;
        bool32 exists = FALSE;
        for (u32 i = 0; i < MAX_SPRITES; i++)
            if (gSprites[i].inUse && gSprites[i].callback == SpriteCB_Route2Flyer && gSprites[i].data[0] == route)
                exists = TRUE;
        if (exists)
            continue;

        EnsureFlyerSheetLoaded(sRoutes[route].template->tileTag);
        if (LoadObjectEventPalette(sRoutes[route].template->paletteTag) == 0xFF)
            continue;
        u8 id = CreateSprite(sRoutes[route].template, 0, 0, 0);
        if (id >= MAX_SPRITES)
            break;
        struct Sprite *sprite = &gSprites[id];
        sprite->invisible = TRUE;
        sprite->coordOffsetEnabled = TRUE;
        sprite->data[0] = route;
        sprite->data[1] = 1;
        sprite->data[2] = sRoutes[route].range * 16;
        StartSpriteAnim(sprite, 1);
    }
}

void StartEclatRoute2Flybys(void)
{
    StartFlyers(gSaveBlock1Ptr->location.mapNum);
}
void StartEclatRockyFlybys(void) { StartFlyers(MAP_NUM(MAP_ECLAT_ROCKY_PATH)); }
