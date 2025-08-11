// spawn_default_star.c.inc

static struct ObjectHitbox sCollectStarHitbox = {
    /* interactType:      */ INTERACT_STAR_OR_KEY,
    /* downOffset:        */ 0,
    /* damageOrCoinValue: */ 0,
    /* health:            */ 0,
    /* numLootCoins:      */ 0,
    /* radius:            */ 80,
    /* height:            */ 50,
    /* hurtboxRadius:     */ 0,
    /* hurtboxHeight:     */ 0,
};

void bhv_collect_star_init(void) {
    s8 starId;
    u8 currentLevelStarFlags;

    starId = (o->oBehParams >> 24) & 0xFF;
    currentLevelStarFlags = save_file_get_star_flags(gCurrSaveFileNum - 1, gCurrCourseNum - 1);
    if (currentLevelStarFlags & (1 << starId)) {
        cur_obj_set_model(MODEL_TRANSPARENT_STAR);
    } else {
        cur_obj_set_model(MODEL_STAR);
    }

    obj_set_hitbox(o, &sCollectStarHitbox);
}

void bhv_collect_star_loop(void) {
    o->oFaceAngleYaw += 0x800;

    if (o->oInteractStatus & INT_STATUS_INTERACTED) {
        mark_obj_for_deletion(o);
        o->oInteractStatus = 0;
    }
}

void bhv_star_spawn_init(void) {
    o->oMoveAngleYaw = atan2s(o->oHomeZ - o->oPosZ, o->oHomeX - o->oPosX);
    o->oStarSpawnDisFromHome = sqrtf(sqr(o->oHomeX - o->oPosX) + sqr(o->oHomeZ - o->oPosZ));
    o->oVelY = (o->oHomeY - o->oPosY) / 30.0f;
    o->oForwardVel = o->oStarSpawnDisFromHome / 30.0f;
    o->oStarSpawnUnkFC = o->oPosY;

    if (o->oStarSpawnExtCutsceneFlags && ((gMarioStates[0].action & ACT_GROUP_MASK) != ACT_GROUP_CUTSCENE)) {
        if (o->oBehParams2ndByte == 0 || gCurrCourseNum == COURSE_BBH)
            cutscene_object(CUTSCENE_STAR_SPAWN, o);
        else
            cutscene_object(CUTSCENE_RED_COIN_STAR_SPAWN, o);

        gMarioStates[0].freeze = 60;
        set_time_stop_flags(TIME_STOP_ENABLED | TIME_STOP_MARIO_AND_DOORS);
        o->activeFlags |= ACTIVE_FLAG_INITIATED_TIME_STOP;
    }
    cur_obj_become_intangible();
}

void bhv_star_spawn_loop(void) {
    switch (o->oAction) {
        case 0:
            o->oFaceAngleYaw += 0x1000;
            if (o->oTimer > 20)
                o->oAction = 1;
            break;

        case 1:
            obj_move_xyz_using_fvel_and_yaw(o);
            o->oStarSpawnUnkFC += o->oVelY;
            o->oPosY = o->oStarSpawnUnkFC + sins((o->oTimer * 0x8000) / 30) * 400.0f;
            o->oFaceAngleYaw += 0x1000;
            spawn_object(o, MODEL_NONE, bhvSparkleSpawn);
            cur_obj_play_sound_1(SOUND_ENV_STAR);
            if (o->oTimer == 30) {
                o->oAction = 2;
                o->oForwardVel = 0;
                play_power_star_jingle(TRUE);
            }
            break;

        case 2:
            if (o->oTimer < 20)
                o->oVelY = 20 - o->oTimer;
            else
                o->oVelY = -10.0f;

            spawn_object(o, MODEL_NONE, bhvSparkleSpawn);
            obj_move_xyz_using_fvel_and_yaw(o);
            o->oFaceAngleYaw = o->oFaceAngleYaw - o->oTimer * 0x10 + 0x1000;
            cur_obj_play_sound_1(SOUND_ENV_STAR);

            if (o->oPosY < o->oHomeY) {
                cur_obj_play_sound_2(SOUND_GENERAL_STAR_APPEARS);
                cur_obj_become_tangible();
                o->oPosY = o->oHomeY;
                o->oAction = 3;
            }
            break;

        case 3:
            o->oFaceAngleYaw += 0x800;
            if (o->oTimer == 20) {
                gMarioStates[0].freeze = 0;
                gObjCutsceneDone = TRUE;
                clear_time_stop_flags(TIME_STOP_ENABLED | TIME_STOP_MARIO_AND_DOORS);
                o->activeFlags &= ~ACTIVE_FLAG_INITIATED_TIME_STOP;
            }

            if (o->oInteractStatus & INT_STATUS_INTERACTED) {
                mark_obj_for_deletion(o);
                o->oInteractStatus = 0;
            }
            break;
    }
}

struct Object *spawn_star(struct Object *obj, f32 x, f32 y, f32 z) {
    obj = spawn_object_abs_with_rot(o, 0, MODEL_STAR, bhvStarSpawnCoordinates, o->oPosX, o->oPosY,
                                     o->oPosZ, 0, 0, 0);
    if (obj == NULL) { return NULL; }
    obj->oBehParams = o->oBehParams;
    obj->oStarSpawnExtCutsceneFlags = 1;
    obj->oHomeX = x;
    obj->oHomeY = y;
    obj->oHomeZ = z;
    obj->oFaceAnglePitch = 0;
    obj->oFaceAngleRoll = 0;
    return obj;
}

static u8 spawn_star_deduplication(u32* array, u8* count, u32 behParams) {
    for (s32 i = 0; i < *count; i++) {
        if (array[i] == behParams) { return TRUE; }
    }
    if (*count < 8) {
        array[*count] = behParams;
        *count = *count + 1;
        return FALSE;
    }
    return TRUE;
}

struct Object *spawn_default_star(f32 x, f32 y, f32 z) {
    if (sCurrPlayMode != PLAY_MODE_NORMAL && sCurrPlayMode != PLAY_MODE_PAUSED) { return NULL; }
    if (o == NULL) { return NULL; }
    u32 behParams = o->oBehParams;

    // de-duplication checking
    if (spawn_star_deduplication(gSpawnedStarDefault, &gSpawnedStarDefaultCount, behParams)) {
        return NULL;
    }

    struct Object *star = NULL;
    star = spawn_star(star, x, y, z);
    if (star != NULL) {
        star->oBehParams2ndByte = 0;
        network_send_spawn_star(star, 0, x, y, z, behParams, UNKNOWN_GLOBAL_INDEX);
    }
    return star;
}

struct Object *spawn_red_coin_cutscene_star(f32 x, f32 y, f32 z) {
    u32 behParams = o->oBehParams;

    // de-duplication checking
    if (spawn_star_deduplication(gSpawnedStarRed, &gSpawnedStarRedCount, behParams)) {
        return NULL;
    }

    struct Object * star = NULL;
    star = spawn_star(star, x, y, z);
    if (star != NULL) {
        star->oBehParams2ndByte = 1;
        network_send_spawn_star(star, 1, x, y, z, behParams, UNKNOWN_GLOBAL_INDEX);
    }
    return star;
}

struct Object *spawn_no_exit_star(f32 x, f32 y, f32 z) {
    u32 behParams = o->oBehParams;

    // de-duplication checking
    if (spawn_star_deduplication(gSpawnedStarHidden, &gSpawnedStarHiddenCount, behParams)) {
        return NULL;
    }

    struct Object * star = NULL;
    star = spawn_star(star, x, y, z);
    if (star != NULL) {
        star->oBehParams2ndByte = 1;
        star->oInteractionSubtype |= INT_SUBTYPE_NO_EXIT;
        network_send_spawn_star(star, 2, x, y, z, behParams, UNKNOWN_GLOBAL_INDEX);
    }
    return star;
}

/**
 * A special star spawning routine just for a networked stars.
 * These stars require the global index for a network player for proper
 * cutscene functionality. 
 */
struct Object *spawn_networked_default_star(f32 x, f32 y, f32 z, u8 networkPlayerIndex) {
    if (sCurrPlayMode != PLAY_MODE_NORMAL && sCurrPlayMode != PLAY_MODE_PAUSED) { return NULL; }
    if (o == NULL) { return NULL; }
    u32 behParams = o->oBehParams;

    // de-duplication checking
    if (spawn_star_deduplication(gSpawnedStarDefault, &gSpawnedStarDefaultCount, behParams)) {
        return NULL;
    }

    struct Object *star = NULL;
    star = spawn_star(star, x, y, z);
    if (star != NULL) {
        star->oBehParams2ndByte = 0;
        //printf("spawn_networked_default_star: Network Player Index is %i, Our Global Index is %i.\n", networkPlayerIndex, gNetworkPlayers[0].globalIndex);
        if (networkPlayerIndex == gNetworkPlayers[0].globalIndex) {
            star->oStarSpawnExtCutsceneFlags = 1;
        } else {
            star->oStarSpawnExtCutsceneFlags = 0;
        }
        network_send_spawn_star(star, 0, x, y, z, behParams, networkPlayerIndex);
    }
    return star;
}

void bhv_hidden_red_coin_star_init(void) {
    if (gCurrCourseNum != COURSE_JRB) {
        spawn_object(o, MODEL_TRANSPARENT_STAR, bhvRedCoinStarMarker);
    }

    s16 redCoins = count_objects_with_behavior(bhvRedCoin);
    if (redCoins == 0) {
        struct Object *star = spawn_object_abs_with_rot(o, 0, MODEL_STAR, bhvStar, o->oPosX, o->oPosY, o->oPosZ, 0, 0, 0);
        if (star != NULL) { star->oBehParams = o->oBehParams; }
        o->activeFlags = ACTIVE_FLAG_DEACTIVATED;
    }

    o->oHiddenStarTriggerCounter = gCurrentArea->numRedCoins - redCoins;
    
    // We haven't interacted with a player yet.
    // We also don't sync this as not only is it not required
    // but it also is only set for an interaction.
    // Therefore this object must already be loaded for it to be set
    // and if it wasn't. You couldn't of possibly been the one
    // who last interacted to begin with.
    o->oHiddenStarLastInteractedObject = NULL;
    
    if (!sync_object_is_initialized(o->oSyncID)) {
        struct SyncObject *so = sync_object_init(o, SYNC_DISTANCE_ONLY_EVENTS);
        if (so) {
            sync_object_init_field(o, &o->oAction);
            sync_object_init_field(o, &o->oHiddenStarTriggerCounter);
            sync_object_init_field(o, &o->oPosX);
            sync_object_init_field(o, &o->oPosY);
            sync_object_init_field(o, &o->oPosZ);
        }
    }
}

void bhv_hidden_red_coin_star_loop(void) {
    switch (o->oAction) {
        case 0:
            if (o->oHiddenStarTriggerCounter >= gCurrentArea->numRedCoins) {
                o->oAction = 1;
            }
            break;

        case 1:
            if (o->oTimer == 3) {
                struct Object *obj = spawn_red_coin_cutscene_star(o->oPosX, o->oPosY, o->oPosZ);
                if (obj != NULL) {
                    if (o->oHiddenStarLastInteractedObject == &gMarioStates[0]) {
                        obj->oStarSpawnExtCutsceneFlags = 1;
                    } else {
                        obj->oStarSpawnExtCutsceneFlags = 0;
                    }
                    spawn_mist_particles();
                }
                network_send_object(o);
            }
            break;
    }
}
