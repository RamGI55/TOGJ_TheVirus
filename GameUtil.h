//
// Created by hunda on 2025-05-09.
//

#ifndef GAMEUTIL_H
#define GAMEUTIL_H

#pragma once
#include <string>
#include <vector>
#include <random>
#include <nlohmann/json.hpp>
#include <fstream>

namespace GameUtil {
    nlohmann::json LoadJson(const std::string& filename);

    int RandomInt(int min, int max);

    std::vector<std::string> Split(const std::string& s, char delim);
    std::string ToLower(const std::string& s);
}

#endif //UTILS_H
