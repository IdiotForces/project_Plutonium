
#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>

#define NANOVG_GL3_IMPLEMENTATION

#include <nanovg.h>
#include <nanovg_gl.h>
#include <nanovg_gl_utils.h>

int main() {

    glfwInit();

    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    GLFWwindow *window = glfwCreateWindow(800, 600, "nanovg Demo", nullptr, nullptr);

    NVGcontext *vg = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG);

    while (!glfwWindowShouldClose(window)) {

        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);

        nvgBeginFrame(vg, 800, 600, 1);

        nvgBeginPath(vg);
        nvgRoundedRect(vg, 12, 12, 800-12*2, 600-12*2, 4);
        nvgFillColor(vg, nvgRGBA(255, 255, 255, 192));
        nvgFill(vg);

        nvgEndFrame(vg);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}