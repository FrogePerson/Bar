#pragma once
#include <drogon/drogon.h>
#include "Event.h"
#include <vector>
#include <optional>

class EventRepository {
public:
    static std::vector<Event> SelectAll() {
        std::vector<Event> events;
        auto client = drogon::app().getDbClient();

        try {
            auto result = client->execSqlSync("SELECT *, TO_CHAR(event_date, 'DD.MM.YYYY HH24:MI') AS formatted_date FROM events WHERE recommended = TRUE ORDER BY event_date ASC");

            for (auto row : result) {
                events.emplace_back(
                    row["id"].as<int>(),
                    row["name"].as<std::string>(),
                    row["formatted_date"].as<std::string>(),
                    row["image_url"].as<std::string>(),
                    row["description"].as<std::string>(),
                    row["recommended"].as<bool>()
                );
            }
        }
        catch (const drogon::orm::DrogonDbException& e) {
            LOG_ERROR << "DB Error: " << e.base().what();
        }
        return events;
    }

    static Event Insert(const Event& event) {
        auto client = drogon::app().getDbClient();
        auto result = client->execSqlSync(
            "INSERT INTO events (name, event_date, image_url, description, recommended) VALUES ($1, $2, $3, $4, $5) RETURNING id",
            event.name, event.event_date, event.imageUrl, event.description, event.recommended
        );

        int newId = result[0]["id"].as<int>();
        return Event(newId, event.name, event.event_date, event.imageUrl, event.description, event.recommended);
    }
};