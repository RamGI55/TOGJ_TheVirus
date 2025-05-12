//
// Created by hunda on 2025-05-09.
//

#include "GameUtil.h"
#include <algorithm>
#include <iostream>
#include <random>

namespace GameUtil {
    // Loading the json file
    nlohmann::json LoadJson(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Failed to open file: " << filename << std::endl;
            throw std::runtime_error("Failed to open file: " + filename);
        }

        try {
            nlohmann::json data;
            file >> data;

            // Debug output
            std::cerr << "Successfully loaded JSON from: " << filename << std::endl;
            std::cerr << "JSON structure: ";
            for (auto it = data.begin(); it != data.end(); ++it) {
                std::cerr << it.key() << " ";
            }
            std::cerr << std::endl;

            return data;
        } catch (const nlohmann::json::parse_error& e) {
            std::cerr << "JSON parse error in file " << filename << ": " << e.what() << std::endl;
            throw;
        }
    }

    //
    int RandomInt(int min, int max) {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dis(min, max);
        return dis(gen);
    }

    std::vector<std::string> Split(const std::string&s, char delim) {
        std::vector<std::string> tokens;
        std::string token;
        std::istringstream tokenstream(s);

        while (std::getline(tokenstream, token, delim)) {
            tokens.push_back(token);
        }
        return tokens;
    }

    std::string ToLower(const std::string &s) {
        std::string result = s;
        std::transform (result.begin(), result.end(), result.begin(), [](unsigned char c) {
            return std::tolower(c);
        });
        return result;
    }

    bool FileExists(const std::string &filename) {
        std::ifstream file(filename);
        return file.good();
    }

    bool ContainsIngnoreCase(const std::string &haystack, const std::string &needle) {
        auto it = std::search(
    haystack.begin(), haystack.end(),
    needle.begin(), needle.end(),
    [](char ch1, char ch2) { return std::toupper(ch1) == std::toupper(ch2); }
);
        return (it != haystack.end());

    }
}
