#include <map>
#include "batcher.hpp"

static std::map<uint16_t, bgfx::ProgramHandle> SHADERS;

void Batcher::init(bgfx::VertexLayout* layout) {
    loadShaders();
    vertexLayout = layout;
}

void Batcher::queue(bgfx::ViewId viewID, Mesh mesh, uint64_t state)
{
    BatchData * batchData = getBatchGroup(1, 1);

    // Check if we have enough room in the buffer. If not, flush the buffer to the GPU before adding stuff
    if (mesh.vertices.size() + batchData->vertexBuffer.elementCount >= 0xFFFF
        || mesh.vertexIndices.size() + batchData->indexBuffer.elementCount >= 0xFFFF) {
        flush();
    }

    // Create new batch command
    BatchCommand newCommand{};

    newCommand.vertexStartIndex = batchData->vertexBuffer.elementCount;
    newCommand.vertexCount = mesh.vertices.size();

    newCommand.indexStartIndex = batchData->indexBuffer.elementCount;
    newCommand.indexCount = mesh.vertexIndices.size();

    newCommand.viewId = viewID;
    newCommand.state = state;

    newCommand.shaderID = mesh.shader;
    newCommand.modelMatrix = mesh.modelMatrix;

    // Copy vertex + indices to transient buffer
    for (size_t i = 0; i < newCommand.vertexCount; ++i)
    {
        ++batchData->vertexBuffer.elementCount;
        memcpy(&batchData->vertexBuffer.content[newCommand.vertexStartIndex + i], &mesh.vertices[i], sizeof(Vertex));
    }

    int vertexIndexOffset = 0;
    if (!batchData->commands.empty())
        vertexIndexOffset = canBatch(&batchData->commands.front(), &newCommand) ? newCommand.vertexStartIndex : 0;

    for (size_t i = 0; i < newCommand.indexCount; ++i) {
        batchData->indexBuffer.content[batchData->indexBuffer.elementCount + i] = mesh.vertexIndices[i] + vertexIndexOffset;
    }

    // Push batch command
    batchData->commands.push(newCommand);
}

static bgfx::RendererType::Enum rendererType = bgfx::RendererType::Noop;
static uint64_t vertexWindingDirection = 0;
void Batcher::flush()
{
    bool isBatch = false;
    uint16_t vertexStart  = 0;
    uint16_t vertexCount  = 0;

    uint16_t indiciesStart = 0;
    uint16_t indiciesCount = 0;

    // Set the vertex winding to CCW if we must
    if (rendererType == bgfx::RendererType::Noop)
    {
        rendererType = bgfx::getRendererType();

        switch (rendererType)
        {
            case bgfx::RendererType::Metal:
                vertexWindingDirection = BGFX_STATE_FRONT_CCW;
                break;
            default:
                break;
        }
    }

    for (auto& batch : batchGroups)
    {
        if (batch.second.vertexBuffer.elementCount == 0)
            continue;

        while (!batch.second.commands.empty()) {
            BatchCommand cmd = batch.second.commands.front();
            BatchCommand nextCmd{};
            bool canBatchCmd, hasNextCmd = false;

            batch.second.commands.pop();

            if (!batch.second.commands.empty())
            {
                nextCmd = batch.second.commands.front();
                hasNextCmd = true;
            }

            canBatchCmd = hasNextCmd && canBatch(&cmd, &nextCmd);
            if (!isBatch)
            {
                isBatch = true;
                vertexStart = cmd.vertexStartIndex;
                indiciesStart = cmd.indexStartIndex;

                indiciesCount = vertexCount = 0;
            }

            vertexCount   += cmd.vertexCount;
            indiciesCount += cmd.indexCount;

            // We can no longer batch the command. Send to GPU
            if (!canBatchCmd)
            {
                bgfx::VertexBufferHandle vbh = bgfx::createVertexBuffer(bgfx::makeRef(batch.second.vertexBuffer.content.data() + vertexStart, sizeof(Vertex) * vertexCount), *vertexLayout);
                bgfx::IndexBufferHandle  ibh = bgfx::createIndexBuffer( bgfx::makeRef(batch.second.indexBuffer.content.data() + indiciesStart,  sizeof(uint16_t) * indiciesCount ));

                bgfx::UniformHandle cameraHandle = bgfx::createUniform("u_cameraPos", bgfx::UniformType::Vec4);

                bgfx::setTransform(reinterpret_cast<void*>(&cmd.modelMatrix));
                bgfx::setVertexBuffer(0, vbh);
                bgfx::setIndexBuffer(ibh);
                bgfx::setState(0
                               | BGFX_STATE_DEFAULT
                               | vertexWindingDirection);

                bgfx::submit(cmd.viewId, SHADERS[cmd.shaderID]);

                bgfx::destroy(vbh);
                bgfx::destroy(ibh);
                bgfx::destroy(cameraHandle);

                isBatch = false;
            }
        }
    }

    // Done!
    resetBuffers();
}

bool Batcher::canBatch(BatchCommand *a, BatchCommand *b)
{
    return (a->state == b->state);
}

/// SHADERS
#include "shaders/default/default.fs.hpp"
#include "shaders/default/default.vs.hpp"

/// CONST
constexpr int DEFAULT_SHADER = 0;

// this need to be hand made !
void Batcher::loadShaders()
{
    if(!SHADERS.empty())
        return;

    SHADERS[DEFAULT_SHADER] = LOAD_SHADER(default);
}






