#pragma once
#include <string>
#include <json/json.h>

struct Event {
    int id;
    std::string name;
    std::string event_date;
    std::string imageUrl;
    std::string description;
    bool recommended;

    Event(int id, std::string name, std::string event_date, std::string imageUrl, std::string description, bool recommended) : id(id), name(name), event_date(event_date), imageUrl(imageUrl), description(description), recommended(recommended) {}

    Json::Value toJson() const {
        Json::Value ret;
        ret["id"] = id;
        ret["name"] = name;
        ret["event_date"] = event_date;
        ret["imageUrl"] = imageUrl;
        ret["description"] = description;
        ret["recommended"] = recommended;
        return ret;
    }
};