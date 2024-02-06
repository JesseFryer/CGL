#ifndef TEXTURE_H
#define TEXTURE_H

/*
 * texture.h
 *
 */

#define TEXTURE_FOLDER "../res/"

typedef struct SpriteAtlas {
    // Total texture size in pixels.
    unsigned int atlasPixW;
    unsigned int atlasPixH;
    // Total texture size in sprites.
    unsigned int atlasSprW;
    unsigned int atlasSprH;
    // Pixel dimensions of each sprite within the atlas.
    unsigned int spriteW;
    unsigned int spriteH;
    // Normalized dimensions of each sprite.
    float normSpriteW;
    float normSpriteH;
} SpriteAtlas;

typedef struct VoxelTex {
    // x/y normalized coordinates of the bottom left pixel of the
    // top/bottom/side textures for the voxel.
    float top[2];
    float bottom[2];
    float side[2];
    // Normalized dimensions of each sprite within its atlas.
    float normSpriteDim[2];
} VoxelTex;

/* tex_init_atlas
 * --------------
 * Store atlas info in atlas and compute normalized sprite dimensions.
 */
void tex_init_atlas(
        SpriteAtlas* atlas, 
        unsigned int atlasW,
        unsigned int atlasH,
        unsigned int spriteW,
        unsigned int spriteH);

/* tex_create_voxel_tex
 * --------------------
 * Fill voxTex with the texture information for the sprites specified by
 * the top/bottom/side indices.
 */
void tex_create_voxel_tex(
        VoxelTex* voxTex,
        SpriteAtlas* atlas,
        unsigned int topIndex,
        unsigned int bottomIndex,
        unsigned int sideIndex);

/* tex_load_texture
 * ----------------
 * Load an OpenGL texture and receive its id handle. (png only atm)
 */
unsigned int tex_load_texture(const char* texPath);

#endif
