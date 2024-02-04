#include "texture.h"
#include "clm.h"
#include <stdio.h>

void tex_init_atlas(SpriteAtlas* atlas, 
        unsigned int atlasPixW,
        unsigned int atlasPixH,
        unsigned int spriteW,
        unsigned int spriteH) {
    atlas->atlasPixW = atlasPixW;
    atlas->atlasPixH = atlasPixH;

    atlas->spriteW = spriteW;
    atlas->spriteH = spriteH;

    atlas->atlasSprW = atlasPixW / spriteW;
    atlas->atlasSprH = atlasPixH / spriteH;

    atlas->normSpriteW = ((float) spriteW) / ((float) atlasPixW);
    atlas->normSpriteH = ((float) spriteH) / ((float) atlasPixH);
}

void tex_create_voxel_tex(
        VoxelTex* voxTex,
        SpriteAtlas* atlas,
        unsigned int topIndex,
        unsigned int bottomIndex,
        unsigned int sideIndex) {
    voxTex->normSpriteDim[0] = atlas->normSpriteW;
    voxTex->normSpriteDim[1] = atlas->normSpriteH;
    float sprW = atlas->normSpriteW;
    float sprH = atlas->normSpriteH;
    voxTex->top[0] = (topIndex % atlas->atlasSprW) * sprW;
    voxTex->top[1] = (topIndex / atlas->atlasSprW) * sprH;
    voxTex->bottom[0] = (bottomIndex % atlas->atlasSprW) * sprW;
    voxTex->bottom[1] = (bottomIndex / atlas->atlasSprW) * sprH;
    voxTex->side[0] = (sideIndex % atlas->atlasSprW) * sprW;
    voxTex->side[1] = (sideIndex / atlas->atlasSprW) * sprH;
}
