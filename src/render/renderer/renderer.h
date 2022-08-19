#ifndef THREE_D_RENDERER_RENDERER_H
#define THREE_D_RENDERER_RENDERER_H

#include "../mesh.h"
#include "render/batcher/batcher.h"

class Renderer
{
public:
    void init ();
    void render (Mesh mesh);
    void present();

    void prepare () { viewID = 0; }
private:
    uint8_t viewID{};
    Batcher batcher{};
    bgfx::VertexLayout vertexLayout;
};

#endif //THREE_D_RENDERER_RENDERER_H
