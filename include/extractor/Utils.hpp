#ifndef EXTRACTOR_UTILS
#define EXTRACTOR_UTILS

#include <memory>
#include <string>

#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Tooling/CompilationDatabase.h"
#include "clang/Tooling/JSONCompilationDatabase.h"

namespace ct = clang::tooling;

namespace utils
{

    void generateUsageStr();

    std::unique_ptr<ct::JSONCompilationDatabase> loadDB(const std::string &path);

    template<typename T>
    bool isValidStmt(const T *)
    {
        return true;
    }

    // TODO: check for non-empty body
    /*template<>
    bool isValidStmt(const clang::CaseStmt *stmt);*/

    template<>
    bool isValidStmt(const clang::BinaryOperator *stmt);

    std::string parseSignature(const clang::FunctionDecl *decl);

} // namespace utils

#endif // EXTRACTOR_UTILS
