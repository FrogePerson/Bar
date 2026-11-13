#pragma once
#include "DishRepository.h"
#include <vector>
#include <stdexcept>
#include "ImgService.h"

class MenuService {
public:
	static MenuService& GetInstance() {
		static MenuService instance;
		return instance;
	}

	std::vector<Dish> GetDishes(const std::string& category) {
		return DishRepository::SelectAll(category);
	}

	std::vector<Dish> GetDishes() {
		return DishRepository::SelectAll();
	}

	Dish SaveDish(const Dish& dish) {

		auto dishes = GetInstance().GetDishes();

		for (const auto& _dish : dishes) {
			if (_dish.name == dish.name) {
				throw std::invalid_argument("Блюдо с таким названием уже есть в этой категории!");
			}
		}

		return DishRepository::Insert(dish);
	}

	Dish FindById(const int id) {

		auto dishOpt = DishRepository::FindById(id);

		if (!dishOpt) {
			throw std::invalid_argument("Блюдо с таким ID не существует!");
		}

		return dishOpt.value();

	}

	bool DeleteDish(const int id) {

		Dish dish = FindById(id);

		if (!ImgService::DeleteImg(dish.imageUrl)) {
			return false;
		}

		if (!DishRepository::DeleteDish(dish.id)) {
			return false;
		}
		return true;
	}

private:
	MenuService() = default;
};