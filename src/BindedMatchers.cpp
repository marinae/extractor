#include "extractor/BindedMatchers.hpp"
#include "extractor/TypeConversion.hpp"

namespace ca = clang::ast_matchers;

namespace match
{

    template<>
    const StmtMatcher getStmtMatcher<clang::IfStmt>()
    {
        return ca::ifStmt().bind(utils::getStringByType<clang::IfStmt>());
    }

    template<>
    const StmtMatcher getStmtMatcher<clang::ForStmt>()
    {
        return ca::forStmt().bind(utils::getStringByType<clang::ForStmt>());
    }

    template<>
    const StmtMatcher getStmtMatcher<clang::CXXForRangeStmt>()
    {
        return ca::cxxForRangeStmt().bind(utils::getStringByType<clang::CXXForRangeStmt>());
    }

    template<>
    const StmtMatcher getStmtMatcher<clang::WhileStmt>()
    {
        return ca::whileStmt().bind(utils::getStringByType<clang::WhileStmt>());
    }

    template<>
    const StmtMatcher getStmtMatcher<clang::DoStmt>()
    {
        return ca::doStmt().bind(utils::getStringByType<clang::DoStmt>());
    }

    template<>
    const StmtMatcher getStmtMatcher<clang::CaseStmt>()
    {
        return ca::caseStmt().bind(utils::getStringByType<clang::CaseStmt>());
    }

    template<>
    const StmtMatcher getStmtMatcher<clang::CXXCatchStmt>()
    {
        return ca::cxxCatchStmt().bind(utils::getStringByType<clang::CXXCatchStmt>());
    }

    template<>
    const StmtMatcher getStmtMatcher<clang::BinaryOperator>()
    {
        return ca::binaryOperator().bind(utils::getStringByType<clang::BinaryOperator>());
    }

    template<>
    const StmtMatcher getStmtMatcher<clang::ConditionalOperator>()
    {
        return ca::conditionalOperator().bind(utils::getStringByType<clang::ConditionalOperator>());
    }

    template<>
    const StmtMatcher getStmtMatcher<clang::BinaryConditionalOperator>()
    {
        return ca::binaryConditionalOperator().bind(utils::getStringByType<clang::BinaryConditionalOperator>());
    }

} // namespace match
