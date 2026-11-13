#include "MenuController.h"
#include <drogon/utils/Utilities.h>

void MenuController::addDish(const drogon::HttpRequestPtr& req, std::function<void(const drogon::HttpResponsePtr&)>&& callback) {

    LOG_INFO << "=== ЗАШЛИ В addDish ===";

    drogon::MultiPartParser fileUpload;

    if (fileUpload.parse(req) != 0 || fileUpload.getFiles().empty()) {
        LOG_ERROR << "=== ОШИБКА ПАРСИНГА: Файл не найден или неверный формат ===";
        auto resp = drogon::HttpResponse::newHttpResponse();
        resp->setStatusCode(drogon::k400BadRequest);
        callback(resp);
        return;
    }

    auto parameters = fileUpload.getParameters();
    auto files = fileUpload.getFiles();

    try {
        std::string name = parameters["name"];
        std::string description = parameters["description"];
        int cost = std::stoi(parameters["cost"]);
        bool recommended = true;

        std::string category = parameters["category"];

        auto& imageFile = files[0];

        std::string imageUrl = ImgService::SaveImg(imageFile);

        Dish newDish(0, name, cost, imageUrl, description, recommended, category);
        Dish savedDish = MenuService::GetInstance().SaveDish(newDish);

        auto resp = drogon::HttpResponse::newHttpJsonResponse(savedDish.toJson());
        resp->setStatusCode(drogon::k201Created);
        callback(resp);

    }
    catch (const std::exception& e) {
        // Если при парсинге цены произошла ошибка (например, пустое поле)
        LOG_ERROR << "Form processing error: " << e.what();
        auto resp = drogon::HttpResponse::newHttpResponse();
        resp->setStatusCode(drogon::k400BadRequest);
        callback(resp);
    }
}


void MenuController::deleteDish(const drogon::HttpRequestPtr& req, std::function<void(const drogon::HttpResponsePtr&)>&& callback) {

    LOG_INFO << "=== ЗАШЛИ В deleteDish ===";

    drogon::MultiPartParser fileUpload;

    if (fileUpload.parse(req) != 0) {
        LOG_ERROR << "=== ОШИБКА ПАРСИНГА: Файл не найден или неверный формат ===";
        auto resp = drogon::HttpResponse::newHttpResponse();
        resp->setStatusCode(drogon::k400BadRequest);
        callback(resp);
        return;
    }

    auto parameters = fileUpload.getParameters();

    try {
        int id = std::stoi(parameters["id"]);

        if (!MenuService::GetInstance().DeleteDish(id)) {
            LOG_ERROR << "DELETE error! ";
            auto resp = drogon::HttpResponse::newHttpResponse();
            resp->setStatusCode(drogon::k400BadRequest);
            callback(resp);
            return;
        }

        auto resp = drogon::HttpResponse::newHttpResponse();
        resp->setStatusCode(drogon::k200OK);
        callback(resp);

    }
    catch (const std::exception& e) {
        // Если при парсинге цены произошла ошибка (например, пустое поле)
        LOG_ERROR << "Form processing error: " << e.what();
        auto resp = drogon::HttpResponse::newHttpResponse();
        resp->setStatusCode(drogon::k400BadRequest);
        callback(resp);
    }
}



void MenuController::changeDish(const drogon::HttpRequestPtr& req, std::function<void(const drogon::HttpResponsePtr&)>&& callback) {
    LOG_INFO << "=== ЗАШЛИ В changeDish ===";

    drogon::MultiPartParser fileUpload;

    if (fileUpload.parse(req) != 0) {
        LOG_ERROR << "=== ОШИБКА ПАРСИНГА: Файл не найден или неверный формат ===";
        auto resp = drogon::HttpResponse::newHttpResponse();
        resp->setStatusCode(drogon::k400BadRequest);
        callback(resp);
        return;
    }

    auto parameters = fileUpload.getParameters();
    auto files = fileUpload.getFiles();

    try {
        int id = std::stoi(parameters["id"]);
        std::string name = parameters["name"];
        std::string description = parameters["description"];
        int cost = std::stoi(parameters["cost"]);
        bool recommended = true;

        std::string category = parameters["category"];

        std::string imageUrl = "";

        if (!files.empty() && files[0].fileLength() > 0) {
            auto& imageFile = files[0];
            imageUrl = ImgService::SaveImg(imageFile);
        }




        Dish newDish(id, name, cost, imageUrl, description, recommended, category);

        if (!MenuService::GetInstance().ChangeDish(newDish)) {
            LOG_ERROR << "Change error! ";
            auto resp = drogon::HttpResponse::newHttpResponse();
            resp->setStatusCode(drogon::k400BadRequest);
            callback(resp);
            return;
        }

        auto resp = drogon::HttpResponse::newHttpResponse();
        resp->setStatusCode(drogon::k200OK);
        callback(resp);

    }
    catch (const std::exception& e) {
        // Если при парсинге цены произошла ошибка (например, пустое поле)
        LOG_ERROR << "Form processing error: " << e.what();
        auto resp = drogon::HttpResponse::newHttpResponse();
        resp->setStatusCode(drogon::k400BadRequest);
        callback(resp);
    }
}
