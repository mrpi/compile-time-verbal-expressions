cmake_minimum_required(VERSION 3.8.0)
project(compile-time-verbal-expressions VERSION 0.1.0 LANGUAGES CXX)

add_library(ctve INTERFACE)
add_library(ctve::ctve ALIAS ctve)
target_include_directories(ctve INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)
target_compile_features(ctve INTERFACE cxx_std_17)

install(TARGETS ctve EXPORT ctve-targets)

export(EXPORT ctve-targets NAMESPACE ctve::)
file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/ctve-config.cmake
    "include(\${CMAKE_CURRENT_LIST_DIR}/ctve-targets.cmake)")
include(CMakePackageConfigHelpers)
write_basic_package_version_file(ctve-config-version.cmake
    VERSION ${ctve_VERSION}
    COMPATIBILITY SameMajorVersion)

install(EXPORT ctve-targets DESTINATION lib/cmake/ctve
    NAMESPACE ctve::)
install(
    FILES
        ${CMAKE_CURRENT_BINARY_DIR}/ctve-config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/ctve-config-version.cmake
    DESTINATION lib/cmake/ctve)
install(DIRECTORY include/ DESTINATION include
    FILES_MATCHING PATTERN *.hpp)

option(ENABLE_CTVE_SELF_TEST "build self tests" OFF)

if(ENABLE_CTVE_SELF_TEST)
   add_subdirectory(tests)
endif()
