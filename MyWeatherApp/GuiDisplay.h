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
    //display all the data in the wondow
    void displayAllWeatherData(std::vector<CityWeather>& cities, WeatherFetcher& fetcher, std::vector<std::pair<std::string, float>>& cityHistory);
    //add pairs of <string & float > to history - save last 5
    void addToHistory(std::vector<std::pair<std::string, float>>& cityHistory, const std::string& city, float tempCelsius);

    void setupImGui(GLFWwindow* window);
    void cleanupImGui();

    //handle Map Button
    void displayMapButton(const CityWeather& cityWeather);
}


