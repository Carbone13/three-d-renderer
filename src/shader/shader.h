#ifndef THREE_D_RENDERER_SHADER_H
#define THREE_D_RENDERER_SHADER_H
#include <bgfx/bgfx.h>
constexpr int DEFAULT_SHADER = 0;

typedef struct Shader {
    Shader();

    bgfx::ProgramHandle program{};

    void load (int shader);
} Shader;
#endif //THREE_D_RENDERER_SHADER_H