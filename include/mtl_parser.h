#pragma once
#include "material.h"
#include <vector>
#include <map>
#include<sstream>
class MTLParser {
public:
    static std::map<std::string, Material> parse(const std::string& filepath);
};
