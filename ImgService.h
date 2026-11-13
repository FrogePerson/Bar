#pragma once
#include <vector>
#include <stdexcept>
#include <drogon/utils/Utilities.h>
#include <drogon/HttpController.h>

class ImgService {
public:
	static std::string SaveImg(const drogon::HttpFile& img);
	static bool DeleteImg(const std::string& url);
};
