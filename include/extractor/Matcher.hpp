#ifndef EXTRACTOR_MATCHER
#define EXTRACTOR_MATCHER

#include <algorithm>
#include <map>
#include <string>

#include "extractor/TypeConversion.hpp"
#include "extractor/Utils.hpp"
#include "llvm/Support/raw_ostream.h"
#include "clang/AST/Decl.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

namespace ca = clang::ast_matchers;

namespace match
{

    struct FunctionId
    {
        std::string path;
        std::string name;
        std::string signature;
    };

    bool operator<(const FunctionId &first, const FunctionId &second);

    struct FunctionInfo
    {
        FunctionInfo();

        uint cyclomaticComplexity;
        // TODO: other metrics
    };

    class MatcherBase
    {
    public:
        virtual ~MatcherBase();

        static std::map<FunctionId, FunctionInfo> getStats();

    protected:
        static std::map<FunctionId, FunctionInfo> stats;
    };

    template<typename T>
    class Matcher : public MatcherBase, public ca::MatchFinder::MatchCallback
    {
    public:
        virtual void run(const ca::MatchFinder::MatchResult &result);
    };

    // Implementation of template methods

    template<typename T>
    void Matcher<T>::run(const ca::MatchFinder::MatchResult &result)
    {
        std::string functionStr = utils::getStringByType<clang::FunctionDecl>();
        std::string nodeStr = utils::getStringByType<T>();

        // Get matched AST node
        const clang::FunctionDecl *fd = result.Nodes.getNodeAs<clang::FunctionDecl>(functionStr);
        const T *node = result.Nodes.getNodeAs<T>(nodeStr);

        if (!fd || !node)
        {
            return;
        }

        // Check if it is not a function template
        auto functionTemplate = clang::FunctionDecl::TemplatedKind::TK_FunctionTemplate;

        if (fd->getTemplatedKind() == functionTemplate)
        {
            return;
        }

        // Check if statement is valid
        // Invalid statements are:
        // - a switch case without body
        // - a non-logical binary operator
        if (!utils::isValidStmt<T>(node))
        {
            return;
        }

        // Get function signature (weird method)
        // TODO: create method for it
        std::string out;
        llvm::raw_string_ostream ostr(out);
        fd->dump(ostr);

        size_t pos = out.find("\n");
        if (pos == std::string::npos)
        {
            llvm::errs() << "Error while parsing AST (first line not found)\n";
            return;
        }

        std::string line = out.substr(0, pos);
        size_t lastQuote = line.find_last_of("'");
        size_t firstQuote = line.find_last_of("'", lastQuote - 1);

        if (firstQuote == std::string::npos || lastQuote == std::string::npos)
        {
            llvm::errs() << "Error while parsing AST (signature not found)\n";
            return;
        }

        FunctionId fid;
        fid.name = fd->getQualifiedNameAsString();
        fid.path = result.Context->getSourceManager().getFilename(fd->getLocation());
        fid.signature = line.substr(firstQuote, lastQuote);

        // TODO: log what has been found (pretty)
        llvm::outs() << "An object '" << nodeStr << "' was found at " << fid.path << ", line "
                     << result.Context->getSourceManager().getSpellingLineNumber(node->getLocStart()) << "\n";

        // Increase cyclomatic number
        ++stats[fid].cyclomaticComplexity;
    }

} // namespace match

#endif // EXTRACTOR_MATCHER
