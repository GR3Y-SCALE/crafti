#ifndef CHUNK_H
#define CHUNK_H

#include <cstdint>
#include <vector>

#include "gl.h"
#include "block_data.h"
#include "aabb.h"

class World;

struct Position {
    GLFix x, y, z;
};

struct IndexedVertex {
    int pos;
    GLFix u, v;
    COLOR c;
};

class Chunk
{
public:
    Chunk(int x, int y, int z, World *parent);
    void render();
    void setDirty() { render_dirty = true; }
    BLOCK_WDATA getLocalBlock(const int x, const int y, const int z);
    void setLocalBlock(const int x, const int y, const int z, const BLOCK_WDATA block);
    AABB &getAABB() { return aabb; }
    bool intersects(AABB &other);
    bool intersectsRay(GLFix x, GLFix y, GLFix z, GLFix dx, GLFix dy, GLFix dz, GLFix &dist, Position &pos, AABB::SIDE &side);
    void generate();
    bool saveToFile(FILE *file);
    bool loadFromFile(FILE *file);

    static constexpr int SIZE = 8;

    const int x, y, z;

private:
    //Terrain generation
    void makeTree(unsigned int x, unsigned int y, unsigned int z);

    //Data
    BLOCK_WDATA getGlobalBlockRelative(int x, int y, int z);
    void setGlobalBlockRelative(int x, int y, int z, BLOCK_WDATA block);
    int getPosition(GLFix x, GLFix y, GLFix z);

    //Rendering
    void addAlignedVertex(const GLFix x, const GLFix y, const GLFix z, GLFix u, GLFix v, const COLOR c);
    int indicesSpecialBlock(BLOCK_WDATA block, unsigned int x, unsigned int y, unsigned int z, BLOCK_SIDE side);
    void geometrySpecialBlock(BLOCK_WDATA block, unsigned int x, unsigned int y, unsigned int z, BLOCK_SIDE side);
    void buildGeometry();
    VERTEX perspective(const IndexedVertex &v, Position &transformed);
    bool drawTriangle(const IndexedVertex &low, const IndexedVertex &middle, const IndexedVertex &high, bool backface_culling = true);

    //Data
    const GLFix abs_x, abs_y, abs_z;
    World * const parent;
    AABB aabb;
    BLOCK_WDATA blocks[SIZE][SIZE][SIZE];

    //Rendering
    bool render_dirty = true;
    int pos_indices[SIZE + 1][SIZE + 1][SIZE + 1];
    std::vector<Position> positions, positions_transformed;
    std::vector<std::pair<Position, bool>> positions_perspective;
    std::vector<IndexedVertex> vertices;
    std::vector<VERTEX> vertices_not_aligned; //The optimized drawing with indices doesn't work with unaligned positions
};

#endif // CHUNK_H
