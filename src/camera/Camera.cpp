#include "Camera.h"

void Camera::setPosition(glm::vec3 _position)
{
    position = _position;
    recalculate();
}

void Camera::setRotation(glm::vec2 _rotation)
{
    rotation = _rotation;
    recalculate();
}

void Camera::recalculate()
{
    proj = glm::perspective(glm::pi<float>() * 0.5f, 16.0f / 9.0f, 0.1f, 100.0f);

    view = glm::rotate(glm::mat4(1.0f), rotation.y, glm::vec3(1.0f, 0.0f, 0.0f));
    view = glm::rotate(view, rotation.x, glm::vec3(0.0f, 1.0f, 0.0f));
    view = glm::translate(view, position);

    right = glm::vec3 { view[0][0], view[1][0], view[2][0] };
    up = glm::vec3 { view[0][1], view[1][1], view[2][1] };
    forward = glm::vec3 { view[0][2], view[1][2], view[2][2] };
}

void Camera::prepare() {
    bgfx::setViewTransform(0,
            reinterpret_cast<void *>(&view),
            reinterpret_cast<void *>(&proj));
}
