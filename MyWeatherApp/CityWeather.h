#pragma once
#include "json.hpp"
#include <string>
#include <mutex>


struct CityWeather {
    std::string city;
    nlohmann::json weatherData;
    std::atomic<bool> refreshRequested = false;
    std::atomic<bool> fetchingData = false;
    //std::atomic<bool> isFavorite = false;
    std::mutex fetchMutex;
    bool refreshSuccessful = false; // New member variable to track refresh status
};

