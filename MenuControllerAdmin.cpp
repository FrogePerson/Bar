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

        // std::string category = parameters["category"];

        auto& imageFile = files[0];
        std::string ext = std::string(imageFile.getFileExtension());

        std::string fileName = drogon::utils::getUuid() + ext;

        imageFile.saveAs("/app/img/" + fileName);

        std::string imageUrl = "/img/" + fileName;

        Dish newDish(0, name, cost, imageUrl, description);
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