#include "sm64.h"
#include "types.h"
#include "characters.h"
#include "hud.h"
#include "model_ids.h"
#include "object_constants.h"
#include "sounds.h"
#include "luigi_sounds.h"
#include "wario_sounds.h"
#include "pc/configfile.h"
#include "audio/external.h"
#include "engine/graph_node.h"
#include "characters_bass_sounds.h"
#include "pc/lua/smlua.h"

extern Gfx mario_cap_seg3_dl_03022F48[];
extern Gfx luigi_cap_seg3_dl_03022F48[];
extern Gfx toad_player_dl_cap[];
extern Gfx toad_player_dl_cap_decal[];
extern Gfx waluigi_cap_seg3_dl_03022F48[];
extern Gfx wario_cap_seg3_dl_03022F48[];

extern ALIGNED8 const u8 texture_hud_char_mario_head[];
extern ALIGNED8 const u8 texture_hud_char_luigi_head[];
extern ALIGNED8 const u8 texture_hud_char_toad_head[];
extern ALIGNED8 const u8 texture_hud_char_waluigi_head[];
extern ALIGNED8 const u8 texture_hud_char_wario_head[];

struct Character gCharacters[CT_MAX] = {
    [CT_MARIO] = {
        .type                  = CT_MARIO,
        .name                  = "Mario",
        .hudHead               = ',',
        .hudHeadTexture        = { .texture = (u8*)texture_hud_char_mario_head, .bitSize = 8, .width = 16, .height = 16 },
        .cameraHudHead         = GLYPH_CAM_MARIO_HEAD,
        .modelId               = MODEL_MARIO,
        .capModelId            = MODEL_MARIOS_CAP,
        .capMetalModelId       = MODEL_MARIOS_METAL_CAP,
        .capWingModelId        = MODEL_MARIOS_WING_CAP,
        .capMetalWingModelId   = MODEL_MARIOS_WINGED_METAL_CAP,
        .capEnemyLayer         = LAYER_OPAQUE,
        .capEnemyGfx           = mario_cap_seg3_dl_03022F48,
        .capEnemyDecalGfx      = NULL,
        .torsoRotMult          = 1.0f,
        // anim
        .animOffsetEnabled     = false,
        // sounds
        .soundFreqScale        = 1.0f,
        .soundYahWahHoo        = SOUND_MARIO_YAH_WAH_HOO,
        .soundHoohoo           = SOUND_MARIO_HOOHOO,
        .soundYahoo            = SOUND_MARIO_YAHOO,
        .soundUh               = SOUND_MARIO_UH,
        .soundHrmm             = SOUND_MARIO_HRMM,
        .soundWah2             = SOUND_MARIO_WAH2,
        .soundWhoa             = SOUND_MARIO_WHOA,
        .soundEeuh             = SOUND_MARIO_EEUH,
        .soundAttacked         = SOUND_MARIO_ATTACKED,
        .soundOoof             = SOUND_MARIO_OOOF,
        .soundOoof2            = SOUND_MARIO_OOOF2,
        .soundHereWeGo         = SOUND_MARIO_HERE_WE_GO,
        .soundYawning          = SOUND_MARIO_YAWNING,
        .soundSnoring1         = SOUND_MARIO_SNORING1,
        .soundSnoring2         = SOUND_MARIO_SNORING2,
        .soundWaaaooow         = SOUND_MARIO_WAAAOOOW,
        .soundHaha             = SOUND_MARIO_HAHA,
        .soundHaha_2           = SOUND_MARIO_HAHA_2,
        .soundUh2              = SOUND_MARIO_UH2,
        .soundUh2_2            = SOUND_MARIO_UH2_2,
        .soundOnFire           = SOUND_MARIO_ON_FIRE,
        .soundDying            = SOUND_MARIO_DYING,
        .soundPantingCold      = SOUND_MARIO_PANTING_COLD,
        .soundPanting          = SOUND_MARIO_PANTING,
        .soundCoughing1        = SOUND_MARIO_COUGHING1,
        .soundCoughing2        = SOUND_MARIO_COUGHING2,
        .soundCoughing3        = SOUND_MARIO_COUGHING3,
        .soundPunchYah         = SOUND_MARIO_PUNCH_YAH,
        .soundPunchHoo         = SOUND_MARIO_PUNCH_HOO,
        .soundMamaMia          = SOUND_MARIO_MAMA_MIA,
        .soundGroundPoundWah   = SOUND_MARIO_GROUND_POUND_WAH,
        .soundDrowning         = SOUND_MARIO_DROWNING,
        .soundPunchWah         = SOUND_MARIO_PUNCH_WAH,
        .soundYahooWahaYippee  = SOUND_MARIO_YAHOO_WAHA_YIPPEE,
        .soundDoh              = SOUND_MARIO_DOH,
        .soundGameOver         = SOUND_MARIO_GAME_OVER,
        .soundHello            = SOUND_MARIO_HELLO,
        .soundPressStartToPlay = SOUND_MARIO_PRESS_START_TO_PLAY,
        .soundTwirlBounce      = SOUND_MARIO_TWIRL_BOUNCE,
        .soundSnoring3         = SOUND_MARIO_SNORING3,
        .soundSoLongaBowser    = SOUND_MARIO_SO_LONGA_BOWSER,
        .soundImaTired         = SOUND_MARIO_IMA_TIRED,
        .soundLetsAGo          = CHAR_BASS_SOUND(SOUND_MARIO_LETS_A_GO),
        .soundOkeyDokey        = CHAR_BASS_SOUND(SOUND_MARIO_OKEY_DOKEY),
    },

    [CT_LUIGI] = {
        .type                  = CT_LUIGI,
        .name                  = "Luigi",
        .hudHead               = '.',
        .hudHeadTexture        = { .texture = (u8*)texture_hud_char_luigi_head, .bitSize = 8, .width = 16, .height = 16 },
        .cameraHudHead         = GLYPH_CAM_LUIGI_HEAD,
        .modelId               = MODEL_LUIGI,
        .capModelId            = MODEL_LUIGIS_CAP,
        .capMetalModelId       = MODEL_LUIGIS_METAL_CAP,
        .capWingModelId        = MODEL_LUIGIS_WING_CAP,
        .capMetalWingModelId   = MODEL_LUIGIS_WINGED_METAL_CAP,
        .capEnemyLayer         = LAYER_OPAQUE,
        .capEnemyGfx           = luigi_cap_seg3_dl_03022F48,
        .capEnemyDecalGfx      = NULL,
        .torsoRotMult          = 1.0f,
        // anim
        .animOffsetEnabled     = false,
        // sounds
        .soundFreqScale        = 1.0f,
        .soundYahWahHoo        = SOUND_LUIGI_YAH_WAH_HOO,
        .soundHoohoo           = SOUND_LUIGI_HOOHOO,
        .soundYahoo            = SOUND_LUIGI_YAHOO,
        .soundUh               = SOUND_LUIGI_UH,
        .soundHrmm             = SOUND_LUIGI_HRMM,
        .soundWah2             = SOUND_LUIGI_WAH2,
        .soundWhoa             = SOUND_LUIGI_WHOA,
        .soundEeuh             = SOUND_LUIGI_EEUH,
        .soundAttacked         = SOUND_LUIGI_ATTACKED,
        .soundOoof             = SOUND_LUIGI_OOOF,
        .soundOoof2            = SOUND_LUIGI_OOOF2,
        .soundHereWeGo         = SOUND_LUIGI_HERE_WE_GO,
        .soundYawning          = SOUND_LUIGI_YAWNING,
        .soundSnoring1         = SOUND_LUIGI_SNORING1,
        .soundSnoring2         = SOUND_LUIGI_SNORING2,
        .soundWaaaooow         = SOUND_LUIGI_WAAAOOOW,
        .soundHaha             = SOUND_LUIGI_HAHA,
        .soundHaha_2           = SOUND_LUIGI_HAHA_2,
        .soundUh2              = SOUND_LUIGI_UH2,
        .soundUh2_2            = SOUND_LUIGI_UH2_2,
        .soundOnFire           = SOUND_LUIGI_ON_FIRE,
        .soundDying            = SOUND_LUIGI_DYING,
        .soundPantingCold      = SOUND_LUIGI_PANTING_COLD,
        .soundPanting          = SOUND_LUIGI_PANTING,
        .soundCoughing1        = SOUND_LUIGI_COUGHING1,
        .soundCoughing2        = SOUND_LUIGI_COUGHING2,
        .soundCoughing3        = SOUND_LUIGI_COUGHING3,
        .soundPunchYah         = SOUND_LUIGI_PUNCH_YAH,
        .soundPunchHoo         = SOUND_LUIGI_PUNCH_HOO,
        .soundMamaMia          = SOUND_LUIGI_MAMA_MIA,
        .soundGroundPoundWah   = SOUND_LUIGI_GROUND_POUND_WAH,
        .soundDrowning         = SOUND_LUIGI_DROWNING,
        .soundPunchWah         = SOUND_LUIGI_PUNCH_WAH,
        .soundYahooWahaYippee  = SOUND_LUIGI_YAHOO_WAHA_YIPPEE,
        .soundDoh              = SOUND_LUIGI_DOH,
        .soundGameOver         = SOUND_LUIGI_GAME_OVER,
        .soundHello            = SOUND_LUIGI_HELLO,
        .soundPressStartToPlay = SOUND_LUIGI_PRESS_START_TO_PLAY,
        .soundTwirlBounce      = SOUND_LUIGI_TWIRL_BOUNCE,
        .soundSnoring3         = SOUND_LUIGI_SNORING3,
        .soundSoLongaBowser    = SOUND_LUIGI_SO_LONGA_BOWSER,
        .soundImaTired         = SOUND_LUIGI_IMA_TIRED,
        .soundLetsAGo          = CHAR_BASS_SOUND(SOUND_LUIGI_LETS_A_GO),
        .soundOkeyDokey        = CHAR_BASS_SOUND(SOUND_LUIGI_OKEY_DOKEY),
    },

    [CT_TOAD] = {
        .type                  = CT_TOAD,
        .name                  = "Toad",
        .hudHead               = '/',
        .hudHeadTexture        = { .texture = (u8*)texture_hud_char_toad_head, .bitSize = 8, .width = 16, .height = 16 },
        .cameraHudHead         = GLYPH_CAM_TOAD_HEAD,
        .modelId               = MODEL_TOAD_PLAYER,
        .capModelId            = MODEL_TOADS_CAP,
        .capMetalModelId       = MODEL_TOADS_METAL_CAP,
        .capWingModelId        = MODEL_TOADS_WING_CAP,
        .capMetalWingModelId   = MODEL_TOADS_WING_CAP,
        .capEnemyLayer         = LAYER_TRANSPARENT,
        .capEnemyGfx           = toad_player_dl_cap,
        .capEnemyDecalGfx      = toad_player_dl_cap_decal,
        .torsoRotMult          = 1.0f,
        // anim
        .animOffsetEnabled     = false,
        // sounds
        .soundFreqScale        = 1.25f,
        .soundYahWahHoo        = SOUND_MARIO_YAH_WAH_HOO,
        .soundHoohoo           = SOUND_MARIO_HOOHOO,
        .soundYahoo            = SOUND_MARIO_YAHOO,
        .soundUh               = SOUND_MARIO_UH,
        .soundHrmm             = SOUND_MARIO_HRMM,
        .soundWah2             = SOUND_MARIO_WAH2,
        .soundWhoa             = SOUND_MARIO_WHOA,
        .soundEeuh             = SOUND_MARIO_EEUH,
        .soundAttacked         = SOUND_MARIO_ATTACKED,
        .soundOoof             = SOUND_MARIO_OOOF,
        .soundOoof2            = SOUND_MARIO_OOOF2,
        .soundHereWeGo         = SOUND_MARIO_HERE_WE_GO,
        .soundYawning          = SOUND_MARIO_YAWNING,
        .soundSnoring1         = SOUND_MARIO_SNORING1,
        .soundSnoring2         = SOUND_MARIO_SNORING2,
        .soundWaaaooow         = SOUND_MARIO_WAAAOOOW,
        .soundHaha             = SOUND_MARIO_HAHA,
        .soundHaha_2           = SOUND_MARIO_HAHA_2,
        .soundUh2              = SOUND_MARIO_UH2,
        .soundUh2_2            = SOUND_MARIO_UH2_2,
        .soundOnFire           = SOUND_MARIO_ON_FIRE,
        .soundDying            = SOUND_MARIO_DYING,
        .soundPantingCold      = SOUND_MARIO_PANTING_COLD,
        .soundPanting          = SOUND_MARIO_PANTING,
        .soundCoughing1        = SOUND_MARIO_COUGHING1,
        .soundCoughing2        = SOUND_MARIO_COUGHING2,
        .soundCoughing3        = SOUND_MARIO_COUGHING3,
        .soundPunchYah         = SOUND_MARIO_PUNCH_YAH,
        .soundPunchHoo         = SOUND_MARIO_PUNCH_HOO,
        .soundMamaMia          = SOUND_MARIO_MAMA_MIA,
        .soundGroundPoundWah   = SOUND_MARIO_GROUND_POUND_WAH,
        .soundDrowning         = SOUND_MARIO_DROWNING,
        .soundPunchWah         = SOUND_MARIO_PUNCH_WAH,
        .soundYahooWahaYippee  = SOUND_MARIO_YAHOO_WAHA_YIPPEE,
        .soundDoh              = SOUND_MARIO_DOH,
        .soundGameOver         = SOUND_MARIO_GAME_OVER,
        .soundHello            = SOUND_MARIO_HELLO,
        .soundPressStartToPlay = SOUND_MARIO_PRESS_START_TO_PLAY,
        .soundTwirlBounce      = SOUND_MARIO_TWIRL_BOUNCE,
        .soundSnoring3         = SOUND_MARIO_SNORING3,
        .soundSoLongaBowser    = SOUND_MARIO_SO_LONGA_BOWSER,
        .soundImaTired         = SOUND_MARIO_IMA_TIRED,
        .soundLetsAGo          = CHAR_BASS_SOUND(SOUND_MARIO_LETS_A_GO),
        .soundOkeyDokey        = CHAR_BASS_SOUND(SOUND_MARIO_OKEY_DOKEY),
    },

    [CT_WALUIGI] = {
        .type                  = CT_WALUIGI,
        .name                  = "Waluigi",
        .hudHead               = 'z',
        .hudHeadTexture        = { .texture = (u8*)texture_hud_char_waluigi_head, .bitSize = 8, .width = 16, .height = 16 },
        .cameraHudHead         = GLYPH_CAM_WALUIGI_HEAD,
        .modelId               = MODEL_WALUIGI,
        .capModelId            = MODEL_WALUIGIS_CAP,
        .capMetalModelId       = MODEL_WALUIGIS_METAL_CAP,
        .capWingModelId        = MODEL_WALUIGIS_WING_CAP,
        .capMetalWingModelId   = MODEL_WALUIGIS_WINGED_METAL_CAP,
        .capEnemyLayer         = LAYER_OPAQUE,
        .capEnemyGfx           = waluigi_cap_seg3_dl_03022F48,
        .capEnemyDecalGfx      = NULL,
        .torsoRotMult          = 1.0f,
        // anim
        .animOffsetEnabled     = true,
        .animOffsetLowYPoint   = 11,
        .animOffsetFeet        = 25,
        .animOffsetHand        = -10,
        // sounds
        .soundFreqScale        = 0.99f,
        .soundYahWahHoo        = SOUND_LUIGI_YAH_WAH_HOO,
        .soundHoohoo           = SOUND_LUIGI_HOOHOO,
        .soundYahoo            = SOUND_LUIGI_YAHOO,
        .soundUh               = SOUND_LUIGI_UH,
        .soundHrmm             = SOUND_LUIGI_HRMM,
        .soundWah2             = SOUND_LUIGI_WAH2,
        .soundWhoa             = SOUND_LUIGI_WHOA,
        .soundEeuh             = SOUND_LUIGI_EEUH,
        .soundAttacked         = SOUND_LUIGI_ATTACKED,
        .soundOoof             = SOUND_LUIGI_OOOF,
        .soundOoof2            = SOUND_LUIGI_OOOF2,
        .soundHereWeGo         = SOUND_LUIGI_HERE_WE_GO,
        .soundYawning          = SOUND_LUIGI_YAWNING,
        .soundSnoring1         = SOUND_LUIGI_SNORING1,
        .soundSnoring2         = SOUND_LUIGI_SNORING2,
        .soundWaaaooow         = SOUND_LUIGI_WAAAOOOW,
        .soundHaha             = SOUND_LUIGI_HAHA,
        .soundHaha_2           = SOUND_LUIGI_HAHA_2,
        .soundUh2              = SOUND_LUIGI_UH2,
        .soundUh2_2            = SOUND_LUIGI_UH2_2,
        .soundOnFire           = SOUND_LUIGI_ON_FIRE,
        .soundDying            = SOUND_LUIGI_DYING,
        .soundPantingCold      = SOUND_LUIGI_PANTING_COLD,
        .soundPanting          = SOUND_LUIGI_PANTING,
        .soundCoughing1        = SOUND_LUIGI_COUGHING1,
        .soundCoughing2        = SOUND_LUIGI_COUGHING2,
        .soundCoughing3        = SOUND_LUIGI_COUGHING3,
        .soundPunchYah         = SOUND_LUIGI_PUNCH_YAH,
        .soundPunchHoo         = SOUND_LUIGI_PUNCH_HOO,
        .soundMamaMia          = SOUND_LUIGI_MAMA_MIA,
        .soundGroundPoundWah   = SOUND_LUIGI_GROUND_POUND_WAH,
        .soundDrowning         = SOUND_LUIGI_DROWNING,
        .soundPunchWah         = SOUND_LUIGI_PUNCH_WAH,
        .soundYahooWahaYippee  = SOUND_LUIGI_YAHOO_WAHA_YIPPEE,
        .soundDoh              = SOUND_LUIGI_DOH,
        .soundGameOver         = SOUND_LUIGI_GAME_OVER,
        .soundHello            = SOUND_LUIGI_HELLO,
        .soundPressStartToPlay = SOUND_LUIGI_PRESS_START_TO_PLAY,
        .soundTwirlBounce      = SOUND_LUIGI_TWIRL_BOUNCE,
        .soundSnoring3         = SOUND_LUIGI_SNORING3,
        .soundSoLongaBowser    = SOUND_LUIGI_SO_LONGA_BOWSER,
        .soundImaTired         = SOUND_LUIGI_IMA_TIRED,
        .soundLetsAGo          = CHAR_BASS_SOUND(SOUND_LUIGI_LETS_A_GO),
        .soundOkeyDokey        = CHAR_BASS_SOUND(SOUND_LUIGI_OKEY_DOKEY),
    },

    [CT_WARIO] = {
        .type                  = CT_WARIO,
        .name                  = "Wario",
        .hudHead               = 'x',
        .hudHeadTexture        = { .texture = (u8*)texture_hud_char_wario_head, .bitSize = 8, .width = 16, .height = 16 },
        .cameraHudHead         = GLYPH_CAM_WARIO_HEAD,
        .modelId               = MODEL_WARIO,
        .capModelId            = MODEL_WARIOS_CAP,
        .capMetalModelId       = MODEL_WARIOS_METAL_CAP,
        .capWingModelId        = MODEL_WARIOS_WING_CAP,
        .capMetalWingModelId   = MODEL_WARIOS_WINGED_METAL_CAP,
        .capEnemyLayer         = LAYER_OPAQUE,
        .capEnemyGfx           = wario_cap_seg3_dl_03022F48,
        .capEnemyDecalGfx      = NULL,
        .torsoRotMult          = 3.0f / 5.0f,
        // anim
        .animOffsetEnabled     = true,
        // sounds
        .soundFreqScale        = 1.0f,
        .soundYahWahHoo        = SOUND_WARIO_YAH_WAH_HOO,
        .soundHoohoo           = SOUND_WARIO_HOOHOO,
        .soundYahoo            = SOUND_WARIO_YAHOO,
        .soundUh               = SOUND_WARIO_UH,
        .soundHrmm             = SOUND_WARIO_HRMM,
        .soundWah2             = SOUND_WARIO_WAH2,
        .soundWhoa             = SOUND_WARIO_WHOA,
        .soundEeuh             = SOUND_WARIO_EEUH,
        .soundAttacked         = SOUND_WARIO_ATTACKED,
        .soundOoof             = SOUND_WARIO_OOOF,
        .soundOoof2            = SOUND_WARIO_OOOF2,
        .soundHereWeGo         = SOUND_WARIO_HERE_WE_GO,
        .soundYawning          = SOUND_WARIO_YAWNING,
        .soundSnoring1         = SOUND_WARIO_SNORING1,
        .soundSnoring2         = SOUND_WARIO_SNORING2,
        .soundWaaaooow         = SOUND_WARIO_WAAAOOOW,
        .soundHaha             = SOUND_WARIO_HAHA,
        .soundHaha_2           = SOUND_WARIO_HAHA_2,
        .soundUh2              = SOUND_WARIO_UH2,
        .soundUh2_2            = SOUND_WARIO_UH2_2,
        .soundOnFire           = SOUND_WARIO_ON_FIRE,
        .soundDying            = SOUND_WARIO_DYING,
        .soundPantingCold      = SOUND_WARIO_PANTING_COLD,
        .soundPanting          = SOUND_WARIO_PANTING,
        .soundCoughing1        = SOUND_WARIO_COUGHING1,
        .soundCoughing2        = SOUND_WARIO_COUGHING2,
        .soundCoughing3        = SOUND_WARIO_COUGHING3,
        .soundPunchYah         = SOUND_WARIO_PUNCH_YAH,
        .soundPunchHoo         = SOUND_WARIO_PUNCH_HOO,
        .soundMamaMia          = SOUND_WARIO_MAMA_MIA,
        .soundGroundPoundWah   = SOUND_WARIO_GROUND_POUND_WAH,
        .soundDrowning         = SOUND_WARIO_DROWNING,
        .soundPunchWah         = SOUND_WARIO_PUNCH_WAH,
        .soundYahooWahaYippee  = SOUND_WARIO_YAHOO_WAHA_YIPPEE,
        .soundDoh              = SOUND_WARIO_DOH,
        .soundGameOver         = SOUND_WARIO_GAME_OVER,
        .soundHello            = SOUND_WARIO_HELLO,
        .soundPressStartToPlay = SOUND_WARIO_PRESS_START_TO_PLAY,
        .soundTwirlBounce      = SOUND_WARIO_TWIRL_BOUNCE,
        .soundSnoring3         = SOUND_WARIO_SNORING3,
        .soundSoLongaBowser    = SOUND_WARIO_SO_LONGA_BOWSER,
        .soundImaTired         = SOUND_WARIO_IMA_TIRED,
        .soundLetsAGo          = CHAR_BASS_SOUND(SOUND_WARIO_LETS_A_GO),
        .soundOkeyDokey        = CHAR_BASS_SOUND(SOUND_WARIO_OKEY_DOKEY),
    },
};

const struct PlayerPalette DEFAULT_MARIO_PALETTE =
{{{ 0x00, 0x00, 0xff }, { 0xff, 0x00, 0x00 }, { 0xff, 0xff, 0xff }, { 0x72, 0x1c, 0x0e }, { 0x73, 0x06, 0x00 }, { 0xfe, 0xc1, 0x79 }, { 0xff, 0x00, 0x00 }}};

const struct PlayerPalette gPalettePresets[PALETTE_PRESET_MAX] = {
/*  ---- PANTS -----      ---- SHIRT -----      ---- GLOVES ----      ---- SHOES -----      ----- HAIR -----      ----- SKIN -----      ----- CAP ------  */
{{{ 0x00, 0x00, 0xff }, { 0xff, 0x00, 0x00 }, { 0xff, 0xff, 0xff }, { 0x72, 0x1c, 0x0e }, { 0x73, 0x06, 0x00 }, { 0xfe, 0xc1, 0x79 }, { 0xff, 0x00, 0x00 }}}, // Mario
{{{ 0x00, 0x00, 0xfe }, { 0x00, 0x98, 0x00 }, { 0xff, 0xff, 0xff }, { 0x72, 0x1c, 0x0e }, { 0x73, 0x06, 0x00 }, { 0xfe, 0xc1, 0x79 }, { 0x00, 0x98, 0x00 }}}, // Luigi
{{{ 0x2c, 0x26, 0x3f }, { 0x6d, 0x3c, 0x9a }, { 0xff, 0xff, 0xff }, { 0x72, 0x1c, 0x0e }, { 0x73, 0x06, 0x00 }, { 0xfe, 0xc1, 0x79 }, { 0x6d, 0x3c, 0x9a }}}, // Waluigi
{{{ 0x7f, 0x20, 0x7a }, { 0xf9, 0xeb, 0x30 }, { 0xff, 0xff, 0xff }, { 0x0e, 0x72, 0x1c }, { 0x73, 0x53, 0x00 }, { 0xfe, 0xc1, 0x79 }, { 0xf9, 0xeb, 0x30 }}}, // Wario
{{{ 0xff, 0x00, 0x00 }, { 0x7b, 0x00, 0xde }, { 0xff, 0xff, 0xff }, { 0x72, 0x1c, 0x0e }, { 0x73, 0x06, 0x00 }, { 0xfe, 0xc1, 0x79 }, { 0x7b, 0x00, 0xde }}}, // Chuckya
{{{ 0xc6, 0xb1, 0x32 }, { 0x95, 0x43, 0x01 }, { 0xff, 0xff, 0xff }, { 0x72, 0x1c, 0x0e }, { 0x73, 0x06, 0x00 }, { 0xfe, 0xc1, 0x79 }, { 0x95, 0x43, 0x01 }}}, // Goomba
{{{ 0x07, 0x09, 0x07 }, { 0x4c, 0x5f, 0x20 }, { 0xff, 0xff, 0xff }, { 0x72, 0x1c, 0x0e }, { 0x73, 0x06, 0x00 }, { 0xfe, 0xc1, 0x79 }, { 0x4c, 0x5f, 0x20 }}}, // Clover
{{{ 0xbf, 0xde, 0xff }, { 0x00, 0x2f, 0xc8 }, { 0xff, 0xff, 0xff }, { 0x72, 0x1c, 0x0e }, { 0x73, 0x06, 0x00 }, { 0xfe, 0xc1, 0x79 }, { 0x00, 0x2f, 0xc8 }}}, // Cobalt
{{{ 0xf8, 0x3b, 0x05 }, { 0x11, 0x11, 0x11 }, { 0xff, 0xff, 0xff }, { 0x72, 0x1c, 0x0e }, { 0x73, 0x06, 0x00 }, { 0xfe, 0xc1, 0x79 }, { 0x11, 0x11, 0x11 }}}, // Fury
{{{ 0x34, 0x16, 0x0d }, { 0xc1, 0x2c, 0x72 }, { 0xff, 0xff, 0xff }, { 0x72, 0x1c, 0x0e }, { 0x73, 0x06, 0x00 }, { 0xfe, 0xc1, 0x79 }, { 0xc1, 0x2c, 0x72 }}}, // Hot Pink
{{{ 0xff, 0x00, 0x00 }, { 0xff, 0x96, 0xc8 }, { 0xff, 0xff, 0xff }, { 0x72, 0x1c, 0x0e }, { 0x73, 0x06, 0x00 }, { 0xfe, 0xc1, 0x79 }, { 0xff, 0x96, 0xc8 }}}, // Nice Pink
{{{ 0x81, 0x00, 0x00 }, { 0x4c, 0xff, 0x4c }, { 0xff, 0xff, 0xff }, { 0x72, 0x1c, 0x0e }, { 0x73, 0x06, 0x00 }, { 0xfe, 0xc1, 0x79 }, { 0x4c, 0xff, 0x4c }}}, // Seafoam
{{{ 0x61, 0x3d, 0x2e }, { 0xa9, 0x78, 0xfc }, { 0xff, 0xff, 0xff }, { 0x72, 0x1c, 0x0e }, { 0x73, 0x06, 0x00 }, { 0xfe, 0xc1, 0x79 }, { 0xa9, 0x78, 0xfc }}}, // Lilac
{{{ 0x00, 0x46, 0x5c }, { 0x84, 0x60, 0x00 }, { 0xff, 0xff, 0xff }, { 0x72, 0x1c, 0x0e }, { 0x73, 0x06, 0x00 }, { 0xfe, 0xc1, 0x79 }, { 0x84, 0x60, 0x00 }}}, // Copper
{{{ 0x4f, 0x31, 0x8b }, { 0x5a, 0x94, 0xff }, { 0xff, 0xff, 0xff }, { 0x72, 0x1c, 0x0e }, { 0x73, 0x06, 0x00 }, { 0xfe, 0xc1, 0x79 }, { 0x5a, 0x94, 0xff }}}, // Azure
{{{ 0x23, 0x11, 0x03 }, { 0x68, 0x0a, 0x17 }, { 0xff, 0xff, 0xff }, { 0x72, 0x1c, 0x0e }, { 0x73, 0x06, 0x00 }, { 0xfe, 0xc1, 0x79 }, { 0x68, 0x0a, 0x17 }}}, // Burgundy
{{{ 0x53, 0x39, 0x3d }, { 0x95, 0xd0, 0x8f }, { 0xff, 0xff, 0xff }, { 0x72, 0x1c, 0x0e }, { 0x73, 0x06, 0x00 }, { 0xfe, 0xc1, 0x79 }, { 0x95, 0xd0, 0x8f }}}, // Mint
{{{ 0xe6, 0xe3, 0xff }, { 0x37, 0x32, 0x42 }, { 0xff, 0xff, 0xff }, { 0x72, 0x1c, 0x0e }, { 0x73, 0x06, 0x00 }, { 0xfe, 0xc1, 0x79 }, { 0x37, 0x32, 0x42 }}}, // Eggplant
{{{ 0x00, 0x51, 0x10 }, { 0xff, 0x8a, 0x00 }, { 0xff, 0xff, 0xff }, { 0x72, 0x1c, 0x0e }, { 0x73, 0x06, 0x00 }, { 0xfe, 0xc1, 0x79 }, { 0xff, 0x8a, 0x00 }}}, // Orange
{{{ 0x4c, 0x1e, 0x3f }, { 0x65, 0xfa, 0xff }, { 0xff, 0xff, 0xff }, { 0x72, 0x1c, 0x0e }, { 0x73, 0x06, 0x00 }, { 0xfe, 0xc1, 0x79 }, { 0x65, 0xfa, 0xff }}}, // Arctic
{{{ 0xb2, 0x28, 0x18 }, { 0xe6, 0xe6, 0xe6 }, { 0xff, 0xff, 0xff }, { 0x72, 0x1c, 0x0e }, { 0x73, 0x06, 0x00 }, { 0xfe, 0xc1, 0x79 }, { 0xe6, 0xe6, 0xe6 }}}, // Fire Mario
{{{ 0x00, 0x98, 0x00 }, { 0xe6, 0xe6, 0xe6 }, { 0xff, 0xff, 0xff }, { 0x72, 0x1c, 0x0e }, { 0x73, 0x06, 0x00 }, { 0xfe, 0xc1, 0x79 }, { 0xe6, 0xe6, 0xe6 }}}, // Fire Luigi
{{{ 0x6d, 0x3c, 0x9a }, { 0xe6, 0xe6, 0xe6 }, { 0xff, 0xff, 0xff }, { 0x72, 0x1c, 0x0e }, { 0x73, 0x06, 0x00 }, { 0xfe, 0xc1, 0x79 }, { 0xe6, 0xe6, 0xe6 }}}, // Fire Waluigi
{{{ 0xf9, 0xeb, 0x30 }, { 0xe6, 0xe6, 0xe6 }, { 0xff, 0xff, 0xff }, { 0x0e, 0x72, 0x1c }, { 0x73, 0x53, 0x00 }, { 0xfe, 0xc1, 0x79 }, { 0xe6, 0xe6, 0xe6 }}}, // Fire Wario
{{{ 0x17, 0x18, 0x15 }, { 0xe7, 0xe7, 0x21 }, { 0xff, 0xff, 0xff }, { 0x72, 0x1c, 0x0e }, { 0x73, 0x06, 0x00 }, { 0xfe, 0xc1, 0x79 }, { 0xe7, 0xe7, 0x21 }}}, // Busy Bee
{{{ 0xf7, 0x9a, 0x47 }, { 0xaa, 0x27, 0x31 }, { 0xff, 0xff, 0xff }, { 0x72, 0x1c, 0x0e }, { 0x73, 0x06, 0x00 }, { 0xfe, 0xc1, 0x79 }, { 0xaa, 0x27, 0x31 }}}, // Fortress
{{{ 0xf7, 0xc2, 0x45 }, { 0x55, 0x92, 0xb2 }, { 0xff, 0xff, 0xff }, { 0x72, 0x1c, 0x0e }, { 0x73, 0x06, 0x00 }, { 0xfe, 0xc1, 0x79 }, { 0x55, 0x92, 0xb2 }}}, // Battlements
{{{ 0xeb, 0x8a, 0x4b }, { 0x10, 0x1b, 0x2e }, { 0xff, 0xff, 0xff }, { 0x72, 0x1c, 0x0e }, { 0x73, 0x06, 0x00 }, { 0xfe, 0xc1, 0x79 }, { 0x10, 0x1b, 0x2e }}}, // Blueberry Pie
{{{ 0xd6, 0x35, 0x4d }, { 0x3b, 0x8f, 0xf7 }, { 0xff, 0xff, 0xff }, { 0x72, 0x1c, 0x0e }, { 0x73, 0x06, 0x00 }, { 0xfe, 0xc1, 0x79 }, { 0x3b, 0x8f, 0xf7 }}}, // Raspberry
{{{ 0xd6, 0x35, 0x4d }, { 0xff, 0x8e, 0xb2 }, { 0xff, 0xff, 0xff }, { 0x72, 0x1c, 0x0e }, { 0x73, 0x06, 0x00 }, { 0xfe, 0xc1, 0x79 }, { 0xff, 0x8e, 0xb2 }}}, // Bubblegum
{{{ 0xb2, 0x28, 0x18 }, { 0x47, 0xc5, 0xff }, { 0xff, 0xff, 0xff }, { 0x72, 0x1c, 0x0e }, { 0x73, 0x06, 0x00 }, { 0xfe, 0xc1, 0x79 }, { 0x47, 0xc5, 0xff }}}, // Ice Mario
{{{ 0x00, 0x98, 0x00 }, { 0x47, 0xc5, 0xff }, { 0xff, 0xff, 0xff }, { 0x72, 0x1c, 0x0e }, { 0x73, 0x06, 0x00 }, { 0xfe, 0xc1, 0x79 }, { 0x47, 0xc5, 0xff }}}, // Ice Luigi
};

enum AnimType {
    ANIM_TYPE_NONE,
    ANIM_TYPE_LOWY,
    ANIM_TYPE_FEET,
    ANIM_TYPE_HAND,
};

#define ANIM_TYPE_MAX 209
u8 sAnimTypes[ANIM_TYPE_MAX] = {
    ANIM_TYPE_HAND, ANIM_TYPE_LOWY, ANIM_TYPE_NONE, ANIM_TYPE_NONE, ANIM_TYPE_NONE,
    ANIM_TYPE_NONE, ANIM_TYPE_NONE, ANIM_TYPE_NONE, ANIM_TYPE_NONE, ANIM_TYPE_NONE,
    ANIM_TYPE_NONE, ANIM_TYPE_NONE, ANIM_TYPE_NONE, ANIM_TYPE_NONE, ANIM_TYPE_NONE,
    ANIM_TYPE_LOWY, ANIM_TYPE_LOWY, ANIM_TYPE_LOWY, ANIM_TYPE_LOWY, ANIM_TYPE_FEET,
    ANIM_TYPE_LOWY, ANIM_TYPE_NONE, ANIM_TYPE_FEET, ANIM_TYPE_FEET, ANIM_TYPE_FEET,
    ANIM_TYPE_LOWY, ANIM_TYPE_LOWY, ANIM_TYPE_LOWY, ANIM_TYPE_NONE, ANIM_TYPE_LOWY,
    ANIM_TYPE_LOWY, ANIM_TYPE_LOWY, ANIM_TYPE_LOWY, ANIM_TYPE_LOWY, ANIM_TYPE_LOWY,
    ANIM_TYPE_FEET, ANIM_TYPE_FEET, ANIM_TYPE_FEET, ANIM_TYPE_NONE, ANIM_TYPE_LOWY,
    ANIM_TYPE_NONE, ANIM_TYPE_NONE, ANIM_TYPE_NONE, ANIM_TYPE_HAND, ANIM_TYPE_LOWY,
    ANIM_TYPE_NONE, ANIM_TYPE_LOWY, ANIM_TYPE_LOWY, ANIM_TYPE_LOWY, ANIM_TYPE_LOWY,
    ANIM_TYPE_LOWY, ANIM_TYPE_HAND, ANIM_TYPE_FEET, ANIM_TYPE_HAND, ANIM_TYPE_LOWY,
    ANIM_TYPE_LOWY, ANIM_TYPE_LOWY, ANIM_TYPE_FEET, ANIM_TYPE_NONE, ANIM_TYPE_NONE,
    ANIM_TYPE_NONE, ANIM_TYPE_NONE, ANIM_TYPE_NONE, ANIM_TYPE_LOWY, ANIM_TYPE_LOWY,
    ANIM_TYPE_NONE, ANIM_TYPE_LOWY, ANIM_TYPE_NONE, ANIM_TYPE_NONE, ANIM_TYPE_NONE,
    ANIM_TYPE_LOWY, ANIM_TYPE_LOWY, ANIM_TYPE_FEET, ANIM_TYPE_NONE, ANIM_TYPE_LOWY,
    ANIM_TYPE_LOWY, ANIM_TYPE_NONE, ANIM_TYPE_LOWY, ANIM_TYPE_LOWY, ANIM_TYPE_FEET,
    ANIM_TYPE_NONE, ANIM_TYPE_NONE, ANIM_TYPE_LOWY, ANIM_TYPE_LOWY, ANIM_TYPE_LOWY,
    ANIM_TYPE_FEET, ANIM_TYPE_NONE, ANIM_TYPE_LOWY, ANIM_TYPE_NONE, ANIM_TYPE_LOWY,
    ANIM_TYPE_LOWY, ANIM_TYPE_NONE, ANIM_TYPE_HAND, ANIM_TYPE_HAND, ANIM_TYPE_LOWY,
    ANIM_TYPE_FEET, ANIM_TYPE_FEET, ANIM_TYPE_LOWY, ANIM_TYPE_LOWY, ANIM_TYPE_LOWY,
    ANIM_TYPE_LOWY, ANIM_TYPE_LOWY, ANIM_TYPE_LOWY, ANIM_TYPE_LOWY, ANIM_TYPE_LOWY,
    ANIM_TYPE_LOWY, ANIM_TYPE_LOWY, ANIM_TYPE_LOWY, ANIM_TYPE_LOWY, ANIM_TYPE_LOWY,
    ANIM_TYPE_LOWY, ANIM_TYPE_NONE, ANIM_TYPE_NONE, ANIM_TYPE_LOWY, ANIM_TYPE_FEET,
    ANIM_TYPE_FEET, ANIM_TYPE_LOWY, ANIM_TYPE_LOWY, ANIM_TYPE_NONE, ANIM_TYPE_NONE,
    ANIM_TYPE_NONE, ANIM_TYPE_LOWY, ANIM_TYPE_LOWY, ANIM_TYPE_LOWY, ANIM_TYPE_LOWY,
    ANIM_TYPE_LOWY, ANIM_TYPE_LOWY, ANIM_TYPE_LOWY, ANIM_TYPE_LOWY, ANIM_TYPE_LOWY,
    ANIM_TYPE_LOWY, ANIM_TYPE_LOWY, ANIM_TYPE_LOWY, ANIM_TYPE_NONE, ANIM_TYPE_NONE,
    ANIM_TYPE_NONE, ANIM_TYPE_NONE, ANIM_TYPE_NONE, ANIM_TYPE_LOWY, ANIM_TYPE_LOWY,
    ANIM_TYPE_LOWY, ANIM_TYPE_LOWY, ANIM_TYPE_NONE, ANIM_TYPE_LOWY, ANIM_TYPE_NONE,
    ANIM_TYPE_NONE, ANIM_TYPE_FEET, ANIM_TYPE_LOWY, ANIM_TYPE_LOWY, ANIM_TYPE_LOWY,
    ANIM_TYPE_LOWY, ANIM_TYPE_FEET, ANIM_TYPE_LOWY, ANIM_TYPE_NONE, ANIM_TYPE_LOWY,
    ANIM_TYPE_LOWY, ANIM_TYPE_LOWY, ANIM_TYPE_LOWY, ANIM_TYPE_NONE, ANIM_TYPE_NONE,
    ANIM_TYPE_NONE, ANIM_TYPE_NONE, ANIM_TYPE_NONE, ANIM_TYPE_NONE, ANIM_TYPE_NONE,
    ANIM_TYPE_NONE, ANIM_TYPE_NONE, ANIM_TYPE_NONE, ANIM_TYPE_NONE, ANIM_TYPE_NONE,
    ANIM_TYPE_NONE, ANIM_TYPE_NONE, ANIM_TYPE_NONE, ANIM_TYPE_NONE, ANIM_TYPE_NONE,
    ANIM_TYPE_NONE, ANIM_TYPE_NONE, ANIM_TYPE_NONE, ANIM_TYPE_NONE, ANIM_TYPE_NONE,
    ANIM_TYPE_NONE, ANIM_TYPE_LOWY, ANIM_TYPE_LOWY, ANIM_TYPE_LOWY, ANIM_TYPE_LOWY,
    ANIM_TYPE_LOWY, ANIM_TYPE_LOWY, ANIM_TYPE_LOWY, ANIM_TYPE_LOWY, ANIM_TYPE_LOWY,
    ANIM_TYPE_LOWY, ANIM_TYPE_NONE, ANIM_TYPE_LOWY, ANIM_TYPE_NONE, ANIM_TYPE_LOWY,
    ANIM_TYPE_LOWY, ANIM_TYPE_LOWY, ANIM_TYPE_LOWY, ANIM_TYPE_HAND, ANIM_TYPE_HAND,
    ANIM_TYPE_LOWY, ANIM_TYPE_LOWY, ANIM_TYPE_FEET, ANIM_TYPE_LOWY, ANIM_TYPE_LOWY,
    ANIM_TYPE_LOWY, ANIM_TYPE_LOWY, ANIM_TYPE_NONE, ANIM_TYPE_NONE,
};

struct Character* get_character(struct MarioState* m) {
    return (m == NULL || m->character == NULL)
        ? &gCharacters[CT_MARIO]
        : m->character;
}

static s32 get_character_sound(struct MarioState* m, enum CharacterSound characterSound) {
    if (m == NULL || m->marioObj == NULL) { return 0; }
	
    s32 override = 0;
    if (smlua_call_event_hooks_mario_charactersound_param_ret_int(HOOK_CHARACTER_SOUND, m, characterSound, &override)) {
        return override;
    }
	
    struct Character* character = ((m == NULL || m->character == NULL) ? &gCharacters[CT_MARIO] : m->character);
    switch (characterSound) {
        case CHAR_SOUND_YAH_WAH_HOO:         return character->soundYahWahHoo;
        case CHAR_SOUND_HOOHOO:              return character->soundHoohoo;
        case CHAR_SOUND_YAHOO:               return character->soundYahoo;
        case CHAR_SOUND_UH:                  return character->soundUh;
        case CHAR_SOUND_HRMM:                return character->soundHrmm;
        case CHAR_SOUND_WAH2:                return character->soundWah2;
        case CHAR_SOUND_WHOA:                return character->soundWhoa;
        case CHAR_SOUND_EEUH:                return character->soundEeuh;
        case CHAR_SOUND_ATTACKED:            return character->soundAttacked;
        case CHAR_SOUND_OOOF:                return character->soundOoof;
        case CHAR_SOUND_OOOF2:               return character->soundOoof2;
        case CHAR_SOUND_HERE_WE_GO:          return character->soundHereWeGo;
        case CHAR_SOUND_YAWNING:             return character->soundYawning;
        case CHAR_SOUND_SNORING1:            return character->soundSnoring1;
        case CHAR_SOUND_SNORING2:            return character->soundSnoring2;
        case CHAR_SOUND_WAAAOOOW:            return character->soundWaaaooow;
        case CHAR_SOUND_HAHA:                return character->soundHaha;
        case CHAR_SOUND_HAHA_2:              return character->soundHaha_2;
        case CHAR_SOUND_UH2:                 return character->soundUh2;
        case CHAR_SOUND_UH2_2:               return character->soundUh2_2;
        case CHAR_SOUND_ON_FIRE:             return character->soundOnFire;
        case CHAR_SOUND_DYING:               return character->soundDying;
        case CHAR_SOUND_PANTING_COLD:        return character->soundPantingCold;
        case CHAR_SOUND_PANTING:             return character->soundPanting;
        case CHAR_SOUND_COUGHING1:           return character->soundCoughing1;
        case CHAR_SOUND_COUGHING2:           return character->soundCoughing2;
        case CHAR_SOUND_COUGHING3:           return character->soundCoughing3;
        case CHAR_SOUND_PUNCH_YAH:           return character->soundPunchYah;
        case CHAR_SOUND_PUNCH_HOO:           return character->soundPunchHoo;
        case CHAR_SOUND_MAMA_MIA:            return character->soundMamaMia;
        case CHAR_SOUND_GROUND_POUND_WAH:    return character->soundGroundPoundWah;
        case CHAR_SOUND_DROWNING:            return character->soundDrowning;
        case CHAR_SOUND_PUNCH_WAH:           return character->soundPunchWah;
        case CHAR_SOUND_YAHOO_WAHA_YIPPEE:   return character->soundYahooWahaYippee;
        case CHAR_SOUND_DOH:                 return character->soundDoh;
        case CHAR_SOUND_GAME_OVER:           return character->soundGameOver;
        case CHAR_SOUND_HELLO:               return character->soundHello;
        case CHAR_SOUND_PRESS_START_TO_PLAY: return character->soundPressStartToPlay;
        case CHAR_SOUND_TWIRL_BOUNCE:        return character->soundTwirlBounce;
        case CHAR_SOUND_SNORING3:            return character->soundSnoring3;
        case CHAR_SOUND_SO_LONGA_BOWSER:     return character->soundSoLongaBowser;
        case CHAR_SOUND_IMA_TIRED:           return character->soundImaTired;
        case CHAR_SOUND_LETS_A_GO:           return character->soundLetsAGo;
        case CHAR_SOUND_OKEY_DOKEY:          return character->soundOkeyDokey;
        default:                             return 0;
    }
}

static void play_character_sound_internal(struct MarioState *m, enum CharacterSound characterSound, u32 offset, u32 flags) {
    if (m != NULL && (m->flags & flags) == 0) {
        s32 sound = get_character_sound(m, characterSound);
        if (sound != 0) {
            struct Character* character = get_character(m);
            f32 *pos = (m->marioObj != NULL ? m->marioObj->header.gfx.cameraToObject : gGlobalSoundSource);
            if ((u32) (sound & CHAR_BASS_MAGIC) == CHAR_BASS_MAGIC) {
                CharacterBassSound *cbs = get_character_bass_sound(sound);
                if (cbs != NULL) {
                    play_character_bass_sound(cbs, pos, character->soundFreqScale);
                }
            } else {
                play_sound_with_freq_scale(sound + offset, pos, character->soundFreqScale);
            }
        }
        m->flags |= flags;
    }
}

void play_character_sound(struct MarioState* m, enum CharacterSound characterSound) {
    play_character_sound_internal(m, characterSound, 0, 0);
}

void play_character_sound_offset(struct MarioState* m, enum CharacterSound characterSound, u32 offset) {
    play_character_sound_internal(m, characterSound, offset, 0);
}

void play_character_sound_if_no_flag(struct MarioState* m, enum CharacterSound characterSound, u32 flags) {
    play_character_sound_internal(m, characterSound, 0, flags);
}

f32 get_character_anim_offset(struct MarioState* m) {
    if (m == NULL || m->marioObj == NULL) { return 0; }
    struct Object* marioObj = m->marioObj;
    struct Character* c = get_character(m);
    if (!c->animOffsetEnabled) { return 0; }
    s32 animID = marioObj->header.gfx.animInfo.animID;
    if (animID < 0 || animID >= ANIM_TYPE_MAX) { return 0; }

    switch (sAnimTypes[animID]) {
        case ANIM_TYPE_LOWY:
            if (m->minimumBoneY < c->animOffsetLowYPoint) {
                return -(m->minimumBoneY - c->animOffsetLowYPoint);
            }
            break;
        case ANIM_TYPE_FEET:
            return c->animOffsetFeet;
            break;
        case ANIM_TYPE_HAND:
            return c->animOffsetHand;
            break;
    }
    return 0;
}

void update_character_anim_offset(struct MarioState* m) {
    if (m == NULL || m->marioObj == NULL) { return; }
    struct Object* marioObj = m->marioObj;
    struct Character* c = get_character(m);
    if (!c->animOffsetEnabled) { return; }

    f32 targetOffset = get_character_anim_offset(m);

    // smooth
    f32 alpha = (fabs(targetOffset - m->curAnimOffset) / 80.0f) + 0.5f;
    alpha = (alpha + 0.5f) / 2.0f;
    alpha *= alpha;
    m->curAnimOffset = (m->curAnimOffset * alpha) + (targetOffset * (1.0f - alpha));
    if (m->curAnimOffset > 40)  { m->curAnimOffset = 40; }
    if (m->curAnimOffset < -40) { m->curAnimOffset = -40; }

    if (m->action == ACT_JUMBO_STAR_CUTSCENE) {
        marioObj->header.gfx.pos[1] = m->pos[1] + m->curAnimOffset;
    } else {
        marioObj->header.gfx.pos[1] += m->curAnimOffset;
    }
    marioObj->header.gfx.node.flags |= GRAPH_RENDER_PLAYER;
}