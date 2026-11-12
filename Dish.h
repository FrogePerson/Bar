#pragma once
#include <string>
#include <json/json.h>

struct Dish {
    int id;
    std::string name;
    int cost;
    std::string imageUrl;
    std::string description;
    bool recommended;
    std::string category;

    Dish(int id, std::string name, int cost, std::string imageUrl, std::string description, bool recommended, std::string category) : id(id), name(name), cost(cost), imageUrl(imageUrl), description(description), recommended(recommended), category(category) {}

    Json::Value toJson() const {
        Json::Value ret;
        ret["id"] = id;
        ret["name"] = name;
        ret["cost"] = cost;
        ret["imageUrl"] = imageUrl;
        ret["description"] = description;
        ret["recommended"] = recommended;
        ret["category"] = category;
        return ret;
    }
};