#pragma once
#include <string>
#include <json/json.h>

struct Dish {
    int id;
    std::string name;
    int cost;

    Dish(int id, std::string name, int cost) : id(id), name(name), cost(cost) {}

    Json::Value toJson() const {
        Json::Value ret;
        ret["id"] = id;
        ret["name"] = name;
        ret["cost"] = cost;
        return ret;
    }
};