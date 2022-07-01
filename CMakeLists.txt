cmake_minimum_required(VERSION 3.15)
file(STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/version.txt" projectVersion)
project(simple_ssp_parser VERSION ${projectVersion})
message("Current ${PROJECT_NAME} version: ${CMAKE_PROJECT_VERSION}\n")

# ==============================================================================
# Build settings
# ==============================================================================

option(SSP_BUILD_TESTS "Build tests" OFF)


# ==============================================================================
# Global internal configuration
# ==============================================================================

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# ==============================================================================
# Dependencies
# ==============================================================================

if (CONAN_EXPORTED) # in conan local cache
    # standard conan installation, deps will be defined in conanfile.py
    # and not necessary to call conan again, conan is already running
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup(TARGETS)
else ()

    include(cmake/conan.cmake)

    conan_cmake_run(
            CONANFILE conanfile.py
            BUILD missing
            BASIC_SETUP CMAKE_TARGETS)

endif ()

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
