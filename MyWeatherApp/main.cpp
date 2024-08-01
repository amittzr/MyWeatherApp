#include "CityWeather.h"
#include "WeatherFetcher.h"
#include "GuiDisplay.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <thread>
#include <atomic>
#include <mutex>
#include <vector>

int main() {
    if (!glfwInit()) return -1;
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Weather App", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    GuiDisplay::setupImGui(window);

    WeatherFetcher fetcher("64d90723e633cb96b39798798430d209");
    std::vector<CityWeather> cities(3);
    std::vector<std::pair<std::string, float>> cityHistory;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        GuiDisplay::displayAllWeatherData(cities, fetcher, cityHistory);

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    GuiDisplay::cleanupImGui();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

