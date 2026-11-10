#pragma once
#include "DishRepository.h"
#include <vector>
#include <stdexcept>

class MenuService {
public:
	static MenuService& GetInstance() {
		static MenuService instance;
		return instance;
	}

	std::vector<Dish> GetDishes(const std::string& category) {
		return DishRepository::SelectAll(category);
	}

	Dish SaveDish(const Dish& dish) {
		return DishRepository::Insert(dish);
	}

private:
	MenuService() = default;
};