#pragma once

#include <cassert>
#include <cstdio>
#include <memory>
#include <string>

// TODO: use string_view's?


namespace util {

// Hmmm... I hope something will work :-)
#if defined(_WINDOWS) || defined(WINDOWS) || defined(_WIN32) ||  defined(WIN32) ||  defined(_WIN64) ||  defined(WIN64)
const char kPathDelimiter = '\\';
#else
const char kPathDelimiter = '/';
#endif

// TODO: Arbitrary arguments number
std::string joinPath(const std::string& beginning, const std::string& ending);

class SmartFileHandler {
public:
  SmartFileHandler(const std::string& path, const std::string& mode) :
      fileHandle_(fopen(path.c_str(), mode.c_str()), &fclose) {
  }

  FILE* get() const {
    return fileHandle_.get();
  }

private:
  std::unique_ptr<FILE, decltype(&fclose)> fileHandle_;
};

void skipWhitespace(FILE* file);

}  // namespace util
