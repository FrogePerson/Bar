#include <drogon/drogon.h>

int main() {
    drogon::app().setLogLevel(trantor::Logger::kTrace);

    drogon::app().loadConfigFile("config.json");

    LOG_INFO << "Gooooool!";

    // Запускаем бесконечный цикл сервера
    drogon::app().run();

    return 0;
}