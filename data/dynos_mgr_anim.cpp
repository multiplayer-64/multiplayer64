#include "dynos.cpp.h"
extern "C" {
#include "object_fields.h"
#include "game/level_update.h"
#include "game/object_list_processor.h"
#include "pc/configfile.h"
}

//
// Update animations
//

// Retrieve the current Mario's animation index
static s32 RetrieveCurrentMarioAnimationIndex(u32 aPlayerIndex) {
    struct MarioAnimDmaRelatedThing *_AnimDmaTable = gMarioStates[aPlayerIndex].animation->animDmaTable;
    for (s32 i = 0; i != (s32) _AnimDmaTable->count; ++i) {
        void *_AnimAddr = _AnimDmaTable->srcAddr + _AnimDmaTable->anim[i].offset;
        if (_AnimAddr == gMarioStates[aPlayerIndex].animation->currentAnimAddr) {
            return i;
        }
    }
    return -1;
}

// Retrieve the current animation index
// As we don't know the length of the table, let's hope that we'll always find the animation...
static s32 RetrieveCurrentAnimationIndex(struct Object *aObject) {
    if (!aObject->oAnimations || !aObject->header.gfx.animInfo.curAnim) {
        return -1;
    }
    for (s32 i = 0; aObject->oAnimations[i] != NULL; ++i) {
        if (aObject->oAnimations[i] == aObject->header.gfx.animInfo.curAnim) {
            return i;
        }
    }
    return -1;
}

// Must be called twice, before and after geo_set_animation_globals
void DynOS_Anim_Swap(void *aPtr) {
    static Animation *pDefaultAnimation = NULL;
    static Animation  sGfxDataAnimation;

    // Does the object has a model?
    struct Object *_Object = (struct Object *) aPtr;
    if (!_Object->header.gfx.sharedChild) {
        return;
    }

    // Swap the current animation with the one from the Gfx data
    if (!pDefaultAnimation) {
        pDefaultAnimation = _Object->header.gfx.animInfo.curAnim;

        // ActorGfx data
        ActorGfx* _ActorGfx = DynOS_Actor_GetActorGfx(_Object->header.gfx.sharedChild);
        if (!_ActorGfx) {
            return;
        }

        // GfxData
        GfxData* _GfxData = _ActorGfx->mGfxData;
        if (!_GfxData) {
            return;
        }

        // Animation table
        if (_GfxData->mAnimationTable.Empty()) {
            return;
        }

        // Animation index
        s32 _AnimIndex = -1;
        for (u32 i = 0; i < MAX_PLAYERS; i++) {
            if (gMarioStates[i].marioObj == NULL) { continue; }
            if (_Object == gMarioStates[i].marioObj) {
                _AnimIndex = RetrieveCurrentMarioAnimationIndex(i);
            }
        }
        if (_AnimIndex == -1) {
            _AnimIndex = RetrieveCurrentAnimationIndex(_Object);
        }
        if (_AnimIndex == -1) {
            return;
        }

        // Animation data
        const AnimData *_AnimData = (const AnimData *) _GfxData->mAnimationTable[_AnimIndex].second;
        if (_AnimData) {
            sGfxDataAnimation.flags = _AnimData->mFlags;
            sGfxDataAnimation.animYTransDivisor = _AnimData->mUnk02;
            sGfxDataAnimation.startFrame = _AnimData->mUnk04;
            sGfxDataAnimation.loopStart = _AnimData->mUnk06;
            sGfxDataAnimation.loopEnd = _AnimData->mUnk08;
            sGfxDataAnimation.unusedBoneCount = _AnimData->mUnk0A.second;
            sGfxDataAnimation.values = _AnimData->mValues.second.begin();
            sGfxDataAnimation.index = _AnimData->mIndex.second.begin();
            sGfxDataAnimation.length = _AnimData->mLength;
            _Object->header.gfx.animInfo.curAnim = &sGfxDataAnimation;
        }

    // Restore the default animation
    } else {
        _Object->header.gfx.animInfo.curAnim = pDefaultAnimation;
        pDefaultAnimation = NULL;
    }
}
