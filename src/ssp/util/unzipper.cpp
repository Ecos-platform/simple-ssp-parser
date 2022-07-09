
#include "unzipper.hpp"

#include <zip.h>

#include <fstream>
#include <string>

using namespace ssp;

bool ssp::unzip(const fs::path& zip_file, const fs::path& tmp_path)
{
    int* err = nullptr;
    zip* za = zip_open(absolute(zip_file).string().c_str(), 0, err);
    if (za == nullptr) {
        return false;
    }

    struct zip_file* zf;
    // clang-format off
        struct zip_stat sb{};
    // clang-format on

    const int bufferSize = 1000;
    std::vector<char> contents(bufferSize);
    zip_int64_t sum;
    zip_int64_t len;
    for (int i = 0; i < zip_get_num_entries(za, 0); i++) {
        if (zip_stat_index(za, i, 0, &sb) == 0) {

            const fs::path newFile = tmp_path / sb.name;

            if (sb.size == 0) {
                fs::create_directories(newFile);
            } else {
                const auto containingDirectory = newFile.parent_path();
                if (!fs::exists(containingDirectory) && !fs::create_directories(containingDirectory)) {
                    return false;
                }
                zf = zip_fopen_index(za, i, 0);

                std::ofstream file;
                file.open(newFile, std::ios::out | std::ios::binary);

                sum = 0;
                while (sum != sb.size) {
                    len = zip_fread(zf, contents.data(), bufferSize);
                    file.write(contents.data(), len);
                    sum += len;
                }

                file.flush();
                file.close();

                zip_fclose(zf);
            }
        }
    }
    zip_close(za);

    return true;
}
