#define GLFW_EXPOSE_NATIVE_WIN32
#include <iostream>
#include <main.h>

#include "GLFW/glfw3.h"
#include "GLFW/glfw3native.h"
#include "bgfx/bgfx.h"
#include "shader/shader.h"
#include "mesh/surface.h"
#include "pipeline/renderer.h"

#define WIDTH 1280
#define HEIGHT 720


GLFWwindow* window;
GLFWmonitor* monitor;

Shader mainShader;

Mesh demoMesh;

bx::Vec3 cameraPos(0, 0, -10);
bx::Vec3 up(0, 1, 0);
bx::Vec3 forward(0, 0, 1);
bx::Vec3 right (1, 0, 0);
float cameraPitch {0};
float cameraYaw {0};

Renderer renderer;

int speed = 15;
float sensitivity = 0.1f;

int main()
{
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    glfwInit();

    window = glfwCreateWindow(WIDTH, HEIGHT, "3D Renderer", nullptr, nullptr);
    monitor = glfwGetPrimaryMonitor();

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

    bgfx::setDebug(BGFX_DEBUG_TEXT);

    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x31a9b4FF, 1.0f, 0);
    bgfx::setViewRect(0, 0, 0, WIDTH, HEIGHT);

    load();

    double lastTime = glfwGetTime();
    while (!glfwWindowShouldClose(window))
    {
        double dt = glfwGetTime() - lastTime;
        lastTime = glfwGetTime();

        input();

        update(static_cast<float>(dt));

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

double mouseX, mouseY;

void render ()
{
    //bgfx::dbgTextPrintf(0, 0, ((0x2) << 4) | 0xF, std::to_string(glfwGetTime()).c_str());
    bx::Quaternion rot = bx::fromEuler(bx::Vec3{cameraPitch, cameraYaw, 0});
    bx::Vec3 at = bx::add(cameraPos, bx::mul(forward, rot));

    renderer.setViewMatrix(cameraPos, at, bx::mul(up, rot));
    renderer.setProjectionMatrix(60, 1, 0.01f, 1000.0f);
    renderer.setViewport(0, 0, WIDTH, HEIGHT);
    renderer.prepare();

    bgfx::setVertexBuffer(0, demoMesh.getVBH());
    bgfx::setIndexBuffer(demoMesh.getIBH());

    bgfx::submit(0, mainShader.program);
}

void update (float delta)
{
    bx::Quaternion rot = bx::fromEuler(bx::Vec3{cameraPitch, cameraYaw, 0});
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        cameraPos = bx::add(cameraPos, bx::mul(bx::mul(forward, rot), delta * speed));
    }
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        cameraPos = bx::sub(cameraPos, bx::mul(bx::mul(forward, rot), delta * speed));
    }
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        cameraPos = bx::sub(cameraPos, bx::mul(bx::mul(right, rot), delta * speed));
    }
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        cameraPos = bx::add(cameraPos, bx::mul(bx::mul(right, rot), delta * speed));
    }
    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        cameraPos = bx::add(cameraPos, bx::mul(bx::mul(up, rot), delta * speed));
    }
    if(glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        cameraPos = bx::sub(cameraPos, bx::mul(bx::mul(up, rot), delta * speed));
    }


    float oldXPos = (float)mouseX, oldYPos = (float)mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);
    float xDelta, yDelta;
    xDelta = (float)mouseX - oldXPos;
    yDelta = (float)mouseY - oldYPos;


    cameraYaw -= sensitivity * xDelta * delta;
    //if (cameraYaw > 360) cameraYaw -= 360;
    //if (cameraYaw < 0) cameraYaw += 360;

    cameraPitch -= sensitivity * yDelta * delta;
    //if (cameraPitch > 360) cameraPitch -= 360;
    //if (cameraPitch < 0) cameraPitch += 360;
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