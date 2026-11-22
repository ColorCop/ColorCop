cmake_minimum_required(VERSION 3.20)

project(ColorCopTests LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Catch2 3 CONFIG REQUIRED)

add_executable(ColorCopTests
    ColorCopTests/TestMain.cpp
    ColorCopTests/WebSafeTests.cpp
    ColorCopTests/SamplingBoundsTests.cpp
    ColorCopTests/ColorConversionTests.cpp
    ColorCopTests/ColorHistoryTests.cpp

)

target_include_directories(ColorCopTests PRIVATE
    ColorCop
)

target_link_libraries(ColorCopTests PRIVATE
    Catch2::Catch2
)
