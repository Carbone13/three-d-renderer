#ifndef THREE_D_RENDERER_SURFACE_H
#define THREE_D_RENDERER_SURFACE_H
#include <vector>
#include <cstdint>

typedef uint32_t ABGRColor_t;

typedef struct Vertex
{
    float x, y, z;
    ABGRColor_t color;

    inline static void getVertexLayout (bgfx::VertexLayout* vertexLayout)
    {
        vertexLayout->begin();
        vertexLayout->add(bgfx::Attrib::Position,
                          3, bgfx::AttribType::Float);
        vertexLayout->add(bgfx::Attrib::Color0, 4,
                          bgfx::AttribType::Uint8, true);
        vertexLayout->end();
    }

} Vertex;

typedef std::vector<Vertex> VertexBuffer;
typedef std::vector<uint16_t> VertexBufferIndices;

typedef struct Mesh
{
    VertexBuffer vertices;
    VertexBufferIndices vertexIndices;

    bgfx::VertexBufferHandle getVBH ()
    {
        bgfx::VertexLayout vertexLayout;
        vertexLayout.begin();
        vertexLayout.add(bgfx::Attrib::Position,
                          3, bgfx::AttribType::Float);
        vertexLayout.add(bgfx::Attrib::Color0, 4,
                          bgfx::AttribType::Uint8, true);
        vertexLayout.end();
        return bgfx::createVertexBuffer(bgfx::makeRef(this->vertices.data(), vertices.size() * sizeof(Vertex)), vertexLayout);
    }

    bgfx::IndexBufferHandle getIBH ()
    {
        return bgfx::createIndexBuffer(bgfx::makeRef(this->vertexIndices.data(), vertexIndices.size() * sizeof(uint16_t)));
    }
} Mesh;

#endif //THREE_D_RENDERER_SURFACE_H
