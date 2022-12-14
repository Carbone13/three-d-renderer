#ifndef THREE_D_RENDERER_VERTEX_HPP
#define THREE_D_RENDERER_VERTEX_HPP

#include <cstdint>
#include "bgfx/bgfx.h"

typedef uint32_t ABGRColor_t;

struct Vertex
{
    float x, y, z;
    ABGRColor_t color {0xFFFFFFFF};

    inline static void getVertexLayout (bgfx::VertexLayout* vertexLayout)
    {
        vertexLayout->begin();
        vertexLayout->add(bgfx::Attrib::Position,
                          3, bgfx::AttribType::Float);
        vertexLayout->add(bgfx::Attrib::Color0, 4,
                          bgfx::AttribType::Uint8, true);
        vertexLayout->end();
    }
};

#endif //THREE_D_RENDERER_VERTEX_HPP
