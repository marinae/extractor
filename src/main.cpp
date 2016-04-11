#include <fstream>
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
    match::Matcher<clang::WhileStmt> whileHandler;
    match::Matcher<clang::DoStmt> doHandler;
    match::Matcher<clang::CaseStmt> caseHandler;
    match::Matcher<clang::CXXCatchStmt> catchHandler;
    match::Matcher<clang::BinaryOperator> binaryOpHandler;
    match::Matcher<clang::ConditionalOperator> condOpHandler;
    match::Matcher<clang::BinaryConditionalOperator> binaryCondOpHandler;

    finder.addMatcher(match::getMatcher<clang::IfStmt>(pathRegex), &ifHandler);
    finder.addMatcher(match::getMatcher<clang::ForStmt>(pathRegex), &forHandler);
    finder.addMatcher(match::getMatcher<clang::CXXForRangeStmt>(pathRegex), &rangeHandler);
    finder.addMatcher(match::getMatcher<clang::WhileStmt>(pathRegex), &whileHandler);
    finder.addMatcher(match::getMatcher<clang::DoStmt>(pathRegex), &doHandler);
    finder.addMatcher(match::getMatcher<clang::CaseStmt>(pathRegex), &caseHandler);
    finder.addMatcher(match::getMatcher<clang::CXXCatchStmt>(pathRegex), &catchHandler);
    finder.addMatcher(match::getMatcher<clang::BinaryOperator>(pathRegex), &binaryOpHandler);
    finder.addMatcher(match::getMatcher<clang::ConditionalOperator>(pathRegex), &condOpHandler);
    finder.addMatcher(match::getMatcher<clang::BinaryConditionalOperator>(pathRegex), &binaryCondOpHandler);

    // Run tool
    int rc = tool.run(ct::newFrontendActionFactory(&finder).get());
    if (rc)
    {
        utils::generateUsageStr();
        return -1;
    }

    // Output results
    std::ofstream output;
    output.open("results.txt");
    if (!output.is_open())
    {
        std::cerr << "Error while opening file for writing\n";
        return -1;
    }
    output << "Path\tName\tSignature\tCyclomaticN\n";
    auto stats = match::MatcherBase::getStats();
    for (auto it = stats.begin(); it != stats.end(); ++it)
    {
        output << it->first.path << "\t"
               << it->first.name << "\t"
               << it->first.signature << "\t"
               << it->second.cyclomaticComplexity << "\n";
    }

    return 0;
}
