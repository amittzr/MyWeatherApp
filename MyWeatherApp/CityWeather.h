#pragma once
#include "json.hpp"
#include <string>
#include <mutex>


struct CityWeather {
    std::string city; // city name
    nlohmann::json weatherData; // city data - json
    std::atomic<bool> refreshRequested = false;     // refrsh pressed denie race condition     
    std::atomic<bool> fetchingData = false;           
    std::mutex fetchMutex;                           
    bool refreshSuccessful = false;                 // member variable to track refresh status
};

