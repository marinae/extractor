#ifndef EXTRACTOR_BINDED_MATCHERS
#define EXTRACTOR_BINDED_MATCHERS

#include <string>

#include "extractor/TypeConversion.hpp"
#include "clang/AST/Decl.h"
#include "clang/AST/Stmt.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

namespace ca = clang::ast_matchers;

namespace match
{

    typedef ca::internal::Matcher<clang::Decl> DeclMatcher;
    typedef ca::internal::Matcher<clang::Stmt> StmtMatcher;

    template<typename T>
    const StmtMatcher getStmtMatcher();

    template<>
    const StmtMatcher getStmtMatcher<clang::IfStmt>();

    template<>
    const StmtMatcher getStmtMatcher<clang::ForStmt>();

    template<>
    const StmtMatcher getStmtMatcher<clang::CXXForRangeStmt>();

    template<>
    const StmtMatcher getStmtMatcher<clang::WhileStmt>();

    template<>
    const StmtMatcher getStmtMatcher<clang::DoStmt>();

    template<>
    const StmtMatcher getStmtMatcher<clang::SwitchCase>();

    template<>
    const StmtMatcher getStmtMatcher<clang::CXXCatchStmt>();

    template<>
    const StmtMatcher getStmtMatcher<clang::BinaryOperator>();

    template<>
    const StmtMatcher getStmtMatcher<clang::ConditionalOperator>();

    template<>
    const StmtMatcher getStmtMatcher<clang::BinaryConditionalOperator>();

    template<typename T>
    const DeclMatcher getMatcher(const std::string &path)
    {
        std::string functionStr = utils::getStringByType<clang::FunctionDecl>();

        return ca::functionDecl(ca::allOf(ca::isExpansionInFileMatching(path),
                                          ca::isDefinition(),
                                          ca::forEachDescendant(getStmtMatcher<T>()))
                               ).bind(functionStr);
    }

} // namespace match

#endif // EXTRACTOR_BINDED_MATCHERS
