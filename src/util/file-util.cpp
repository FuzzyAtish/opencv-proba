#include "file-util.hh"

std::string utcn::ip::FileUtil::getSingleFileAbsPath() {
  // const std::vector<std::string> selectedFile = pfd::open_file("Select a single file", DEFAULT_IMAGES_PATH).result();
  const std::vector<std::string> selectedFile = pfd::open_file("Select a single file").result();
  if (!selectedFile.empty()) {
    return std::accumulate(selectedFile.begin(), selectedFile.end(), std::string{});
  }
  return "";
}

std::string utcn::ip::FileUtil::getDirectoryAbsPath() {
  // const auto dir = pfd::select_folder("Select any directory", ASSETS_DIR).result();
  const auto dir = pfd::select_folder("Select any directory").result();
  return dir.empty() ? "" : dir;
}