
#ifndef VICO_TEMP_DIR_HPP
#define VICO_TEMP_DIR_HPP

#include "random.hpp"

#include "ssp/util/fs_portability.hpp"

#include <iostream>
#include <string>

namespace ssp
{
std::string generate_simple_id()
{
    const int len = 6;
    std::string id;
    fixed_range_random_generator rng(0, 9);
    for (auto i = 0; i < len; i++) {
        id += std::to_string(rng.next());
    }
    return id;
}

class temp_dir
{
private:
    const fs::path path_;

public:
    explicit temp_dir(const std::string& name)
        : path_(fs::temp_directory_path() /= "vico_" + name + "_" + generate_simple_id())
    {
        fs::create_directories(path_);
    }

    [[nodiscard]] fs::path path()
    {
        return path_;
    }

    ~temp_dir()
    {
        std::error_code status;
        fs::remove_all(path_, status);
        if (status) {
            std::cerr << "Failed to remove temp folder '" << path_.string() << "': " << status.message()
                      << std::endl;
        }
    }
};

} // namespace ssp


#endif // VICO_TEMP_DIR_HPP
