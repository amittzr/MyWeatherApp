#include "GuiDisplay.h"
#include "WeatherFetcher.h"
#include "CityWeather.h"
#include "imgui.h"
#include <deque>
#include <algorithm>
#include <thread>

void GuiDisplay::displayAllWeatherData(std::vector<CityWeather>& cities, WeatherFetcher& fetcher, std::vector<std::pair<std::string, float>>& cityHistory) {
    ImGui::Begin("Weather Information");

    ImGui::Columns(3, "WeatherColumns");

    static char cityBuffer[3][128] = { "", "", "" };

    for (size_t i = 0; i < cities.size(); ++i) {
        ImGui::PushID(i);  // Push a unique identifier for each city block

        // Display the input box and refresh button for each city
        if (ImGui::InputText("City", cityBuffer[i], IM_ARRAYSIZE(cityBuffer[i]))) {
            cities[i].city = std::string(cityBuffer[i]);
        }

        if (ImGui::Button("Refresh")) {
            if (!cities[i].city.empty()) {
                cities[i].refreshRequested = true;
            }
            else {
                cities[i].refreshRequested = false;
            }
        }

        ImGui::SameLine(); // Display the next button on the same line
        displayMapButton(cities[i]);

        // Fetch and display weather data
        if (cities[i].refreshRequested && !cities[i].fetchingData) {
            cities[i].fetchingData = true;
            cities[i].refreshRequested = false;
            cities[i].refreshSuccessful = false; // Reset success flag

            std::thread fetchThread([&fetcher, &cityWeather = cities[i], &cityHistory]() {
                try {
                    fetcher.fetchWeatherData(cityWeather.city);
                    std::lock_guard<std::mutex> lock(cityWeather.fetchMutex);
                    cityWeather.weatherData = fetcher.getWeatherData();

                    // Check HTTP response status and update refreshSuccessful
                    if (cityWeather.weatherData.contains("cod") && cityWeather.weatherData["cod"].get<int>() == 200) {
                        cityWeather.refreshSuccessful = true;
                    }

                    // Update city history
                    float tempCelsius = cityWeather.weatherData["main"]["temp"].get<float>() - 273.15;
                    GuiDisplay::addToHistory(cityHistory, cityWeather.city, tempCelsius);
                }
                catch (const std::runtime_error& e) {
                    std::lock_guard<std::mutex> lock(cityWeather.fetchMutex);
                    cityWeather.weatherData.clear();
                    cityWeather.weatherData["error"] = e.what();
                }
                cityWeather.fetchingData = false;
                });
            fetchThread.detach();
        }

        if (cities[i].fetchingData) {
            ImGui::Text("Fetching weather data for %s...", cities[i].city.c_str());
        }
        else {
            std::lock_guard<std::mutex> lock(cities[i].fetchMutex);
            if (!cities[i].weatherData.empty()) {
                if (cities[i].weatherData.contains("error")) {
                    ImGui::Text("Error: %s", cities[i].weatherData["error"].get<std::string>().c_str());
                }
                else {
                    ImGui::Text("City: %s", cities[i].weatherData["name"].get<std::string>().c_str());
                    ImGui::Text("Temperature: %.2f C", cities[i].weatherData["main"]["temp"].get<float>() - 273.15);
                    ImGui::Text("Humidity: %d%%", cities[i].weatherData["main"]["humidity"].get<int>());
                    ImGui::Text("Wind Speed: %.2f m/s", cities[i].weatherData["wind"]["speed"].get<float>());
                    ImGui::Text("Description: %s", cities[i].weatherData["weather"][0]["description"].get<std::string>().c_str());
                }
            }
        }

        ImGui::NextColumn();  // Move to the next column for the next city
        ImGui::PopID();  // Pop the unique identifier
    }

    ImGui::Columns(1);

    // Display city history
    ImGui::Separator();
    ImGui::Text("City Search History:");
    for (const auto& entry : cityHistory) {
        ImGui::BulletText("%s: %.2f C", entry.first.c_str(), entry.second);
    }

    ImGui::End();
}

void GuiDisplay::addToHistory(std::vector<std::pair<std::string, float>>& cityHistory, const std::string& city, float tempCelsius) {
    auto it = std::find_if(cityHistory.begin(), cityHistory.end(),
        [&city](const std::pair<std::string, float>& entry) {
            return entry.first == city;
        });

    if (it != cityHistory.end()) {
        it->second = tempCelsius;
    }
    else {
        cityHistory.emplace_back(city, tempCelsius);
        if (cityHistory.size() > 5) {
            cityHistory.erase(cityHistory.begin());
        }
    }
}


// Utility function for URL encoding
// Converts spaces and special characters into a format safe for URLs.
std::string urlEncode(const std::string& value) {
    std::ostringstream out;
    for (auto c : value) {
        if (std::isalnum(static_cast<unsigned char>(c)) || c == '-' || c == '_' || c == '.' || c == '~') {
            out << c;
        }
        else {
            out << '%' << std::uppercase << std::setw(2) << std::setfill('0') << static_cast<int>(static_cast<unsigned char>(c));
        }
    }
    return out.str();
}

void GuiDisplay::displayMapButton(const CityWeather& cityWeather) {
    if (cityWeather.refreshSuccessful && !cityWeather.city.empty()) {
        // URL encode the city name
        std::string encodedCity = urlEncode(cityWeather.city);
        std::string mapsURL = "https://www.google.com/maps/search/?api=1&query=" + encodedCity;

        if (ImGui::Button("Map")) {
            // Open the Google Maps URL in the default web browser
            std::string command;
#ifdef _WIN32
            command = "start \"\" \"" + mapsURL + "\"";
#elif __APPLE__
            command = "open \"" + mapsURL + "\"";
#else
            command = "xdg-open \"" + mapsURL + "\"";
#endif
            system(command.c_str());
        }
    }
    else {
        ImGui::BeginDisabled(); // Disable the button if refresh wasn't successful
        ImGui::Button("Map");
        ImGui::EndDisabled();
    }
}


void GuiDisplay::setupImGui(GLFWwindow* window) {
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
}

void GuiDisplay::cleanupImGui() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}




