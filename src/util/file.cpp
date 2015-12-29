#include "util/file.h"


namespace util {

std::string joinPath(const std::string& beginning, const std::string& ending) {
  assert(!beginning.empty());
  assert(!ending.empty());
  assert(ending[0] != kPathDelimiter);  // only the first part can be an absolute path
  auto result = beginning;
  if (result.back() != kPathDelimiter)
    result += kPathDelimiter;
  result += ending;
  return result;
}

void skipWhitespace(FILE* file) {
  int ch;
  do {
    ch = fgetc(file);
    if (ch == -1)
      return;
  } while (isspace(ch));
  ungetc(ch, file);
}

}  // namespace util
