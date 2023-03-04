#ifndef FILE_UTIL_HH
#define FILE_UTIL_HH

#include <filesystem>
#include <iostream>
#include <numeric>
#include <vector>

#include "nfd.hpp"

namespace fs = std::filesystem;

namespace utcn::ip {
class FileUtil {
 private:
  static inline std::string DEFAULT_IMAGES_PATH = ASSETS_DIR "Images/*";
  static std::string getFileOrDir(const bool isFile = true);

 public:
  static std::string getSingleFileAbsPath();
  static std::string getDirectoryAbsPath();
  static std::vector<std::string> getAllFilesInDirectory();
};
}  // namespace utcn::ip

#endif  // FILE_UTIL_HH
