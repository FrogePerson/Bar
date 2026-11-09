#pragma once
#include <drogon/HttpController.h>
#include "MenuService.h"

class MenuController : public drogon::HttpController< MenuController> {
public:
	METHOD_LIST_BEGIN
		ADD_METHOD_TO(MenuController::getDishes, "/api/menu", drogon::Get);
		ADD_METHOD_TO(MenuController::addDish, "/api/admin/addDish", drogon::Post);
	METHOD_LIST_END

	void getDishes(const drogon::HttpRequestPtr& req, std::function<void(const drogon::HttpResponsePtr&)>&& callback);

	void saveDish(const drogon::HttpRequestPtr& req, std::function<void(const drogon::HttpResponsePtr&)>&& callback);

	void addDish(const drogon::HttpRequestPtr& req, std::function<void(const drogon::HttpResponsePtr&)>&& callback);

};