#include "switchrenderer.h"

#include "lamprenderer.h"

void SwitchRenderer::renderSpecialBlock(const BLOCK_WDATA block, GLFix x, GLFix y, GLFix z, Chunk &c)
{
    const TextureAtlasEntry &tex = getPOWERSTATE(block) ? terrain_atlas[10][15].current : terrain_atlas[0][6].current;

    glPushMatrix();
    glLoadIdentity();

    glTranslatef(x + BLOCK_SIZE/2, y + BLOCK_SIZE/2, z + BLOCK_SIZE/2);

    std::vector<VERTEX> switch_vertices;
    switch_vertices.reserve(16);

    switch_vertices.push_back({0, -BLOCK_SIZE/4, BLOCK_SIZE/2, tex.left, tex.bottom, TEXTURE_TRANSPARENT | TEXTURE_DRAW_BACKFACE});
    switch_vertices.push_back({0, BLOCK_SIZE - BLOCK_SIZE/4, BLOCK_SIZE/2, tex.left, tex.top, TEXTURE_TRANSPARENT | TEXTURE_DRAW_BACKFACE});
    switch_vertices.push_back({BLOCK_SIZE, BLOCK_SIZE - BLOCK_SIZE/3, BLOCK_SIZE/2, tex.right, tex.top, TEXTURE_TRANSPARENT | TEXTURE_DRAW_BACKFACE});
    switch_vertices.push_back({BLOCK_SIZE, -BLOCK_SIZE/4, BLOCK_SIZE/2, tex.right, tex.bottom, TEXTURE_TRANSPARENT | TEXTURE_DRAW_BACKFACE});

    switch_vertices.push_back({BLOCK_SIZE/2, -BLOCK_SIZE/4, BLOCK_SIZE, tex.left, tex.bottom, TEXTURE_TRANSPARENT | TEXTURE_DRAW_BACKFACE});
    switch_vertices.push_back({BLOCK_SIZE/2, BLOCK_SIZE-BLOCK_SIZE/4, BLOCK_SIZE, tex.left, tex.top, TEXTURE_TRANSPARENT | TEXTURE_DRAW_BACKFACE});
    switch_vertices.push_back({BLOCK_SIZE/2, BLOCK_SIZE-BLOCK_SIZE/4, 0, tex.right, tex.top, TEXTURE_TRANSPARENT | TEXTURE_DRAW_BACKFACE});
    switch_vertices.push_back({BLOCK_SIZE/2, -BLOCK_SIZE/4, 0, tex.right, tex.bottom, TEXTURE_TRANSPARENT | TEXTURE_DRAW_BACKFACE});

    const BLOCK_SIDE side = static_cast<BLOCK_SIDE>(getBLOCKDATA(block));

    switch(side)
    {
    default:
    case BLOCK_TOP:
        break;
    case BLOCK_BOTTOM:
        nglRotateX(180);
        break;
    case BLOCK_BACK:
        nglRotateX(90);
        break;
    case BLOCK_FRONT:
        nglRotateX(270);
        break;
    case BLOCK_LEFT:
        nglRotateZ(90);
        break;
    case BLOCK_RIGHT:
        nglRotateZ(270);
        break;
    }

    glTranslatef(0, -BLOCK_SIZE/2, 0);

    if(side == BLOCK_LEFT || side == BLOCK_RIGHT)
    {
        if(getPOWERSTATE(block))
            nglRotateZ(40);
        else
            nglRotateZ(320);
    }
    else
    {
        if(getPOWERSTATE(block))
            nglRotateX(40);
        else
            nglRotateX(320);
    }

    glTranslatef(-BLOCK_SIZE / 2, 0, -BLOCK_SIZE / 2);

    for(auto&& v : switch_vertices)
    {
        VERTEX v1;
        nglMultMatVectRes(transformation, &v, &v1);
        c.addUnalignedVertex(v1.x, v1.y, v1.z, v.u, v.v, v.c);
    }

    glPopMatrix();
}

void SwitchRenderer::drawPreview(const BLOCK_WDATA /*block*/, TEXTURE &dest, int x, int y)
{
    return BlockRenderer::drawTextureAtlasEntry(*terrain_resized, terrain_atlas[0][6].resized, dest, x, y);
}

bool SwitchRenderer::action(const BLOCK_WDATA block, const int local_x, const int local_y, const int local_z, Chunk &c)
{
    c.setLocalBlock(local_x, local_y, local_z, getBLOCKWDATAPower(block, getBLOCKDATA(block), !getPOWERSTATE(block)));

    return true;
}

const char *SwitchRenderer::getName(const BLOCK_WDATA)
{
    return "Redstone Switch";
}
