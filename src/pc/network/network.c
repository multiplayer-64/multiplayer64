#include "socket/socket.h"
#include <stdio.h>
#include "network.h"
#include "object_fields.h"
#include "object_constants.h"
#include "behavior_table.h"
#include "src/game/hardcoded.h"
#include "src/game/scroll_targets.h"
#ifdef DISCORD_SDK
#include "discord/discord.h"
#endif
#include "pc/configfile.h"
#include "pc/cheats.h"
#include "pc/djui/djui.h"
#include "pc/djui/djui_hud_utils.h"
#include "pc/utils/misc.h"
#include "pc/lua/smlua.h"
#include "pc/lua/utils/smlua_model_utils.h"
#include "pc/mods/mods.h"
#include "pc/crash_handler.h"
#include "pc/debuglog.h"
#include "game/camera.h"

// fix warnings when including rendering_graph_node
#undef near
#undef far
#include "src/game/rendering_graph_node.h"

// Mario 64 specific externs
extern s16 sCurrPlayMode;
extern s16 gCurrCourseNum, gCurrActStarNum, gCurrLevelNum, gCurrAreaIndex;

enum NetworkType gNetworkType = NT_NONE;
#ifdef DISCORD_SDK
struct NetworkSystem* gNetworkSystem = &gNetworkSystemDiscord;
#else
struct NetworkSystem* gNetworkSystem = &gNetworkSystemSocket;
#endif

#define LOADING_LEVEL_THRESHOLD 10
#define MAX_PACKETS_PER_SECOND_PER_PLAYER ((u16)70)

u16 networkLoadingLevel = 0;
bool gNetworkAreaLoaded = false;
bool gNetworkAreaSyncing = true;
u32 gNetworkAreaTimerClock = 0;
u32 gNetworkAreaTimer = 0;
void* gNetworkServerAddr = NULL;
bool gNetworkSentJoin = false;
u16 gNetworkRequestLocationTimer = 0;

u8 gDebugPacketIdBuffer[256] = { 0xFF };
u8 gDebugPacketSentBuffer[256] = { 0 };
u8 gDebugPacketOnBuffer = 0;

struct StringLinkedList gRegisteredMods = { 0 };

struct ServerSettings gServerSettings = {
    .playerInteractions = PLAYER_INTERACTIONS_SOLID,
    .playerKnockbackStrength = 25,
    .skipIntro = 0,
    .shareLives = 0,
    .enableCheats = 0,
    .bubbleDeath = 1,
    .headlessServer = 0,
};

void network_set_system(enum NetworkSystemType nsType) {
    network_forget_all_reliable();
    switch (nsType) {
        case NS_SOCKET:  gNetworkSystem = &gNetworkSystemSocket; break;
#ifdef DISCORD_SDK
        case NS_DISCORD: gNetworkSystem = &gNetworkSystemDiscord; break;
#endif
        default: LOG_ERROR("Unknown network system: %d", nsType);
    }
}

bool network_init(enum NetworkType inNetworkType) {
    // reset override hide hud
    extern u8 gOverrideHideHud;
    gOverrideHideHud = 0;

    // sanity check network system
    if (gNetworkSystem == NULL) {
        LOG_ERROR("no network system attached");
        return false;
    }

    network_forget_all_reliable();
    crash_handler_init();

    // set server settings
    gServerSettings.playerInteractions = configPlayerInteraction;
    gServerSettings.playerKnockbackStrength = configPlayerKnockbackStrength;
    gServerSettings.stayInLevelAfterStar = configStayInLevelAfterStar;
    gServerSettings.skipIntro = configSkipIntro;
    gServerSettings.shareLives = configShareLives;
    gServerSettings.enableCheats = configEnableCheats;
    gServerSettings.bubbleDeath = configBubbleDeath;
#if defined(RAPI_DUMMY) || defined(WAPI_DUMMY)
    gServerSettings.headlessServer = (inNetworkType == NT_SERVER);
#else
    gServerSettings.headlessServer = 0;
#endif
    Cheats.enabled = gServerSettings.enableCheats;

    // initialize the network system
    gNetworkSentJoin = false;
    int rc = gNetworkSystem->initialize(inNetworkType);
    if (!rc) {
        LOG_ERROR("failed to initialize network system");
        return false;
    }
    if (gNetworkServerAddr != NULL) {
        free(gNetworkServerAddr);
        gNetworkServerAddr = NULL;
    }

    // set network type
    gNetworkType = inNetworkType;

    if (gNetworkType == NT_SERVER) {
        extern s16 gCurrSaveFileNum;
        gCurrSaveFileNum = configHostSaveSlot;

        mods_activate(&gLocalMods);
        smlua_init();

        dynos_behavior_hook_all_custom_behaviors();

        network_player_connected(NPT_LOCAL, 0, configPlayerModel, &configPlayerPalette, configPlayerName);
        extern u8* gOverrideEeprom;
        gOverrideEeprom = NULL;

        if (gCurrLevelNum != (s16)gLevelValues.entryLevel) {
            extern s16 gChangeLevelTransition;
            gChangeLevelTransition = gLevelValues.entryLevel;
        }

        djui_chat_box_create();
    }

    configfile_save(configfile_name());

    LOG_INFO("initialized");

    return true;
}

void network_on_init_area(void) {
    // reset loading timer
    networkLoadingLevel = 0;
    gNetworkAreaLoaded = false;
    gNetworkAreaSyncing = true;
    gNetworkAreaTimer = 0;
    gNetworkAreaTimerClock = clock_elapsed_ticks();
}

void network_on_loaded_area(void) {
    area_remove_sync_ids_clear();
    struct NetworkPlayer* np = gNetworkPlayerLocal;
    if (np != NULL) {
        bool levelMatch = (np->currCourseNum == gCurrCourseNum
                           && np->currActNum == gCurrActStarNum
                           && np->currLevelNum == gCurrLevelNum);
        if (np->currLevelSyncValid && levelMatch && np->currAreaIndex != gCurrAreaIndex) {
            network_send_change_area();
        } else {
            network_send_change_level();
        }
    }
}

static void network_remember_debug_packet(u8 id, bool sent) {
    if (id == PACKET_ACK) { return; }
    if (id == PACKET_KEEP_ALIVE) { return; }
    if (id == PACKET_DEBUG_SYNC) { return; }
    if (id == PACKET_PLAYER && id == gDebugPacketIdBuffer[gDebugPacketOnBuffer]) { return; }
    if (id == PACKET_OBJECT && id == gDebugPacketIdBuffer[gDebugPacketOnBuffer]) { return; }
    gDebugPacketOnBuffer++;
    gDebugPacketIdBuffer[gDebugPacketOnBuffer] = id;
    gDebugPacketSentBuffer[gDebugPacketOnBuffer] = sent;
}

bool network_allow_unknown_local_index(enum PacketType packetType) {
    return (packetType == PACKET_JOIN_REQUEST)
        || (packetType == PACKET_KICK)
        || (packetType == PACKET_ACK)
        || (packetType == PACKET_MOD_LIST_REQUEST)
        || (packetType == PACKET_MOD_LIST)
        || (packetType == PACKET_MOD_LIST_ENTRY)
        || (packetType == PACKET_MOD_LIST_FILE)
        || (packetType == PACKET_MOD_LIST_DONE)
        || (packetType == PACKET_DOWNLOAD_REQUEST)
        || (packetType == PACKET_DOWNLOAD)
        || (packetType == PACKET_KEEP_ALIVE)
        || (packetType == PACKET_DEBUG_SYNC);
}

void network_send_to(u8 localIndex, struct Packet* p) {
    if (p == NULL) {
        LOG_ERROR("no data to send");
        return;
    }
    // sanity checks
    if (gNetworkType == NT_NONE) { LOG_ERROR("network type error none!"); return; }
    if (p->error) { LOG_ERROR("packet error!"); return; }
    if (gNetworkSystem == NULL) { LOG_ERROR("no network system attached"); return; }
    if (localIndex == 0 && !network_allow_unknown_local_index(p->buffer[0])) {
        LOG_ERROR("\n####################\nsending to myself, packetType: %d\n####################\n", p->packetType);
        // SOFT_ASSERT(false); - Crash?
        return;
    }

    if (gNetworkType == NT_SERVER) {
        struct NetworkPlayer* np = &gNetworkPlayers[localIndex];
        // don't send a packet to a player that can't receive it
        if (p->levelAreaMustMatch) {
            if (p->courseNum != np->currCourseNum) { return; }
            if (p->actNum    != np->currActNum)    { return; }
            if (p->levelNum  != np->currLevelNum)  { return; }
            if (p->areaIndex != np->currAreaIndex) { return; }
        } else if (p->levelMustMatch) {
            if (p->courseNum != np->currCourseNum) { return; }
            if (p->actNum    != np->currActNum)    { return; }
            if (p->levelNum  != np->currLevelNum)  { return; }
        }
    }

    // set the flags again
    packet_set_flags(p);

    // set destination
    packet_set_destination(p, p->requestBroadcast
                              ? PACKET_DESTINATION_BROADCAST
                              : gNetworkPlayers[(localIndex == 0) ? p->localIndex : localIndex].globalIndex);

    p->localIndex = localIndex;

    // set ordered data (MUST BE IMMEDITAELY BEFORE network_remember_reliable())
    if (p->orderedGroupId != 0 && !p->sent) {
        packet_set_ordered_data(p);
    }

    // remember reliable packets
    network_remember_reliable(p);

    // save inside packet buffer
    u32 hash = packet_hash(p);
    memcpy(&p->buffer[p->dataLength], &hash, sizeof(u32));

    // redirect to server if required
    if (localIndex != 0 && gNetworkType != NT_SERVER && gNetworkSystem->requireServerBroadcast && gNetworkPlayerServer != NULL) {
        localIndex = gNetworkPlayerServer->localIndex;
    }

    SOFT_ASSERT(p->dataLength < PACKET_LENGTH);

    // rate limit packets
    bool tooManyPackets = false;
    s32 maxPacketsPerSecond = (gNetworkType == NT_SERVER) ? (MAX_PACKETS_PER_SECOND_PER_PLAYER * (u16)network_player_connected_count()) : MAX_PACKETS_PER_SECOND_PER_PLAYER;
    static s32 sPacketsPerSecond[MAX_PLAYERS] = { 0 };
    static f32 sPacketsPerSecondTime[MAX_PLAYERS] = { 0 };
    f32 currentTime = clock_elapsed();
    if ((currentTime - sPacketsPerSecondTime[localIndex]) > 0) {
        if (sPacketsPerSecond[localIndex] > maxPacketsPerSecond) {
            LOG_ERROR("Too many packets sent to localIndex %d! Attempted %d. Connected count %d.", localIndex, sPacketsPerSecond[localIndex], network_player_connected_count());
        }
        sPacketsPerSecondTime[localIndex] = currentTime;
        sPacketsPerSecond[localIndex] = 1;
    } else {
        sPacketsPerSecond[localIndex]++;
        if (sPacketsPerSecond[localIndex] > maxPacketsPerSecond) {
            tooManyPackets = true;
        }
    }

    // send
    if (!tooManyPackets) {
        int rc = gNetworkSystem->send(localIndex, p->addr, p->buffer, p->cursor + sizeof(u32));
        if (rc == SOCKET_ERROR) { LOG_ERROR("send error %d", rc); return; }
    }
    p->sent = true;

    network_remember_debug_packet(p->packetType, true);

    gNetworkPlayers[localIndex].lastSent = clock_elapsed();
}

void network_send(struct Packet* p) {
    if (p == NULL) {
        LOG_ERROR("no data to send");
        return;
    }
    // prevent errors during writing from propagating
    if (p->writeError) {
        LOG_ERROR("packet has write error: %u", p->packetType);
        return;
    }

    // set the flags again
    packet_set_flags(p);

    if (gNetworkType != NT_SERVER) {
        p->requestBroadcast = TRUE;
        if (gNetworkSystem != NULL && gNetworkSystem->requireServerBroadcast && gNetworkPlayerServer != NULL) {
            int i = gNetworkPlayerServer->localIndex;
            p->localIndex = i;
            p->sent = false;
            network_send_to(i, p);
            return;
        }
    }

    for (s32 i = 1; i < MAX_PLAYERS; i++) {
        struct NetworkPlayer* np = &gNetworkPlayers[i];
        if (!np->connected) { continue; }

        // don't send a packet to a player that can't receive it
        if (p->levelAreaMustMatch) {
            if (p->courseNum != np->currCourseNum) { continue; }
            if (p->actNum    != np->currActNum)    { continue; }
            if (p->levelNum  != np->currLevelNum)  { continue; }
            if (p->areaIndex != np->currAreaIndex) { continue; }
        } else if (p->levelMustMatch) {
            if (p->courseNum != np->currCourseNum) { continue; }
            if (p->actNum    != np->currActNum)    { continue; }
            if (p->levelNum  != np->currLevelNum)  { continue; }
        }

        p->localIndex = i;
        p->sent = false;
        network_send_to(i, p);
    }
}

void network_receive(u8 localIndex, void* addr, u8* data, u16 dataLength) {
    // receive packet
    struct Packet p = {
        .localIndex = localIndex,
        .cursor = 3,
        .addr = addr,
        .buffer = { 0 },
        .dataLength = dataLength,
    };
    memcpy(p.buffer, data, dataLength);

    if (localIndex != UNKNOWN_LOCAL_INDEX && localIndex != 0) {
        gNetworkPlayers[localIndex].lastReceived = clock_elapsed();
    }

    // subtract and check hash
    p.dataLength -= sizeof(u32);
    if (!packet_check_hash(&p)) {
        LOG_ERROR("invalid packet hash!");
        return;
    }

    network_remember_debug_packet(p.buffer[0], false);

    // execute packet
    packet_receive(&p);
}

void* network_duplicate_address(u8 localIndex) {
    assert(localIndex < MAX_PLAYERS);
    return gNetworkSystem->dup_addr(localIndex);
}

static void network_update_area_timer(void) {
    bool brokenClock = false;
#ifdef DEVELOPMENT
    static u16 skipClockCount = 0;
    static u16 updateClockCount = 1;
    if (updateClockCount > 0) {
        updateClockCount--;
        if (updateClockCount <= 0 || updateClockCount > 120) {
            skipClockCount = rand() % 30;
        }
    }
    else {
        skipClockCount--;
        if (skipClockCount <= 0 || skipClockCount > 60) {
            updateClockCount = rand() % 120;
        }
    }
    //brokenClock = (skipClockCount > 0);
#endif
    if (!brokenClock) {
        // update network area timer
        u32 desiredNAT = gNetworkAreaTimer + 1;
        gNetworkAreaTimer = (clock_elapsed_ticks() - gNetworkAreaTimerClock);
        if (gNetworkAreaTimer < desiredNAT) {
            gNetworkAreaTimer++;
        }
        else if (gNetworkAreaTimer > desiredNAT) {
            gNetworkAreaTimer--;
        }
    }
}

void network_update(void) {
    // check for level loaded event
    if (networkLoadingLevel < LOADING_LEVEL_THRESHOLD) {
        networkLoadingLevel++;
        if (!gNetworkAreaLoaded && networkLoadingLevel >= LOADING_LEVEL_THRESHOLD) {
            gNetworkAreaLoaded = true;
            network_on_loaded_area();
        }
    }

    // update network area timer
    network_update_area_timer();

    // send out update packets
    if (gNetworkType != NT_NONE) {
        network_player_update();
        if (sCurrPlayMode == PLAY_MODE_NORMAL || sCurrPlayMode == PLAY_MODE_PAUSED) {
            network_update_player();
            network_update_objects();
        }
    }

    // receive packets
    if (gNetworkSystem != NULL) {
        gNetworkSystem->update();
    }

    // update reliable and ordered packets
    if (gNetworkType != NT_NONE) {
        network_update_reliable();
        packet_ordered_update();
    }

    // update level/area request timers
    /*struct NetworkPlayer* np = gNetworkPlayerLocal;
    if (np != NULL && !np->currLevelSyncValid) {
        gNetworkRequestLocationTimer++;
        if (gNetworkRequestLocationTimer > 30 * 10) {
            // find a NetworkPlayer around that location
            struct NetworkPlayer *npLevelAreaMatch = get_network_player_from_area(np->currCourseNum, np->currActNum, np->currLevelNum, np->currAreaIndex);
            struct NetworkPlayer *npLevelMatch     = get_network_player_from_level(np->currCourseNum, np->currActNum, np->currLevelNum);
            struct NetworkPlayer *npAny = (npLevelAreaMatch == NULL) ? npLevelMatch : npLevelAreaMatch;

            bool inCredits = (np->currActNum == 99);
            if (gNetworkType == NT_SERVER && (npAny == NULL || inCredits)) {
                // no NetworkPlayer in the level
                network_send_sync_valid(np, np->currCourseNum, np->currActNum, np->currLevelNum, np->currAreaIndex);
                return;
            }

            // matching NetworkPlayer is client
            if (npAny == npLevelAreaMatch) {
                network_send_level_area_request(np, npAny);
            } else {
                network_send_level_request(np, npAny);
            }
        }
    }*/

}

void network_register_mod(char* modName) {
    if (string_linked_list_contains(&gRegisteredMods, modName)) { return; }
    string_linked_list_append(&gRegisteredMods, modName);
}

void network_shutdown(bool sendLeaving, bool exiting, bool popup) {
    if (gDjuiChatBox != NULL) {
        djui_base_destroy(&gDjuiChatBox->base);
        gDjuiChatBox = NULL;
    }

    gNetworkSentJoin = false;

    network_forget_all_reliable();
    if (gNetworkType == NT_NONE) { return; }
    if (gNetworkSystem == NULL) { LOG_ERROR("no network system attached"); return; }

    if (gNetworkPlayerLocal != NULL && sendLeaving) { network_send_leaving(gNetworkPlayerLocal->globalIndex); }
    network_player_shutdown(popup);
    gNetworkSystem->shutdown();

    if (gNetworkServerAddr != NULL) {
        free(gNetworkServerAddr);
        gNetworkServerAddr = NULL;
    }
    gNetworkPlayerServer = NULL;

    gNetworkType = NT_NONE;


#ifdef DISCORD_SDK
    network_set_system(NS_DISCORD);
#endif

    if (exiting) { return; }

    // reset other stuff
    extern u8* gOverrideEeprom;
    gOverrideEeprom = NULL;
    extern u8 gOverrideFreezeCamera;
    gOverrideFreezeCamera = false;
    gDjuiHudLockMouse = false;
    gOverrideNear = 0;
    gOverrideFar = 0;
    gOverrideFOV = 0;
    dynos_mod_shutdown();
    mods_clear(&gActiveMods);
    mods_clear(&gRemoteMods);
    smlua_shutdown();
    extern s16 gChangeLevel;
    gChangeLevel = LEVEL_CASTLE_GROUNDS;
    network_player_init();
    camera_set_use_course_specific_settings(true);
    free_vtx_scroll_targets();

    struct Controller* cnt = gMarioStates[0].controller;
    cnt->rawStickX = 0;
    cnt->rawStickY = 0;
    cnt->stickX = 0;
    cnt->stickY = 0;
    cnt->stickMag = 0;
    cnt->buttonDown = 0;
    cnt->buttonPressed = 0;
    cnt->extStickX = 0;
    cnt->extStickY = 0;

    extern void save_file_load_all(UNUSED u8 reload);
    save_file_load_all(TRUE);
    extern void save_file_set_using_backup_slot(bool usingBackupSlot);
    save_file_set_using_backup_slot(false);

    extern s16 gMenuMode;
    gMenuMode = -1;

    extern bool gIsModerator;
    gIsModerator = false;

    djui_panel_shutdown();
    extern bool gDjuiInMainMenu;
    if (!gDjuiInMainMenu) {
        gDjuiInMainMenu = true;
        djui_panel_main_create(NULL);
    }
}
