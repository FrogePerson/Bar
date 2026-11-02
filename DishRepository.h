#pragma once
#include <drogon/drogon.h>
#include "Dish.h"
#include <vector>
#include <optional>

class DishRepository {
public:
    static std::vector<Dish> SelectAll() {
        std::vector<Dish> dishes;
        auto client = drogon::app().getDbClient();

        try {
            auto result = client->execSqlSync("SELECT id, name, cost FROM dishes");

            for (auto row : result) {
                dishes.emplace_back(
                    row["id"].as<int>(),
                    row["name"].as<std::string>(),
                    row["cost"].as<int>()
                );
            }
        }
        catch (const drogon::orm::DrogonDbException& e) {
            LOG_ERROR << "DB Error: " << e.base().what();
        }
        return dishes;
    }

    static Dish Insert(const Dish& dish) {
        auto client = drogon::app().getDbClient();
        auto result = client->execSqlSync(
            "INSERT INTO dishes (name, cost) VALUES ($1, $2) RETURNING id",
            dish.name, dish.cost
        );

        int newId = result[0]["id"].as<int>();
        return Dish(newId, dish.name, dish.cost);
    }
};