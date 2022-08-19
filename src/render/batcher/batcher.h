#ifndef THREE_D_RENDERER_BATCHER_H
#define THREE_D_RENDERER_BATCHER_H

#include <array>
#include <queue>
#include <map>
#include <stdexcept>
#include "../mesh.h"
#include "bgfx/bgfx.h"

template <class T, size_t size>
struct buffer
{
    std:: array<T, size> content = {};
    size_t elementCount = 0;
};

struct BatchCommand
{
    uint16_t vertexStartIndex;
    uint16_t vertexCount;
    uint16_t indexStartIndex;
    uint16_t indexCount;
    bgfx::ViewId viewId;
    uint64_t state;
    uint8_t shaderID;
    glm::mat4 modelMatrix;
};

struct BatchData
{
    buffer<Vertex, 0xFFFF> vertexBuffer;
    buffer<uint16_t, 0xFFFF> indexBuffer;

    std::queue<BatchCommand> commands;
};

class Batcher
{
public:
    void init (bgfx::VertexLayout* layout);
    void queue (bgfx::ViewId viewID, Mesh mesh, uint64_t state);
    void flush ();
private:
    std::map<unsigned int, BatchData> batchGroups;
    bgfx::VertexLayout* vertexLayout = nullptr;
private:
    static bool canBatch(BatchCommand* a, BatchCommand* b);

    inline BatchData* getBatchGroup (uint16_t shaderID, uint16_t textureID)
    {
        unsigned int batchID = shaderID * textureID;
        const auto& batchGroupEntry = batchGroups.find(batchID);

        if (batchGroupEntry == batchGroups.end())
        {
            BatchData newBatchData;

            if(!batchGroups.insert_or_assign(batchID, std::move(newBatchData)).second)
                throw std::runtime_error("Can't insert batch data");

            return &batchGroups.find(batchID)->second;
        }

        return &batchGroupEntry->second;
    }

    inline void resetBuffers ()
    {
        for (auto& batch : batchGroups)
        {
            batch.second.vertexBuffer.elementCount = 0;
            batch.second.indexBuffer.elementCount = 0;
        }
    }

    static void loadShaders ();
};

#define LOAD_SHADER(NAME) (bgfx::createProgram( \
bgfx::createShader(bgfx::makeRef(NAME ## _vs, sizeof(NAME ## _vs))), \
bgfx::createShader(bgfx::makeRef(NAME ## _fs, sizeof(NAME ## _fs))), true))

#endif //THREE_D_RENDERER_BATCHER_H
