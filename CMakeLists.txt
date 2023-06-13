cmake_minimum_required(VERSION 3.15)
file(STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/version.txt" projectVersion)
project(simplessp VERSION ${projectVersion})
message("Current ${PROJECT_NAME} version: ${CMAKE_PROJECT_VERSION}\n")

# ==============================================================================
# Build settings
# ==============================================================================

option(SSP_BUILD_TESTS "Build tests" ON)


# ==============================================================================
# Global internal configuration
# ==============================================================================

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

# ==============================================================================
# Dependencies
# ==============================================================================

find_package(spdlog CONFIG REQUIRED)
find_package(libzip CONFIG REQUIRED)
find_package(pugixml CONFIG REQUIRED)

if (SSP_BUILD_TESTS)
    Include(FetchContent)

    FetchContent_Declare(
            Catch2
            GIT_REPOSITORY https://github.com/catchorg/Catch2.git
            GIT_TAG v2.13.8
    )
    FetchContent_MakeAvailable(Catch2)
endif ()


# ==============================================================================
# Targets
# ==============================================================================

add_subdirectory(src)

if (SSP_BUILD_TESTS)
    file(COPY data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

    enable_testing()
    add_subdirectory(tests)
endif ()


# ==============================================================================
# Exports and remaining installation
# ==============================================================================


configure_package_config_file(cmake/config.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/simplessp-config.cmake
        INSTALL_DESTINATION ${CMAKE_INSTALL_DATADIR}/simple-ssp
        NO_SET_AND_CHECK_MACRO)
write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/simplessp-config-version.cmake
        VERSION ${CMAKE_PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion)
install(
        FILES
        ${CMAKE_CURRENT_BINARY_DIR}/simplessp-config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/simplessp-config-version.cmake
        DESTINATION
        ${CMAKE_INSTALL_DATADIR}/simplessp)

install(TARGETS simplessp EXPORT simplessp-targets)
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(EXPORT simplessp-targets
        NAMESPACE simplessp::
        DESTINATION ${CMAKE_INSTALL_DATADIR}/simplessp)
