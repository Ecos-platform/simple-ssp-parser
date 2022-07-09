
#ifndef SSP_TEMP_DIR_HPP
#define SSP_TEMP_DIR_HPP

#include "ssp/util/fs_portability.hpp"

#include <string>

namespace ssp
{

class temp_dir
{
private:
    const fs::path path_;
    bool deleteTemporalFolder_{true};

public:
    explicit temp_dir(const std::string& name);
    temp_dir(const temp_dir&) = delete;
    temp_dir(const temp_dir&&) = delete;

    [[nodiscard]] fs::path path()
    {
        return path_;
    }

    void set_delete_temp_dir_on_destruction(bool flag) {
        deleteTemporalFolder_ = flag;
    }

    ~temp_dir();
};

} // namespace ssp


#endif // SSP_TEMP_DIR_HPP
