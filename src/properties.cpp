#include "../include/properties.h"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

nlohmann::json loadConfig(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open config file: " + filepath);
    }
    nlohmann::json config;
    file >> config;
    return config;
}

Properties::Properties() {
    auto config = loadConfig("../properties.json");

    if (!config.contains("data_set") || !config["data_set"].contains("path") || config["data_set"]["path"].is_null()) {
        throw std::runtime_error("Error: Missing or null 'data_set.path' in properties.json");
    }

    path = config["data_set"]["path"].get<std::string>();
}

std::string Properties::getPath() const { return path; }
