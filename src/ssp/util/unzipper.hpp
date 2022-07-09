
#ifndef SSP_UNZIPPER_HPP
#define SSP_UNZIPPER_HPP

#include "ssp/util/fs_portability.hpp"

namespace ssp
{

bool unzip(const fs::path& zip_file, const fs::path& tmp_path);

} // namespace ssp

#endif // SSP_UNZIPPER_HPP
