#pragma once
#include <drogon/drogon.h>
#include "Dish.h"
#include <vector>
#include <optional>

class DishRepository {
public:
    static std::vector<Dish> SelectAll(const std::string& category) {
        std::vector<Dish> dishes;
        auto client = drogon::app().getDbClient();

        try {
            auto result = client->execSqlSync("SELECT * FROM dishes WHERE category = $1 OR $1 = 'all'", category);
            
            for (auto row : result) {
                dishes.emplace_back(
                    row["id"].as<int>(),
                    row["name"].as<std::string>(),
                    row["cost"].as<int>(),
                    row["image_url"].as<std::string>(),
                    row["description"].as<std::string>(),
                    row["recommended"].as<bool>(),
                    row["category"].as<std::string>()
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
            "INSERT INTO dishes (name, cost, image_url, description, recommended, category) VALUES ($1, $2, $3, $4, $5, $6) RETURNING id",
            dish.name, dish.cost, dish.imageUrl, dish.description, dish.recommended, dish.category
        );

        int newId = result[0]["id"].as<int>();
        return Dish(newId, dish.name, dish.cost, dish.imageUrl, dish.description, dish.recommended, dish.category);
    }
};