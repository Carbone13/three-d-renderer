#ifndef THREE_D_RENDERER_RENDERER_HPP
#define THREE_D_RENDERER_RENDERER_HPP

#include "../mesh.hpp"
#include "render/batcher/batcher.hpp"

class Renderer
{
public:
    void init ();
    void render (Mesh mesh);
    void present();

   // void prepare () { viewID = 0; }
private:
    uint8_t viewID{};
    Batcher batcher{};
    bgfx::VertexLayout vertexLayout;
};

#endif //THREE_D_RENDERER_RENDERER_HPP
