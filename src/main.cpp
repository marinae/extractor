#include <iostream>
#include <string>
#include <vector>

#include "extractor/BindedMatchers.hpp"
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
    std::string pathRegex(path + "*");

    match::Matcher<clang::IfStmt> ifHandler;
    match::Matcher<clang::ForStmt> forHandler;
    match::Matcher<clang::CXXForRangeStmt> rangeHandler;

    finder.addMatcher(match::getMatcher<clang::IfStmt>(pathRegex), &ifHandler);
    finder.addMatcher(match::getMatcher<clang::ForStmt>(pathRegex), &forHandler);
    finder.addMatcher(match::getMatcher<clang::CXXForRangeStmt>(pathRegex), &rangeHandler);

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
