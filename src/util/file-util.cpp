#include "file-util.hh"

std::string utcn::ip::FileUtil::getFileOrDir(const bool isFile) {
  NFD::Guard nfdGuard;
  NFD::UniquePath outPath;
  nfdresult_t result;
  if (isFile) {
    result = NFD::OpenDialog(outPath);
  } else {
    result = NFD::PickFolder(outPath);
  }
  if (result == NFD_OKAY) {
    return outPath.get();
  }
  return "";
}

std::string utcn::ip::FileUtil::getSingleFileAbsPath() {
  return getFileOrDir();
}

std::string utcn::ip::FileUtil::getDirectoryAbsPath() {
  return getFileOrDir(false);
}