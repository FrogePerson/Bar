cmake_minimum_required(VERSION 3.15)
project(bar_backend CXX)

set(CMAKE_CXX_STANDARD 20)

find_package(Drogon CONFIG REQUIRED)

#(заголовочные .h указывать не нужно)
add_executable(${PROJECT_NAME} 
    main.cpp 
    MenuController.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE Drogon::Drogon)