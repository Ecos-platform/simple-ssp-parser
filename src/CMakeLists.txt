set(publicHeaderDir ${CMAKE_CURRENT_SOURCE_DIR}/../include)


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

        "ssp/util/random.hpp"
        "ssp/util/unzipper.hpp"

        )


# ==============================================================================
# sources
# ==============================================================================

set(sources
        "ssp/ssp.cpp"
        "ssp/util/temp_dir.cpp"
        )


# ==============================================================================
# targets
# ==============================================================================

add_library(ssp ${sources} ${publicHeadersFull})
target_compile_features(ssp PUBLIC "cxx_std_17")
target_include_directories(ssp
        PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>"
        PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}"
        )
target_link_libraries(ssp PUBLIC CONAN_PKG::pugixml PRIVATE CONAN_PKG::libzip)
if (UNIX)
    target_link_libraries(ssp INTERFACE stdc++fs)
endif ()



# ==============================================================================
# Installation rules
# ==============================================================================

install(
        TARGETS ssp
        EXPORT "${SSP_EXPORT_TARGET}"
        ${SSP_INSTALL_DESTINATIONS}
)
install(
        DIRECTORY "${publicHeaderDir}/ssp"
        DESTINATION "${SSP_HEADER_INSTALL_DIR}"
)
