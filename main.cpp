#include <drogon/drogon.h>

int main() {
    drogon::app().setLogLevel(trantor::Logger::kTrace);

    drogon::app().loadConfigFile("config.json");

    LOG_INFO << "Gooooool!";

    drogon::app().run();

    return 0;
}