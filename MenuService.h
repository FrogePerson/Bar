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

	std::vector<Dish> GetDishes() {
		return DishRepository::SelectAll();
	}

	Dish SaveDish(const Dish& dish) {
		return DishRepository::Insert(dish);
	}

private:
	MenuService() = default;
};