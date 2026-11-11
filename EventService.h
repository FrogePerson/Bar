#pragma once
#include "EventRepository.h"
#include <vector>
#include <stdexcept>

class EventService {
public:
	static EventService& GetInstance() {
		static EventService instance;
		return instance;
	}

	std::vector<Event> getEvents() {
		return EventRepository::SelectAll();
	}

	Event SaveEvent(const Event& event) {
		return EventRepository::Insert(event);
	}

private:
	EventService() = default;
};