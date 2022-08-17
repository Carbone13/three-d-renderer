#ifndef THREE_D_RENDERER_CAMERA_H
#define THREE_D_RENDERER_CAMERA_H
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/scalar_constants.hpp>

#include <bgfx/bgfx.h>

class Camera {
private:
    glm::mat4 view;
    glm::mat4 proj;

    glm::vec3 position;
    glm::vec2 rotation;

    glm::vec3 forward, right, up;

    void recalculate();

public:
    // GETTERS
    glm::vec3 getPosition() const { return position; }
    glm::vec2 getRotation () const { return rotation; }
    glm::vec3 getForward () const { return forward; }
    glm::vec3 getRight () const { return right; }
    glm::vec3 getUp () const { return up; }

    // SETTERS
    void setPosition (glm::vec3 _position);
    void setRotation (glm::vec2 _rotation);

    void prepare ();
};

#endif //THREE_D_RENDERER_CAMERA_H
