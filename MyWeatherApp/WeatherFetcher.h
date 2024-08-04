#pragma once
#include "httplib.h"
#include "json.hpp"
#include <string>

using json = nlohmann::json;

//class for the api requests
class WeatherFetcher {
public:
    WeatherFetcher(const std::string& apiKey) : apiKey_(apiKey) {}
    void fetchWeatherData(const std::string& city);
    json getWeatherData() const;

private:
    std::string apiKey_;
    json weatherData_;
};
#pragma once



