#pragma once
#include <string>
#include <json/json.h>

struct Dish {
    int id;
    std::string name;
    int cost;
    std::string imageUrl;
    std::string description;

    Dish(int id, std::string name, int cost, std::string imageUrl, std::string description) : id(id), name(name), cost(cost), imageUrl(imageUrl), description() {}

    Json::Value toJson() const {
        Json::Value ret;
        ret["id"] = id;
        ret["name"] = name;
        ret["cost"] = cost;
        ret["imageUrl"] = imageUrl;
        ret["description"] = description;
        return ret;
    }
};