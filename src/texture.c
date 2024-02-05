#include "texture.h"
#include "clm.h"
#include "stbi/stbi_image.h"
#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>
#include <stdbool.h>

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

unsigned int tex_load_texture(const char* texPath) {
    // Construct path to texture.
    size_t texPathLen = strlen(TEXTURE_FOLDER) + strlen(texPath);
    char* path = (char*) malloc((texPathLen * sizeof(char)) + 1);
    strcpy(path, TEXTURE_FOLDER);
    strcat(path, texPath);

    // Load image data.
    stbi_set_flip_vertically_on_load(true);
    int w, h, numChannels;
    unsigned char *data = stbi_load(
            path,
            &w, &h, 
            &numChannels,
            STBI_rgb_alpha); 

    free(path);

    // Create OpenGL texture.
    unsigned int tex = 0;
    if (data) {
        glGenTextures(1, &tex);  
        glBindTexture(GL_TEXTURE_2D, tex);
        glTexImage2D(
                GL_TEXTURE_2D, 
                0, 
                GL_RGBA, 
                w, h, 
                0, 
                GL_RGBA, 
                GL_UNSIGNED_BYTE, 
                data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
    } else {
        fprintf(stderr, "Failed to load image with stbi_image\n");
    }

    return tex;
}
