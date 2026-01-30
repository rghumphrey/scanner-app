cmake_minimum_required(VERSION 3.16)

project(scanner-app
    VERSION 1.0
    LANGUAGES CXX
)

# ---------------------------
# C++ Standard
# ---------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ---------------------------
# Dependencies
# ---------------------------

# OpenCV
find_package(OpenCV REQUIRED)

# ZXing-C++
find_package(ZXing REQUIRED)

# CURL (for Open Food Facts API)
find_package(CURL REQUIRED)

# nlohmann/json (header-only)
find_package(nlohmann_json REQUIRED)

# ---------------------------
# Executable
# ---------------------------
add_executable(scanner-app
    src/main.cpp
    src/BarcodeScanner.cpp
    src/OpenFoodFactsClient.cpp
    src/FoodDatabase.cpp
    src/FoodItem.cpp
)

# ---------------------------
# Include directories
# ---------------------------
target_include_directories(scanner-app
    PRIVATE
        ${OpenCV_INCLUDE_DIRS}
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/config
)

# ---------------------------
# Link libraries
# ---------------------------
target_link_libraries(scanner-app
    PRIVATE
        ${OpenCV_LIBS}
        ZXing::ZXing
        CURL::libcurl
        nlohmann_json::nlohmann_json
)

# ---------------------------
# Compile warnings (nice to have)
# ---------------------------
target_compile_options(scanner-app
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)
