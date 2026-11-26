cmake_minimum_required(VERSION 3.20)

project(ColorCopTests LANGUAGES CXX)

find_package(Catch2 3 CONFIG REQUIRED)

add_executable(ColorCopTests
    ColorCopTests/WebSafeTests.cpp
    ColorCopTests/SamplingBoundsTests.cpp
    ColorCopTests/ColorConversionTests.cpp
    ColorCopTests/ColorHistoryTests.cpp
)

target_compile_features(ColorCopTests PRIVATE cxx_std_23)

target_include_directories(ColorCopTests PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/ColorCop"
)

target_link_libraries(ColorCopTests PRIVATE
    Catch2::Catch2WithMain
)

if(MSVC)
    target_compile_options(ColorCopTests PRIVATE /W4 /permissive- /Zc:__cplusplus)
endif()

include(CTest)
if(BUILD_TESTING)
    include(Catch)
    catch_discover_tests(ColorCopTests)
endif()
