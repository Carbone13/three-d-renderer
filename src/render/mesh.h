//
// Created by lucas on 18/08/2022.
//

#ifndef THREE_D_RENDERER_MESH_H
#define THREE_D_RENDERER_MESH_H

#include <vector>
#include "vertex.h"

typedef std::vector<Vertex> VertexBuffer;
typedef std::vector<uint16_t> VertexBufferIndices;

class Mesh {
public:
    VertexBuffer vertices;
    VertexBufferIndices vertexIndices;

    int shader;
};


#endif //THREE_D_RENDERER_MESH_H
