#include "renderer.hpp"

#include <utility>

void Renderer::init() {
    Vertex::getVertexLayout(&vertexLayout);
    batcher.init(&vertexLayout);
}

void Renderer::render(Mesh mesh)
{
    batcher.queue(viewID, mesh, 0);
}

void Renderer::present()
{
    batcher.flush();
    // TODO why do we use that ?
    ++viewID;
}