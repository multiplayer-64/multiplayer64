#ifndef DYNOS_C_H
#define DYNOS_C_H
#ifndef __cplusplus

#include "dynos.h"
#include "types.h"
#include "engine/behavior_script.h"
#include "src/game/moving_texture.h"

void *dynos_swap_cmd(void *cmd);

// -- built in -- //
void *dynos_update_cmd(void *cmd);
void  dynos_update_gfx();
void  dynos_update_opt(void *pad);
s32   dynos_tex_import(void **output, void *ptr, s32 tile, void *grapi, void **hashmap, void *pool, s32 *poolpos, s32 poolsize);
void  dynos_gfx_swap_animations(void *ptr);

// -- warps -- //
LevelScript* dynos_get_level_script(char* scriptEntryName);
bool dynos_warp_to_warpnode(s32 aLevel, s32 aArea, s32 aAct, s32 aWarpId);
bool dynos_warp_to_level(s32 aLevel, s32 aArea, s32 aAct);
bool dynos_warp_restart_level(void);
bool dynos_warp_to_start_level(void);
bool dynos_warp_exit_level(s32 aDelay);
bool dynos_warp_to_castle(s32 aLevel);

// -- dynos packs -- //
int dynos_pack_get_count(void);
const char* dynos_pack_get_name(s32 index);
bool dynos_pack_get_enabled(s32 index);
void dynos_pack_set_enabled(s32 index, bool value);
void dynos_pack_init(void);
void dynos_generate_packs(const char* directory);

// -- geos -- //
void dynos_actor_override(void** aSharedChild);
void dynos_add_actor_custom(const char *filePath, const char* geoName);
const void* dynos_geolayout_get(const char *name);
void *dynos_geolayout_to_graphnode(const void *geoLayout, bool keepInMemory);

// -- collisions -- //
void dynos_add_collision(const char *filePath, const char* collisionName);
Collision* dynos_collision_get(const char* collisionName);

// -- textures -- //
void dynos_add_texture(const char *filePath, const char* textureName);
bool dynos_texture_get(const char* textureName, struct TextureInfo* outTextureInfo);

// -- movtexqcs -- //
void dynos_movtexqc_register(const char* name, s16 level, s16 area, s16 type);
struct MovtexQuadCollection* dynos_movtexqc_get_from_id(u32 id);
struct MovtexQuadCollection* dynos_movtexqc_get_from_index(s32 index);

// -- levels -- //
void dynos_add_level(s32 modIndex, const char *filePath, const char* levelName);
const char* dynos_level_get_token(u32 index);
Trajectory* dynos_level_get_trajectory(const char* name);
void dynos_level_load_background(void *ptr);

// -- behaviors -- //
void dynos_add_behavior(s32 modIndex, const char *filePath, const char *behaviorName);
s32 dynos_behavior_get_active_mod_index(BehaviorScript *bhvScript);
const char *dynos_behavior_get_token(BehaviorScript *bhvScript, u32 index);
void dynos_behavior_hook_all_custom_behaviors(void);

// -- other -- //
void dynos_mod_shutdown(void);
void dynos_add_scroll_target(u32 index, const char *name, u32 offset, u32 size);

#endif
#endif
