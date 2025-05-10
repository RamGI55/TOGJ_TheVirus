//
// Created by hunda on 2025-05-09.
//

#include "GameUtil.h"
#include <algorithm>
#include <random>

namespace GameUtil {
    // Loading the json file
    nlohmann::json LoadJson(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file");
        }
        nlohmann::json data;
        file >> data;
        return data;
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
}
