#include "ImgService.h"

std::string ImgService::SaveImg(const drogon::HttpFile& img)
{

    std::string ext = std::string(img.getFileExtension());

    ImgName++;
    std::string fileName = std::to_string(ImgName);

    img.saveAs("/app/img/" + fileName);

    std::string imageUrl = "/img/" + fileName;

    return imageUrl;
}

bool ImgService::DeleteImg(const std::string& url)
{

    if (url != "/img/dish-placeholder.png" && !url.empty()) {

        std::string physicalPath = "/app" + url;

        std::error_code ec;
        if (std::filesystem::remove(physicalPath, ec)) {
            LOG_INFO << "Успешно удален файл: " << physicalPath;
        }
        else {
            LOG_WARN << "Не удалось удалить файл (возможно он уже удален): " << ec.message();
        }
    }

    return true;
}
