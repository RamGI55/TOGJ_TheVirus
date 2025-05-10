//
// Created by hunda on 2025-05-09.
//

#ifndef UTILS_H
#define UTILS_H

#pragma once
#include <string>
#include <vector>
#include <random>
#include <nlohmann/json.hpp>
#include <fstream>

namespace Utils {
    nlohmann::json LoadJson(const std::string& filename);

    int RandomIntger(int min, int max);

    std::vector<std::string> Split(const std::string& s, char delim);
    std::string ToLower(const std::string& s);
}



#endif //UTILS_H
