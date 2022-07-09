
#include "ssp/util/temp_dir.hpp"

#include "uuid.hpp"

#include "ssp/util/fs_portability.hpp"

#include <spdlog/spdlog.h>

#include <string>

using namespace ssp;

temp_dir::temp_dir(const std::string& name)
    : path_(fs::temp_directory_path() /= "vico_" + name + "_" + generate_uuid())
{
    fs::create_directories(path_);
    spdlog::debug("Created temporal directory: {}", fs::absolute(path_).string());
}

temp_dir::~temp_dir()
{
    if (deleteTemporalFolder_) {
        std::error_code status;
        fs::remove_all(path_, status);
        if (status) {
            spdlog::warn("Failed to remove temp folder '{}': {}", path_.string(), status.message());
        }
    }
}
