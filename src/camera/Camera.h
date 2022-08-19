#ifndef THREE_D_RENDERER_CAMERA_H
#define THREE_D_RENDERER_CAMERA_H

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/scalar_constants.hpp>

#include <bgfx/bgfx.h>

class Camera
{
private:
    glm::mat4 view;
    glm::mat4 proj;

    glm::vec3 position;
    glm::vec2 rotation;

    glm::vec3 oldPosition;
    glm::vec2 oldRotation;

    glm::vec3 forward, right, up;

    void recalculate();
public:
    // GETTERS
    [[nodiscard]] glm::vec3 getPosition() const { return position; }
    [[nodiscard]] glm::vec2 getRotation () const { return rotation; }
    [[nodiscard]] glm::vec3 getForward () const { return forward; }
    [[nodiscard]] glm::vec3 getRight () const { return right; }
    [[nodiscard]] glm::vec3 getUp () const { return up; }

    // SETTERS
    void setPosition (glm::vec3 _position);
    void setRotation (glm::vec2 _rotation);

    void prepare ();
    Camera ()
    {
        proj = glm::perspective(glm::pi<float>() * 0.5f, 16.0f / 9.0f, 0.1f, 100.0f);
    }
};

#endif //THREE_D_RENDERER_CAMERA_H
