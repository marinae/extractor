#ifndef EXTRACTOR_UTILS
#define EXTRACTOR_UTILS

#include <memory>
#include <string>

#include "clang/Tooling/CompilationDatabase.h"
#include "clang/Tooling/JSONCompilationDatabase.h"

namespace ct = clang::tooling;

namespace utils
{

    void generateUsageStr();

    std::unique_ptr<ct::JSONCompilationDatabase> loadDB(const std::string &path);

} // namespace utils

#endif // EXTRACTOR_UTILS
