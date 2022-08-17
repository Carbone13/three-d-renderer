#include "dbg_colors.h"
#include <iostream>

#include "shader.h"

#include "shaders/default/default.fs.h"
#include "shaders/default/default.vs.h"

void Shader::load(int shader)
{
    switch (shader)
    {
        case DEFAULT_SHADER:
        {
            auto vs = bgfx::createShader(bgfx::makeRef(default_vs, sizeof(default_vs)));
            auto fs = bgfx::createShader(bgfx::makeRef(default_fs, sizeof(default_fs)));
            program = bgfx::createProgram(vs, fs, true);
            std::cout << FGRN("[LOADED] Shader 'DEFAULT'") << std::endl;
            break;
        }
        default:
        {
            std::cout << FRED("[LOAD_ERROR] Could not load Shader with id:" + std::to_string(shader) +) << std::endl;
        }
    }
}

Shader::Shader() = default;
