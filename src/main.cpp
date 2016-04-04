#include <iostream>
#include <string>
#include <vector>

#include "extractor/Matcher.hpp"
#include "extractor/Utils.hpp"
#include "clang/AST/Decl.h"
#include "clang/AST/Stmt.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Tooling/Tooling.h"

namespace ct = clang::tooling;
namespace ca = clang::ast_matchers;

int main(int argc, char *argv[])
{
    std::vector<std::string> args(argv + 1, argv + argc);

    if (args.size() < 1 || (args.size() > 1 && args[1] != "--"))
    {
        utils::generateUsageStr();
        return -1;
    }

    const std::string path(args[0]);
    const std::string json("compile_commands.json");

    // Load compile commands
    auto db = utils::loadDB(path + json);
    if (!db)
    {
        utils::generateUsageStr();
        return -1;
    }

    ct::ClangTool tool(*db, db->getAllFiles());

    // Pass compiler options
    if (args.size() > 2)
    {
        std::vector<std::string> compilerArgs(args.begin() + 2, args.end());
        ct::ArgumentInsertPosition pos = ct::ArgumentInsertPosition::END;

        tool.appendArgumentsAdjuster(ct::getInsertArgumentAdjuster(compilerArgs, pos));
    }

    // Add matchers
    ca::MatchFinder finder;

    match::Matcher<clang::IfStmt> ifPrinter;
    match::Matcher<clang::ForStmt> forPrinter;
    match::Matcher<clang::CXXForRangeStmt> rangePrinter;

    // TODO: move to utils
    auto ifMatcher = ca::ifStmt().bind(utils::getStringByType<clang::IfStmt>());
    auto forMatcher = ca::forStmt().bind(utils::getStringByType<clang::ForStmt>());
    auto rangeMatcher = ca::cxxForRangeStmt().bind(utils::getStringByType<clang::CXXForRangeStmt>());

    auto functionIfMatcher = ca::functionDecl(ca::allOf(ca::isExpansionInFileMatching(path + "*"),
                                                        ca::isDefinition(),
                                                        ca::forEachDescendant(ifMatcher)
                                                        )).bind("function");
    auto functionForMatcher = ca::functionDecl(ca::allOf(ca::isExpansionInFileMatching(path + "*"),
                                                         ca::isDefinition(),
                                                         ca::forEachDescendant(forMatcher)
                                                         )).bind("function");
    auto functionRangeMatcher = ca::functionDecl(ca::allOf(ca::isExpansionInFileMatching(path + "*"),
                                                         ca::isDefinition(),
                                                         ca::forEachDescendant(rangeMatcher)
                                                         )).bind("function");
    //

    // TODO: find just all methods!
    finder.addMatcher(functionIfMatcher, &ifPrinter);
    finder.addMatcher(functionForMatcher, &forPrinter);
    finder.addMatcher(functionRangeMatcher, &rangePrinter);

    // Run tool
    int rc = tool.run(ct::newFrontendActionFactory(&finder).get());
    if (rc)
    {
        utils::generateUsageStr();
        return -1;
    }

    // TODO: pretty print
    // Get results
    auto stats = match::MatcherBase::getStats();
    for (auto it = stats.begin(); it != stats.end(); ++it)
    {
        std::cout << it->first.path << " "
                  << it->first.name << " "
                  << it->first.signature << " "
                  << it->second.cyclomaticComplexity << "\n";
    }
}
