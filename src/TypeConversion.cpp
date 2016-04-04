#include "extractor/TypeConversion.hpp"

namespace utils
{

    template<>
    std::string getStringByType<clang::FunctionDecl>()
    {
        return std::string("function");
    }

    template<>
    std::string getStringByType<clang::IfStmt>()
    {
        return std::string("if");
    }

    template<>
    std::string getStringByType<clang::ForStmt>()
    {
        return std::string("for");
    }

    template<>
    std::string getStringByType<clang::CXXForRangeStmt>()
    {
        return std::string("range");
    }

    template<>
    std::string getStringByType<clang::WhileStmt>()
    {
        return std::string("while");
    }

    template<>
    std::string getStringByType<clang::DoStmt>()
    {
        return std::string("doWhile");
    }

    template<>
    std::string getStringByType<clang::SwitchCase>()
    {
        return std::string("case");
    }

    template<>
    std::string getStringByType<clang::CXXCatchStmt>()
    {
        return std::string("catch");
    }

    template<>
    std::string getStringByType<clang::BinaryOperator>()
    {
        return std::string("binaryOp");
    }

    template<>
    std::string getStringByType<clang::ConditionalOperator>()
    {
        return std::string("condOp");
    }

    template<>
    std::string getStringByType<clang::BinaryConditionalOperator>()
    {
        return std::string("binaryCondOp");
    }

} // namespace utils
