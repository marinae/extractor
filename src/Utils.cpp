#include <iostream>
#include <string>

#include "extractor/Utils.hpp"
#include "clang/Tooling/CompilationDatabase.h"
#include "clang/Tooling/JSONCompilationDatabase.h"

namespace utils
{

    void generateUsageStr()
    {
        const std::string usage("Usage: ./extractor <path-to-project-dir> -- <compiler-arguments>");
        std::cout << usage << "\n";
    }

    std::unique_ptr<ct::JSONCompilationDatabase> loadDB(const std::string &path)
    {
        std::string errorMessage;

        auto db = ct::JSONCompilationDatabase::loadFromFile(path, errorMessage);
        if (!db)
        {
            std::cerr << "Error while parsing JSON file: '" << errorMessage << "'\n";
            return {};
        }
        else
        {
            return db;
        }
    }

} // namespace utils
