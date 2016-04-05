#ifndef EXTRACTOR_BINDED_MATCHERS
#define EXTRACTOR_BINDED_MATCHERS

#include <string>

#include "clang/AST/Decl.h"
#include "clang/AST/Stmt.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

namespace ca = clang::ast_matchers;

namespace match
{

    typedef ca::internal::Matcher<clang::Decl> FunctionDeclMatcher;

    template<typename T>
    const FunctionDeclMatcher getMatcher(const std::string &path);

    template<>
    const FunctionDeclMatcher getMatcher<clang::IfStmt>(const std::string &path);

    template<>
    const FunctionDeclMatcher getMatcher<clang::ForStmt>(const std::string &path);

    template<>
    const FunctionDeclMatcher getMatcher<clang::CXXForRangeStmt>(const std::string &path);

} // namespace match

#endif // EXTRACTOR_BINDED_MATCHERS
