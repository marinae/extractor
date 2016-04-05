#ifndef EXTRACTOR_TYPE_CONVERSION
#define EXTRACTOR_TYPE_CONVERSION

#include <string>

#include "clang/AST/Stmt.h"
#include "clang/AST/StmtCXX.h"

namespace utils
{

    template<typename T>
    std::string getStringByType();

    template<>
    std::string getStringByType<clang::FunctionDecl>();

    template<>
    std::string getStringByType<clang::IfStmt>();

    template<>
    std::string getStringByType<clang::ForStmt>();

    template<>
    std::string getStringByType<clang::CXXForRangeStmt>();

    template<>
    std::string getStringByType<clang::WhileStmt>();

    template<>
    std::string getStringByType<clang::DoStmt>();

    template<>
    std::string getStringByType<clang::CaseStmt>();

    template<>
    std::string getStringByType<clang::CXXCatchStmt>();

    template<>
    std::string getStringByType<clang::BinaryOperator>();

    template<>
    std::string getStringByType<clang::ConditionalOperator>();

    template<>
    std::string getStringByType<clang::BinaryConditionalOperator>();

} // namespace utils

#endif // EXTRACTOR_TYPE_CONVERSION
