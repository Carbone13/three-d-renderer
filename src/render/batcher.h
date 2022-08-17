#ifndef THREE_D_RENDERER_BATCHER_H
#define THREE_D_RENDERER_BATCHER_H

#include "mesh.h"
#include "bgfx/bgfx.h"
#include "shader/shader.h"

#define LOAD_SHADER(NAME) (bgfx::createProgram( \
bgfx::createShader(bgfx::makeRef(NAME ## _vs, sizeof(NAME ## _vs))), \
bgfx::createShader(bgfx::makeRef(NAME ## _fs, sizeof(NAME ## _fs))), true))


struct BatchCommand {
    uint16_t vertexStartIndex;
    uint16_t vertexCount;
    uint16_t indexStartIndex;
    uint16_t indexCount;
    bgfx::ViewId viewId;
    uint64_t state;
    uint8_t shaderID;
};

class Batcher {
public:
    void add (bgfx::ViewId, Mesh mesh);
    void flush ();
    Batcher ();
private:
    void loadShaders ();
};


#endif //THREE_D_RENDERER_BATCHER_H
