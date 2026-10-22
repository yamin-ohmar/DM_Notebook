// prevents the same header from being included multiple times in a translation unit
#pragma once

#include <string>

struct NPC {
    int id = 0;
    std::string name;
    std::string race;
    std::string role;
    std::string description;
};