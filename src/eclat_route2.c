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
    ANIMCMD_FRAME(0, 10), ANIMCMD_FRAME(1, 10), ANIMCMD_JUMP(0),
};
static const union AnimCmd sFlyerRight[] = {
    ANIMCMD_FRAME(0, 10, .hFlip = TRUE), ANIMCMD_FRAME(1, 10, .hFlip = TRUE), ANIMCMD_JUMP(0),
};
static const union AnimCmd *const sFlyerAnims[] = {sFlyerRight, sFlyerLeft};
static const u32 sFlyer65[] = INCBIN_U32("graphics/eclat_intro/route2/flyer_65.bin");
static const struct SpriteFrameImage sImages65[] = {{sFlyer65,512}, {sFlyer65+128,512}};
static const u32 sFlyer98[] = INCBIN_U32("graphics/eclat_intro/route2/flyer_98.bin");
static const struct SpriteFrameImage sImages98[] = {{sFlyer98,512}, {sFlyer98+128,512}};
static const u32 sFlyer9A[] = INCBIN_U32("graphics/eclat_intro/route2/flyer_9a.bin");
static const struct SpriteFrameImage sImages9A[] = {{sFlyer9A,512}, {sFlyer9A+128,512}};
static const struct SpriteTemplate sFlyerTemplate65 = {
    .tileTag = TAG_NONE, .paletteTag = OBJ_EVENT_PAL_TAG_PETIPRES_04,
    .oam = &sFlyerOam, .anims = sFlyerAnims, .images = sImages65,
    .affineAnims = gDummySpriteAffineAnimTable, .callback = SpriteCB_Route2Flyer,
};
static const struct SpriteTemplate sFlyerTemplate98 = {
    .tileTag = TAG_NONE, .paletteTag = OBJ_EVENT_PAL_TAG_PETIPRES_03,
    .oam = &sFlyerOam, .anims = sFlyerAnims, .images = sImages98,
    .affineAnims = gDummySpriteAffineAnimTable, .callback = SpriteCB_Route2Flyer,
};
static const struct SpriteTemplate sFlyerTemplate9A = {
    .tileTag = TAG_NONE, .paletteTag = OBJ_EVENT_PAL_TAG_PETIPRES_04,
    .oam = &sFlyerOam, .anims = sFlyerAnims, .images = sImages9A,
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
    s16 distance = sRoutes[route].range * 16;
    sprite->data[2] += sprite->data[1] ? -1 : 1;
    if (sprite->data[2] <= 0 || sprite->data[2] >= distance)
    {
        sprite->data[1] ^= 1;
        StartSpriteAnim(sprite, sprite->data[1]);
    }
    GetMapCoordsFromSpritePos(sRoutes[route].x + MAP_OFFSET - sRoutes[route].range,
                              sRoutes[route].y + MAP_OFFSET, &sprite->x, &sprite->y);
    sprite->x += 8 + sprite->data[2];
    s16 x = sprite->x + gSpriteCoordOffsetX;
    s16 y = sprite->y + gSpriteCoordOffsetY;
    sprite->invisible = x < -16 || x > DISPLAY_WIDTH + 16 || y < -16 || y > DISPLAY_HEIGHT + 16;
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
        if (exists || LoadObjectEventPalette(sRoutes[route].template->paletteTag) == 0xFF)
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
