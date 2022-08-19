#ifndef THREE_D_RENDERER_MESH_H
#define THREE_D_RENDERER_MESH_H

#include <vector>
#include "vertex.h"
#include "glm/glm.hpp"

typedef std::vector<Vertex> VertexBuffer;
typedef std::vector<uint16_t> VertexBufferIndices;

struct Mesh
{
public:
    VertexBuffer vertices;
    VertexBufferIndices vertexIndices;

    glm::mat4 modelMatrix {glm::mat4(1.0f)};

    int shader;
};

#endif //THREE_D_RENDERER_MESH_H
