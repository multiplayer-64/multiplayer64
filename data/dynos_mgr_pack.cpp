#include "dynos.cpp.h"
extern "C" {
#include "engine/graph_node.h"
}

static Array<PackData>& DynosPacks() {
    static Array<PackData> sDynosPacks;
    return sDynosPacks;
}

static void ScanPackBins(struct PackData* aPack) {
    DIR *_PackDir = opendir(aPack->mPath.c_str());
    if (!_PackDir) { return; }

    struct dirent *_PackEnt = NULL;
    while ((_PackEnt = readdir(_PackDir)) != NULL) {
        // Skip . and ..
        if (SysPath(_PackEnt->d_name) == ".") continue;
        if (SysPath(_PackEnt->d_name) == "..") continue;

        SysPath _FileName = fstring("%s/%s", aPack->mPath.c_str(), _PackEnt->d_name);
        s32 length = strlen(_PackEnt->d_name);

        // check for actors
        if (length > 4 && !strncmp(&_PackEnt->d_name[length - 4], ".bin", 4)) {
            String _ActorName = _PackEnt->d_name;
            _ActorName[length - 4] = '\0';
            DynOS_Actor_LoadFromBinary(aPack->mPath, strdup(_ActorName.begin()), _FileName, true);
        }

        // check for textures
        if (length > 4 && !strncmp(&_PackEnt->d_name[length - 4], ".tex", 4)) {
            String _TexName = _PackEnt->d_name;
            _TexName[length - 4] = '\0';
            DynOS_Tex_LoadFromBinary(aPack->mPath, _FileName, _TexName.begin(), true);
        }
    }
}

static void DynOS_Pack_ActivateActor(s32 aPackIndex, Pair<const char *, GfxData *>& pair) {
    const char* aActorName = pair.first;
    GfxData* aGfxData = pair.second;

    auto& geoNode = *(aGfxData->mGeoLayouts.end() - 1);
    GraphNode* graphNode = (GraphNode *) DynOS_Geo_GetGraphNode(geoNode->mData, false);
    if (graphNode == NULL) { return; }

    const void* georef = DynOS_Builtin_Actor_GetFromName(aActorName);
    graphNode->georef = georef;

    ActorGfx actorGfx;
    actorGfx.mGfxData   = aGfxData;
    actorGfx.mGraphNode = graphNode;
    actorGfx.mPackIndex = aPackIndex;

    if (geoNode->mFlags & GRAPH_EXTRA_FORCE_3D) {
        actorGfx.mGraphNode->extraFlags |= GRAPH_EXTRA_FORCE_3D;
    }

    DynOS_Actor_Valid(georef, actorGfx);
}

static void DynOS_Pack_DeactivateActor(s32 aPackIndex, Pair<const char *, GfxData *>& pair) {
    const char* aActorName = pair.first;
    const void* georef = DynOS_Builtin_Actor_GetFromName(aActorName);
    DynOS_Actor_Invalid(georef, aPackIndex);

    // figure out which actor to replace it with
    Pair<const char *, GfxData *>* _Replacement = NULL;
    s32 _ReplacementPackIndex = 0;
    for (auto& _Pack : DynosPacks()) {
        if (!_Pack.mEnabled) { continue; }
        auto _Tmp = DynOS_Pack_GetActor(&_Pack, aActorName);
        if (_Tmp != NULL) {
            _Replacement = _Tmp;
            _ReplacementPackIndex = _Pack.mIndex;
        }
    }
    if (_Replacement != NULL) {
        DynOS_Pack_ActivateActor(_ReplacementPackIndex, *_Replacement);
    }
}

s32 DynOS_Pack_GetCount() {
    return DynosPacks().Count();
}

void DynOS_Pack_SetEnabled(PackData* aPack, bool aEnabled) {
    if (aPack == NULL) { return; }
    aPack->mEnabled = aEnabled;
    aPack->mEnabledSet = true;

    if (aEnabled && !aPack->mLoaded) {
        ScanPackBins(aPack);
        aPack->mLoaded = true;
    }

    if (aEnabled) {
        for (auto& pair : aPack->mGfxData) {
            DynOS_Pack_ActivateActor(aPack->mIndex, pair);
        }
        for (auto& _Tex : aPack->mTextures) {
            DynOS_Tex_Activate(_Tex, false);
        }
    } else {
        for (auto& pair : aPack->mGfxData) {
            DynOS_Pack_DeactivateActor(aPack->mIndex, pair);
        }
        for (auto& _Tex : aPack->mTextures) {
            DynOS_Tex_Deactivate(_Tex);
        }
    }
    DynOS_Actor_Override_All();
}

PackData* DynOS_Pack_GetFromIndex(s32 aIndex) {
    auto& _DynosPacks = DynosPacks();
    if (aIndex < 0 || aIndex >= _DynosPacks.Count()) {
        return NULL;
    }
    return &_DynosPacks[aIndex];
}

PackData* DynOS_Pack_GetFromPath(const SysPath& aPath) {
    for (auto& packData : DynosPacks()) {
        if (packData.mPath == aPath) {
            return &packData;
        }
    }
    return NULL;
}

PackData* DynOS_Pack_Add(const SysPath& aPath) {
    PackData* existing = DynOS_Pack_GetFromPath(aPath);
    if (existing != NULL) { return existing; }

    auto& _DynosPacks = DynosPacks();
    s32 index = _DynosPacks.Count();
    const PackData packData = {
        .mIndex = index,
        .mPath = aPath,
        .mGfxData = {},
        .mTextures = {},
        .mLoaded = false,
    };
    _DynosPacks.Add(packData);

    PackData* _Pack = &_DynosPacks[index];

    // extract basename
    const char* cpath = aPath.c_str();
    const char* ctoken = cpath;
    while (*ctoken != '\0') {
        if (*ctoken == '/' || *ctoken == '\\') {
            if (*(ctoken + 1) != '\0') {
                cpath = (ctoken + 1);
            }
        }
        ctoken++;
    }
    _Pack->mDisplayName = cpath;

    _Pack->mEnabled = true;
    _Pack->mEnabledSet = false;

    return _Pack;
}

void DynOS_Pack_Init() {
    for (auto& pack : DynosPacks()) {
        if (!pack.mEnabledSet) {
            DynOS_Pack_SetEnabled(&pack, pack.mEnabled);
        }
    }
}

Pair<const char *, GfxData *>* DynOS_Pack_GetActor(PackData* aPackData, const char* aActorName) {
    if (aPackData == NULL || aActorName == NULL) {
        return NULL;
    }
    for (auto& pair : aPackData->mGfxData) {
        if (!strcmp(pair.first, aActorName)) {
            return &pair;
        }
    }
    return NULL;
}

void DynOS_Pack_AddActor(PackData* aPackData, const char* aActorName, GfxData* aGfxData) {
    if (aPackData == NULL || aActorName == NULL || aGfxData == NULL) {
        return;
    }

    s32 index = aPackData->mGfxData.Count();
    aPackData->mGfxData.Add({ aActorName, aGfxData });

    if (aPackData->mEnabled) {
        DynOS_Pack_ActivateActor(aPackData->mIndex, aPackData->mGfxData[index]);
    }
}

DataNode<TexData>* DynOS_Pack_GetTex(PackData* aPackData, const char* aTexName) {
    if (aPackData == NULL || aTexName == NULL) {
        return NULL;
    }

    for (auto& _Tex : aPackData->mTextures) {
        if (!strcmp(_Tex->mName.begin(), aTexName)) {
            return _Tex;
        }
    }
    return NULL;
}

void DynOS_Pack_AddTex(PackData* aPackData, DataNode<TexData>* aTexData) {
    if (aPackData == NULL || aTexData == NULL) {
        return;
    }

    aPackData->mTextures.Add(aTexData);

    if (aPackData->mEnabled) {
        DynOS_Tex_Activate(aTexData, false);
    }
}
