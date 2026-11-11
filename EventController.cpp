#include "EventController.h"

void EventController::getEvents(const drogon::HttpRequestPtr& req, std::function<void(const drogon::HttpResponsePtr&)>&& callback) {

	auto events = EventService::GetInstance().getEvents();

	Json::Value jsonArray(Json::arrayValue);
	for (const auto& event : events) {
		jsonArray.append(event.toJson());
	}

	auto resp = drogon::HttpResponse::newHttpJsonResponse(jsonArray);

	callback(resp);
}

void EventController::saveEvent(const drogon::HttpRequestPtr& req, std::function<void(const drogon::HttpResponsePtr&)>&& callback) {
	auto resp = drogon::HttpResponse::newHttpResponse();
	resp->setStatusCode(drogon::k400BadRequest);
	callback(resp);
}