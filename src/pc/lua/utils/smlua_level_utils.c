#include "sm64.h"
#include "types.h"
#include "smlua_level_utils.h"
#include "pc/lua/smlua.h"

struct CustomLevelInfo* sCustomLevelHead = NULL;
static s16 sCustomLevelNumNext = CUSTOM_LEVEL_NUM_START;

void smlua_level_util_reset(void) {
    struct CustomLevelInfo* node = sCustomLevelHead;

    while (node != NULL) {
        struct CustomLevelInfo* next = node->next;
        if (node->scriptEntryName) {
            free(node->scriptEntryName);
            node->scriptEntryName = NULL;
        }
        if (node->fullName) {
            free(node->fullName);
            node->fullName = NULL;
        }
        if (node->shortName) {
            free(node->shortName);
            node->shortName = NULL;
        }
        free(node);
        node = next;
    }

    sCustomLevelHead = NULL;
    sCustomLevelNumNext = CUSTOM_LEVEL_NUM_START;
}

struct CustomLevelInfo* smlua_level_util_get_info(s16 levelNum) {
    struct CustomLevelInfo* node = sCustomLevelHead;
    while (node != NULL) {
        if (node->levelNum == levelNum) {
            return node;
        }
        node = node->next;
    }
    return NULL;
}

struct CustomLevelInfo* smlua_level_util_get_info_from_short_name(char* shortName) {
    struct CustomLevelInfo* node = sCustomLevelHead;
    while (node != NULL) {
        if (!strcmp(node->shortName, shortName)) {
            return node;
        }
        node = node->next;
    }
    return NULL;
}

static struct CustomLevelInfo* smlua_level_util_get_info_from_script(char* scriptEntryName) {
    struct CustomLevelInfo* node = sCustomLevelHead;
    while (node != NULL) {
        if (!strcmp(node->scriptEntryName, scriptEntryName)) {
            return node;
        }
        node = node->next;
    }
    return NULL;
}

s16 level_register(const char* scriptEntryName, s16 courseNum, const char* fullName, const char* shortName, u32 acousticReach, u32 echoLevel1, u32 echoLevel2, u32 echoLevel3) {
    // validate params
    if (scriptEntryName == NULL) {
        LOG_LUA("Provided nil scriptEntryName");
        return 0;
    }

    if (fullName == NULL) {
        LOG_LUA("Provided nil fullName");
        return 0;
    }

    if (shortName == NULL) {
        LOG_LUA("Provided nil shortName");
        return 0;
    }

    // find duplicate
    struct CustomLevelInfo* info = smlua_level_util_get_info_from_script((char*)scriptEntryName);
    if (info != NULL) {
        return info->levelNum;
    }

    // find script
    LevelScript* script = dynos_get_level_script((char*)scriptEntryName);
    if (script == NULL) {
        LOG_LUA("Failed to find script: %s", scriptEntryName);
        return 0;
    }

    // allocate and fill
    info = calloc(1, sizeof(struct CustomLevelInfo));
    info->script = script;
    info->scriptEntryName = strdup(scriptEntryName);
    info->courseNum = courseNum;
    info->levelNum = sCustomLevelNumNext++;
    info->fullName = strdup(fullName);
    info->shortName = strdup(shortName);
    info->acousticReach = acousticReach;
    info->echoLevel1 = echoLevel1;
    info->echoLevel2 = echoLevel2;
    info->echoLevel3 = echoLevel3;

    // add to list
    if (!sCustomLevelHead) {
        sCustomLevelHead = info;
        return info->levelNum;
    }

    struct CustomLevelInfo* node = sCustomLevelHead;
    while (node) {
        if (!node->next) {
            node->next = info;
            return info->levelNum;
        }
        node = node->next;
    }

    // just in case, should never trigger
    return 0;
}

bool warp_to_warpnode(s32 aLevel, s32 aArea, s32 aAct, s32 aWarpId) {
    return dynos_warp_to_warpnode(aLevel, aArea, aAct, aWarpId);
}

bool warp_to_level(s32 aLevel, s32 aArea, s32 aAct) {
    return dynos_warp_to_level(aLevel, aArea, aAct);
}

bool warp_to_start_level(void) {
    return dynos_warp_to_start_level();
}

bool warp_restart_level(void) {
    return dynos_warp_restart_level();
}

bool warp_exit_level(s32 aDelay) {
    return dynos_warp_exit_level(aDelay);
}

bool warp_to_castle(s32 aLevel) {
    return dynos_warp_to_castle(aLevel);
}
