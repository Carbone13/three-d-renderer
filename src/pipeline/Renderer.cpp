//
// Created by lucas on 17/08/2022.
//

#include <stdexcept>
#include "renderer.h"
#include "bgfx/bgfx.h"

void Renderer::setViewMatrix(const bx::Vec3 &eye, const bx::Vec3 &at, const bx::Vec3 &up)
{
    const bx::Vec3 _eye(eye.x, eye.y, eye.z);
    const bx::Vec3 _at(at.x, at.y, at.z);
    const bx::Vec3 _up(up.x, up.y, up.z);
    bx::mtxLookAt(viewMtx, _eye, _at, _up);
    hasViewMtx = true;
}

void Renderer::setProjectionMatrix(float fov, float aspect, float near, float far)
{
    bx::mtxProj(projMtx, fov, aspect, near, far, bgfx::getCaps()->homogeneousDepth);
    hasProjMtx = true;
}

void Renderer::setViewport(float x, float y, float w, float h)
{
    viewport = { x, y, w, h };
    hasViewport = true;
}

void Renderer::prepare()
{
    bgfx::setViewRect(viewId, uint16_t(viewport.x), uint16_t(viewport.y), uint16_t(viewport.w), uint16_t(viewport.h) );

    if (!hasInit)
        init();

    if (!(hasViewMtx && hasProjMtx && hasViewport))
        throw std::runtime_error("Rendering to uninitialized view");

    // Cache V * P
    bx::mtxMul(vpMtx, viewMtx, projMtx);
    bx::mtxInverse(vpMtx_inv, vpMtx);

    bgfx::setViewTransform(viewId, viewMtx, projMtx);
    bgfx::setViewMode(viewId, bgfx::ViewMode::Default);
}

void Renderer::init() {

}
