#include "DishRepository.h"

std::vector<Dish> DishRepository::SelectAll(const std::string& category) {
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

std::vector<Dish> DishRepository::SelectAll() {
    std::vector<Dish> dishes;
    auto client = drogon::app().getDbClient();

    try {
        auto result = client->execSqlSync("SELECT * FROM dishes");

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

Dish DishRepository::Insert(const Dish& dish) {
    auto client = drogon::app().getDbClient();
    auto result = client->execSqlSync(
        "INSERT INTO dishes (name, cost, image_url, description, recommended, category) VALUES ($1, $2, $3, $4, $5, $6) RETURNING id",
        dish.name, dish.cost, dish.imageUrl, dish.description, dish.recommended, dish.category
    );

    int newId = result[0]["id"].as<int>();
    return Dish(newId, dish.name, dish.cost, dish.imageUrl, dish.description, dish.recommended, dish.category);
}

std::optional<Dish> DishRepository::FindById(const int id) {
    auto client = drogon::app().getDbClient();
    try {
        auto result = client->execSqlSync("SELECT * FROM dishes WHERE id = $1", id);

        if (!result.empty()) {

            auto row = result[0];

            return Dish(
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
    return std::nullopt;
}

bool DishRepository::DeleteDish(const int id) {
    auto client = drogon::app().getDbClient();

    try {
        client->execSqlSync("DELETE FROM dishes WHERE id = $1", id);
        return true;
    }
    catch (const drogon::orm::DrogonDbException& e) {
        LOG_ERROR << "DB Error: " << e.base().what();
    }
    return false;

}

bool DishRepository::UpdateDish(const Dish& dish)
{
    auto client = drogon::app().getDbClient();

    try {
        if (dish.imageUrl.empty()) {
            auto result = client->execSqlSync(
                "UPDATE dishes "
                "SET name = $1, cost = $2, description = $3, recommended = $4, category = $5 "
                "WHERE id = $6",
                dish.name,
                dish.cost,
                dish.description,
                dish.recommended,
                dish.category,
                dish.id
            );
            return result.affectedRows() > 0;
        }
        else {
            auto result = client->execSqlSync(
                "UPDATE dishes "
                "SET name = $1, cost = $2, image_url = $3, description = $4, recommended = $5, category = $6 "
                "WHERE id = $7",
                dish.name,
                dish.cost,
                dish.imageUrl,
                dish.description,
                dish.recommended,
                dish.category,
                dish.id
            );
            return result.affectedRows() > 0;
        }
    }
    catch (const drogon::orm::DrogonDbException& e) {
        LOG_ERROR << "DB Error in UpdateDish: " << e.base().what();
        return false;
    }
}
