#include "extractor/BindedMatchers.hpp"
#include "extractor/TypeConversion.hpp"

namespace ca = clang::ast_matchers;

namespace match
{

    template<>
    const FunctionDeclMatcher getMatcher<clang::IfStmt>(const std::string &path)
    {
        std::string functionStr = utils::getStringByType<clang::FunctionDecl>();
        std::string nodeStr = utils::getStringByType<clang::IfStmt>();

        return ca::functionDecl(ca::allOf(ca::isExpansionInFileMatching(path),
                                          ca::isDefinition(),
                                          ca::forEachDescendant(ca::ifStmt().bind(nodeStr)))
                               ).bind(functionStr);
    }

    template<>
    const FunctionDeclMatcher getMatcher<clang::ForStmt>(const std::string &path)
    {
        std::string functionStr = utils::getStringByType<clang::FunctionDecl>();
        std::string nodeStr = utils::getStringByType<clang::ForStmt>();

        return ca::functionDecl(ca::allOf(ca::isExpansionInFileMatching(path),
                                          ca::isDefinition(),
                                          ca::forEachDescendant(ca::forStmt().bind(nodeStr)))
                               ).bind(functionStr);
    }

    template<>
    const FunctionDeclMatcher getMatcher<clang::CXXForRangeStmt>(const std::string &path)
    {
        std::string functionStr = utils::getStringByType<clang::FunctionDecl>();
        std::string nodeStr = utils::getStringByType<clang::CXXForRangeStmt>();

        return ca::functionDecl(ca::allOf(ca::isExpansionInFileMatching(path),
                                          ca::isDefinition(),
                                          ca::forEachDescendant(ca::cxxForRangeStmt().bind(nodeStr)))
                               ).bind(functionStr);
    }

} // namespace match
