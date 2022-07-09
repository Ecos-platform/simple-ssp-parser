
#include "ssp/util/temp_dir.hpp"

#include "uuid.hpp"

#include "ssp/util/fs_portability.hpp"

#include <iostream>
#include <string>

using namespace ssp;

temp_dir::temp_dir(const std::string& name)
    : path_(fs::temp_directory_path() /= "vico_" + name + "_" + generate_uuid())
{
    fs::create_directories(path_);
}

temp_dir::~temp_dir()
{
    std::error_code status;
    fs::remove_all(path_, status);
    if (status) {
        std::cerr << "Failed to remove temp folder '" << path_.string() << "': " << status.message()
                  << std::endl;
    }
}
