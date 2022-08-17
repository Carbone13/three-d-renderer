

#include <main.h>
#include <iostream>

#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3.h"
#include "GLFW/glfw3native.h"

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
#include "glm/ext.hpp"


#include "bgfx/bgfx.h"
#include "bgfx/platform.h"

#include "shader/shader.h"
#include "mesh/surface.h"
#include "camera/Camera.h"

#define WIDTH 1280
#define HEIGHT 720

GLFWwindow* window;
Camera camera;

Shader mainShader;
Mesh demoMesh;

double mouseX, mouseY;

static void debugGlfwError (int err, const char *msg)
{
    std::cerr << "GLFW ERROR (" << err << "): " << msg << std::endl;
}

int main()
{
    glfwSetErrorCallback(debugGlfwError);

    if (!glfwInit()) {
        std::cerr << "Error initializing GLFW" << std::endl;
        return -1;
    }

    window = glfwCreateWindow(WIDTH, HEIGHT, "3D Renderer", nullptr, nullptr);

    if (!window) {
        std::cerr << "Error creating window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);


    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (glfwRawMouseMotionSupported())
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

    bgfx::PlatformData pd;
    bgfx::Init bgfxInit;
    pd.nwh = glfwGetWin32Window(window);
    bgfxInit.type = bgfx::RendererType::Count; // Automatically choose a renderer.
    bgfxInit.resolution.width = WIDTH;
    bgfxInit.resolution.height = HEIGHT;
    bgfxInit.resolution.reset = BGFX_RESET_VSYNC;
    bgfxInit.platformData = pd;
    bgfx::init(bgfxInit);

    bgfx::reset(WIDTH, HEIGHT, BGFX_RESET_VSYNC, bgfxInit.resolution.format);
    bgfx::setDebug(BGFX_DEBUG_TEXT);
    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x003030FF, 1.0f, 0);
    bgfx::setViewRect(0, 0, 0, bgfx::BackbufferRatio::Equal);

    load();

    camera.setPosition(glm::vec3{0.0, 0.0, -7.0});
    //camera.set_rotation(glm::vec2{60.0, -120.0});

    double lastTime = glfwGetTime();
    while (!glfwWindowShouldClose(window))
    {
        double dt = glfwGetTime() - lastTime;
        lastTime = glfwGetTime();

        input();

        update(static_cast<float>(dt));

        bgfx::touch(0);
        bgfx::dbgTextClear();
        render();
        bgfx::frame();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}

void input ()
{
    glfwPollEvents();

    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, 1);
    }
}

void render ()
{
    bgfx::dbgTextPrintf(0, 0, ((0x2 + 1) << 4) | 0xF, "Hello, world");
    bgfx::dbgTextPrintf(0, 1, ((0x2) << 4) | 0xF, std::to_string(glfwGetTime()).c_str());

    camera.prepare();
    bgfx::setViewRect(0, 0, 0, WIDTH, HEIGHT);

    glm::mat4 model = glm::translate(glm::mat4(1.0), glm::vec3(0, 0, 0));
    bgfx::setTransform(reinterpret_cast<void *>(&model));

    bgfx::setVertexBuffer(0, demoMesh.getVBH());
    bgfx::setIndexBuffer(demoMesh.getIBH());

    bgfx::submit(0, mainShader.program);
}

void update (float delta)
{
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.setPosition(camera.getPosition() + camera.getForward() * 10.0f * delta);
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.setPosition(camera.getPosition() - camera.getForward() * 10.0f * delta);
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.setPosition(camera.getPosition() + camera.getRight() * 10.0f * delta);
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.setPosition(camera.getPosition() - camera.getRight() * 10.0f * delta);
    if(glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        camera.setPosition(camera.getPosition() + camera.getUp() * 10.0f * delta);
    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.setPosition(camera.getPosition() - camera.getUp() * 10.0f * delta);

    float oldXPos = (float)mouseX, oldYPos = (float)mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);
    float xDelta, yDelta;
    xDelta = (float)mouseX - oldXPos;
    yDelta = (float)mouseY - oldYPos;

    camera.setRotation(camera.getRotation() + glm::vec2{0.1f * xDelta * delta, 0.1f * yDelta * delta});
}


void load ()
{
    demoMesh.vertices = {
            {-1.0f,  1.0f,  1.0f, 0xff000000 },
            { 1.0f,  1.0f,  1.0f, 0xff0000ff },
            {-1.0f, -1.0f,  1.0f, 0xff00ff00 },
            { 1.0f, -1.0f,  1.0f, 0xff00ffff },
            {-1.0f,  1.0f, -1.0f, 0xffff0000 },
            { 1.0f,  1.0f, -1.0f, 0xffff00ff },
            {-1.0f, -1.0f, -1.0f, 0xffffff00 },
            { 1.0f, -1.0f, -1.0f, 0xffffffff },
    };

    demoMesh.vertexIndices = {
            0, 2, 1,
            1, 2, 3,
            4, 5, 6,
            5, 7, 6,
            0, 4, 2,
            4, 6, 2,
            1, 3, 5,
            5, 3, 7,
            0, 1, 4,
            4, 1, 5,
            2, 6, 3,
            6, 7, 3,
    };

    mainShader.load(DEFAULT_SHADER);
}