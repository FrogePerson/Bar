#include "MenuController.h"

void MenuController::getDishes(const drogon::HttpRequestPtr& req, std::function<void(const drogon::HttpResponsePtr&)>&& callback) {

	std::string category = req->getParameter("category"); //принимаем категорию
	if (category.empty()) category = "all";
	auto dishes = MenuService::GetInstance().GetDishes(category);

	Json::Value jsonArray(Json::arrayValue);
	for (const auto& dish : dishes) {
		jsonArray.append(dish.toJson());
	}

	auto resp = drogon::HttpResponse::newHttpJsonResponse(jsonArray);

	callback(resp);
}

void MenuController::saveDish(const drogon::HttpRequestPtr& req, std::function<void(const drogon::HttpResponsePtr&)>&& callback) {
	auto resp = drogon::HttpResponse::newHttpResponse();
	resp->setStatusCode(drogon::k400BadRequest);
	callback(resp);
}