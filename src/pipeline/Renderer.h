//
// Created by lucas on 17/08/2022.
//

#ifndef THREE_D_RENDERER_RENDERER_H
#define THREE_D_RENDERER_RENDERER_H

#include <cstdint>
#include "bx/math.h"

class Renderer {
public:
    void setViewMatrix (const bx::Vec3& eye, const bx::Vec3& at, const bx::Vec3& up = { 0, 1, 0 });
    void setProjectionMatrix (float fov, float aspect, float near, float far);
    void setViewport (float x, float y, float w, float h);
    void prepare ();
private:
    bool hasViewMtx, hasProjMtx, hasViewport = false;
    bool hasInit = false;

    float viewMtx[16];
    float projMtx[16];

    /// Use this to cache the result of viewMtx * projMtx
    float vpMtx[16]; // v * p
    float vpMtx_inv[16]; // inverse of vp

    struct {
        float x, y, w, h;
    } viewport;

    uint8_t viewId;

private:
    void init();
};


#endif //THREE_D_RENDERER_RENDERER_H
