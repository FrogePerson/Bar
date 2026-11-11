#pragma once
#include <drogon/HttpController.h>
#include "EventService.h"

class EventController : public drogon::HttpController<EventController> {
public:
	METHOD_LIST_BEGIN
		ADD_METHOD_TO(EventController::getEvents, "/api/events", drogon::Get);
		ADD_METHOD_TO(EventController::saveEvent, "/api/events", drogon::Post);
	METHOD_LIST_END

	void getEvents(const drogon::HttpRequestPtr& req, std::function<void(const drogon::HttpResponsePtr&)>&& callback);

	void saveEvent(const drogon::HttpRequestPtr& req, std::function<void(const drogon::HttpResponsePtr&)>&& callback);

	void addEvent(const drogon::HttpRequestPtr& req, std::function<void(const drogon::HttpResponsePtr&)>&& callback);

};