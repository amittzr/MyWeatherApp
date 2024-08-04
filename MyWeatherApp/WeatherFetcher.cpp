#include "WeatherFetcher.h"

// get api data
void WeatherFetcher::fetchWeatherData(const std::string& city) {
    httplib::Client cli("api.openweathermap.org");
    std::string path = "/data/2.5/weather?q=" + city + "&appid=" + apiKey_;
    auto res = cli.Get(path.c_str());

    if (res && res->status == 200) {
        weatherData_ = json::parse(res->body);
    }
    else {
        //std::cerr << "Error : " << res->status << std::endl;
        throw std::runtime_error("Failed to fetch weather data");
    }
}

json WeatherFetcher::getWeatherData() const {
    return weatherData_;
}



