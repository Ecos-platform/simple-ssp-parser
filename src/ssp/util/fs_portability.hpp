
#ifndef SSP_FS_PORTABILITY_HPP
#define SSP_FS_PORTABILITY_HPP

#if __has_include(<filesystem>)

#    include <filesystem>

namespace ssp {
    namespace fs = std::filesystem;
}
#else
#    include <experimental/filesystem>
namespace vico
{
namespace fs = std::experimental::filesystem;
}
#endif

#endif // SSP_FS_PORTABILITY_HPP
