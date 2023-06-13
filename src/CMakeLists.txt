set(publicHeaderDir ${PROJECT_SOURCE_DIR}/include)


# ==============================================================================
# public headers
# ==============================================================================

set(publicHeaders
        "ssp/ssp_parser.hpp"

        "util/temp_dir.hpp"
        "util/fs_portability.hpp"
        )

set(publicHeadersFull)
foreach (header IN LISTS commonPublicHeaders)
    list(APPEND commonPublicHeadersFull "${publicHeaderDir}/${header}")
endforeach ()


# ==============================================================================
# private headers
# ==============================================================================

set(privateHeaders
        "ssp/util/uuid.hpp"
        "ssp/util/unzipper.hpp"
        )


# ==============================================================================
# sources
# ==============================================================================

set(sources
        "ssp/ssp.cpp"
        "ssp/util/temp_dir.cpp"
        "ssp/util/unzipper.cpp"
        "ssp/util/uuid.cpp"
        )


# ==============================================================================
# targets
# ==============================================================================

add_library(simplessp ${sources} ${publicHeadersFull})
target_compile_features(simplessp PUBLIC "cxx_std_17")
set_property(TARGET simplessp PROPERTY POSITION_INDEPENDENT_CODE ON)
target_include_directories(simplessp
        PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>"
        PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}"
        )
target_link_libraries(simplessp PUBLIC pugixml::pugixml PRIVATE libzip::zip spdlog::spdlog)
if (UNIX)
    target_link_libraries(simplessp INTERFACE stdc++fs)
endif ()
