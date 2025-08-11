#ifndef MODS_H
#define MODS_H

#include "PR/ultratypes.h"
#include <types.h>
#include "src/pc/platform.h"
#include "mod.h"

#define MAX_MOD_SIZE (35 * 1048576) // 35MB
#define TMP_DIRECTORY "tmp"

struct Mods {
    struct Mod** entries;
    u16 entryCount;
    size_t size;
};

extern struct Mods gLocalMods;
extern struct Mods gRemoteMods;
extern struct Mods gActiveMods;

extern char gRemoteModsBasePath[];

bool mods_generate_remote_base_path(void);
void mods_activate(struct Mods* mods);
void mods_clear(struct Mods* mods);
void mods_init(void);
void mods_shutdown(void);

#endif