#pragma once

#include "json.hpp"
#include <string>
#include <atomic>
#include <mutex>
#include <vector>
#include <GLFW/glfw3.h>
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"

struct CityWeather;
class WeatherFetcher;

namespace GuiDisplay {
    void displayAllWeatherData(std::vector<CityWeather>& cities, WeatherFetcher& fetcher, std::vector<std::pair<std::string, float>>& cityHistory);
    void addToHistory(std::vector<std::pair<std::string, float>>& cityHistory, const std::string& city, float tempCelsius);
    void setupImGui(GLFWwindow* window);
    void cleanupImGui();
    void displayMapButton(const CityWeather& cityWeather);
}


