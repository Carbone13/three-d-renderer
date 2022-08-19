#include "renderer.hpp"

#include <utility>

void Renderer::init() {
    Vertex::getVertexLayout(&vertexLayout);
    batcher.init(&vertexLayout);
}

void Renderer::render(Mesh mesh)
{
    batcher.queue(0, mesh, viewID);
    ++viewID;
}

void Renderer::present()
{
    batcher.flush();
    // TODO why do we use that ?
    //++viewID;
}