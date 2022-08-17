#include <map>
#include "batcher.h"

#include "shaders/default/default.fs.h"
#include "shaders/default/default.vs.h"

static std::map<uint16_t, bgfx::ProgramHandle> SHADERS;

Batcher::Batcher()
{
    loadShaders();
}

// this need to be hand made ! but can easily be made by cmake
void Batcher::loadShaders()
{
    SHADERS[0] = LOAD_SHADER(default);
}
