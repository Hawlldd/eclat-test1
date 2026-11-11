#include "global.h"
#include "quest_indicator.h"
#include "event_data.h"
#include "event_object_movement.h"
#include "graphics.h"
#include "sprite.h"
#include "trig.h"
#include "constants/event_objects.h"
#include "constants/flags.h"
#include "constants/maps.h"

struct QuestIndicatorEntry
{
    u8 mapGroup;
    u8 mapNum;
    u8 localId;
    u16 flagId;
};

// Table of NPCs with active Quest / Alert Indicators
// As soon as the corresponding flag is set, the exclamation mark automatically disappears.
static const struct QuestIndicatorEntry sQuestIndicators[] =
{
    {
        .mapGroup = MAP_GROUP(MAP_LITTLEROOT_TOWN),
        .mapNum = MAP_NUM(MAP_LITTLEROOT_TOWN),
        .localId = 14, // Alda (Iviere Town river NPC)
        .flagId = FLAG_GOT_EXP_SHARE_FROM_ALDA,
    },
    {
        .mapGroup = MAP_GROUP(MAP_IVIERE_FILAOS_HOUSE),
        .mapNum = MAP_NUM(MAP_IVIERE_FILAOS_HOUSE),
        .localId = 1, // Mrs. Filao (Gives Town Map)
        .flagId = FLAG_IVIERE_TOWN_MAP_TAKEN,
    },
};

static const u8 sQuestExclamationGfx[] = INCGFX_U8("graphics/field_effects/pics/emotion_exclamation.png", ".4bpp");

static const struct SpriteFrameImage sPicTable_QuestIndicator[] = {
    { sQuestExclamationGfx, sizeof(sQuestExclamationGfx) }
};

static const union AnimCmd sAnim_QuestIndicator[] = {
    ANIMCMD_FRAME(0, 60),
    ANIMCMD_JUMP(0),
};

static const union AnimCmd *const sAnimTable_QuestIndicator[] = {
    sAnim_QuestIndicator,
};

static const struct OamData sOam_QuestIndicator = {
    .y = 0,
    .affineMode = ST_OAM_AFFINE_OFF,
    .objMode = ST_OAM_OBJ_NORMAL,
    .mosaic = FALSE,
    .bpp = ST_OAM_4BPP,
    .shape = SPRITE_SHAPE(16x16),
    .x = 0,
    .matrixNum = 0,
    .size = SPRITE_SIZE(16x16),
    .tileNum = 0,
    .priority = 1,
    .paletteNum = 0,
    .affineParam = 0,
};

static void SpriteCB_QuestIndicator(struct Sprite *sprite);

static const struct SpriteTemplate sSpriteTemplate_QuestIndicator = {
    .tileTag = TAG_NONE,
    .paletteTag = OBJ_EVENT_PAL_TAG_MAY,
    .oam = &sOam_QuestIndicator,
    .anims = sAnimTable_QuestIndicator,
    .images = sPicTable_QuestIndicator,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCB_QuestIndicator,
};

static void SpriteCB_QuestIndicator(struct Sprite *sprite)
{
    u8 objEventId;

    // If the quest flag has been set, destroy indicator immediately
    if (FlagGet(sprite->data[3]))
    {
        DestroySprite(sprite);
        return;
    }

    // If map changed or NPC is no longer on map, destroy indicator
    if (gSaveBlock1Ptr->location.mapNum != sprite->data[1]
     || gSaveBlock1Ptr->location.mapGroup != sprite->data[2]
     || TryGetObjectEventIdByLocalIdAndMap(sprite->data[0], sprite->data[1], sprite->data[2], &objEventId))
    {
        DestroySprite(sprite);
        return;
    }

    struct ObjectEvent *objEvent = &gObjectEvents[objEventId];
    struct Sprite *objSprite = &gSprites[objEvent->spriteId];

    // Follow the NPC smoothly
    sprite->x = objSprite->x;
    sprite->y = objSprite->y - 20;
    sprite->x2 = objSprite->x2;

    // Gentle floating bob animation (sine wave between -2 and +2 pixels)
    sprite->data[4] = (sprite->data[4] + 4) & 0xFF;
    sprite->y2 = objSprite->y2 + (gSineTable[sprite->data[4]] >> 6);

    sprite->invisible = objSprite->invisible;
    sprite->subpriority = 0;
}

void UpdateQuestIndicators(void)
{
    u8 currentMapGroup = gSaveBlock1Ptr->location.mapGroup;
    u8 currentMapNum = gSaveBlock1Ptr->location.mapNum;

    for (u32 i = 0; i < ARRAY_COUNT(sQuestIndicators); i++)
    {
        const struct QuestIndicatorEntry *entry = &sQuestIndicators[i];

        if (entry->mapGroup != currentMapGroup || entry->mapNum != currentMapNum)
            continue;

        // If quest already completed, do nothing
        if (FlagGet(entry->flagId))
            continue;

        // Check if an indicator sprite for this NPC is already active
        bool32 alreadyActive = FALSE;
        for (u32 s = 0; s < MAX_SPRITES; s++)
        {
            if (gSprites[s].inUse && gSprites[s].callback == SpriteCB_QuestIndicator
             && gSprites[s].data[0] == entry->localId
             && gSprites[s].data[1] == entry->mapNum
             && gSprites[s].data[2] == entry->mapGroup)
            {
                alreadyActive = TRUE;
                break;
            }
        }

        if (alreadyActive)
            continue;

        // Check if NPC exists in the active object events
        u8 objEventId;
        if (TryGetObjectEventIdByLocalIdAndMap(entry->localId, entry->mapNum, entry->mapGroup, &objEventId))
            continue;

        // Spawn indicator sprite
        u8 spriteId = CreateSpriteAtEndUnchecked(&sSpriteTemplate_QuestIndicator, 0, 0, 0x50);
        if (spriteId != MAX_SPRITES)
        {
            struct Sprite *sprite = &gSprites[spriteId];
            sprite->coordOffsetEnabled = TRUE;
            sprite->data[0] = entry->localId;
            sprite->data[1] = entry->mapNum;
            sprite->data[2] = entry->mapGroup;
            sprite->data[3] = entry->flagId;
            sprite->data[4] = 0;

            u32 palNum = IndexOfSpritePaletteTag(OBJ_EVENT_PAL_TAG_MAY);
            if (palNum == 0xFF)
                palNum = LoadObjectEventPalette(OBJ_EVENT_PAL_TAG_MAY);
            if (palNum != 0xFF)
                sprite->oam.paletteNum = palNum;

            SpriteCB_QuestIndicator(sprite);
        }
    }
}
