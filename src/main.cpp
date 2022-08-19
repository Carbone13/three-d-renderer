#include <main.hpp>
#include <iostream>

#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3.h"
#include "GLFW/glfw3native.h"

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"

#include "bgfx/bgfx.h"

#include "camera/camera.hpp"
#include "render/renderer/renderer.hpp"

#define WIDTH 1600
#define HEIGHT 900

GLFWwindow* window;

Renderer renderer;
Camera camera {};

Mesh demoMesh;
Mesh demoMesh2;

double mouseX, mouseY;

int main()
{
    glfwInit();

    window = glfwCreateWindow(WIDTH, HEIGHT, "3D Renderer", nullptr, nullptr);

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
    renderer.init();

    double lastTime = glfwGetTime();
    while (!glfwWindowShouldClose(window))
    {
        double dt = glfwGetTime() - lastTime;
        lastTime = glfwGetTime();

        input();
        update(static_cast<float>(dt));

        bgfx::touch(0);
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
    //renderer.prepare();
    camera.prepare();

    renderer.render(demoMesh);
    renderer.render(demoMesh2);

    renderer.present();
}

void update (float delta)
{
    auto camPos = camera.getPosition();

    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camPos += camera.getForward() * 10.0f * delta;
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camPos -= camera.getForward() * 10.0f * delta;
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camPos += camera.getRight() * 10.0f * delta;
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camPos -= camera.getRight() * 10.0f * delta;
    if(glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        camPos +=  camera.getUp() * 10.0f * delta;
    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camPos -= camera.getUp() * 10.0f * delta;

    float oldXPos = (float)mouseX, oldYPos = (float)mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);
    float xDelta, yDelta;
    xDelta = (float)mouseX - oldXPos;
    yDelta = (float)mouseY - oldYPos;

    camera.setRotation(camera.getRotation() + glm::vec2{0.1f * xDelta * delta, 0.1f * yDelta * delta});
    camera.setPosition(camPos);
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

    demoMesh2.vertices = {
            {-1.0f,  1.0f,  1.0f, 0xff000000 },
            { 1.0f,  1.0f,  1.0f, 0xff0000ff },
            {-1.0f, -1.0f,  1.0f, 0xff00ff00 },
            { 1.0f, -1.0f,  1.0f, 0xff00ffff },
            {-1.0f,  1.0f, -1.0f, 0xffff0000 },
            { 1.0f,  1.0f, -1.0f, 0xffff00ff },
            {-1.0f, -1.0f, -1.0f, 0xffffff00 },
            { 1.0f, -1.0f, -1.0f, 0xffffffff },
    };

    demoMesh2.vertexIndices = {
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

    demoMesh2.modelMatrix = glm::translate(glm::mat4(1.0), glm::vec3(5, 0, 0));
}