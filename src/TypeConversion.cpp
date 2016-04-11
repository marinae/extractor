#include "extractor/TypeConversion.hpp"

namespace utils
{

    template<>
    std::string getStringByType<clang::FunctionDecl>()
    {
        return std::string("FunctionDecl");
    }

    template<>
    std::string getStringByType<clang::IfStmt>()
    {
        return std::string("IfStmt");
    }

    template<>
    std::string getStringByType<clang::ForStmt>()
    {
        return std::string("ForStmt");
    }

    template<>
    std::string getStringByType<clang::CXXForRangeStmt>()
    {
        return std::string("CXXForRangeStmt");
    }

    template<>
    std::string getStringByType<clang::WhileStmt>()
    {
        return std::string("WhileStmt");
    }

    template<>
    std::string getStringByType<clang::DoStmt>()
    {
        return std::string("DoStmt");
    }

    template<>
    std::string getStringByType<clang::CaseStmt>()
    {
        return std::string("CaseStmt");
    }

    template<>
    std::string getStringByType<clang::CXXCatchStmt>()
    {
        return std::string("CXXCatchStmt");
    }

    template<>
    std::string getStringByType<clang::BinaryOperator>()
    {
        return std::string("BinaryOperator");
    }

    template<>
    std::string getStringByType<clang::ConditionalOperator>()
    {
        return std::string("ConditionalOperator");
    }

    template<>
    std::string getStringByType<clang::BinaryConditionalOperator>()
    {
        return std::string("BinaryConditionalOperator");
    }

} // namespace utils
