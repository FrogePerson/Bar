#pragma once
#include <drogon/drogon.h>
#include "Dish.h"
#include <vector>
#include <optional>

class DishRepository {
public:
    static std::vector<Dish> SelectAll(const std::string& category);

    static std::vector<Dish> SelectAll();

    static Dish Insert(const Dish& dish);

    static std::optional<Dish> FindById(const int id);

    static bool DeleteDish(const int id);
};