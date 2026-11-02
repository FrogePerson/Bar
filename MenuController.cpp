#include "MenuController.h"

void MenuController::getDishes(const drogon::HttpRequestPtr& req, std::function<void(const drogon::HttpResponsePtr&)>&& callback) {

	auto dishes = MenuService::GetInstance().GetDishes();

	Json::Value jsonArray(Json::arrayValue);
	for (const auto& dish : dishes) {
		jsonArray.append(dish.toJson());
	}

	auto resp = drogon::HttpResponse::newHttpJsonResponse(jsonArray);

	callback(resp);
}

void MenuController::saveDish(const drogon::HttpRequestPtr& req, std::function<void(const drogon::HttpResponsePtr&)>&& callback) {

	auto jsonBody = req->getJsonObject();

	if (!jsonBody) {
		auto resp = drogon::HttpResponse::newHttpResponse();
		resp->setStatusCode(drogon::k400BadRequest);
		callback(resp);
		return;
	}

	Dish newDish(0, (*jsonBody)["name"].asString(), (*jsonBody)["cost"].asInt());

	Dish savedDish = MenuService::GetInstance().SaveDish(newDish);

	auto resp = drogon::HttpResponse::newHttpJsonResponse(savedDish.toJson());
	resp->setStatusCode(drogon::k201Created);
	callback(resp);
}