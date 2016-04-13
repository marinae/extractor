#ifndef EXTRACTOR_MATCHER
#define EXTRACTOR_MATCHER

#include <algorithm>
#include <map>
#include <string>

#include "extractor/TypeConversion.hpp"
#include "extractor/Utils.hpp"
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

        uint cyclomaticN;
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

    class SimpleMatcher : public MatcherBase, public ca::MatchFinder::MatchCallback
    {
    public:
        virtual void run(const ca::MatchFinder::MatchResult &result);
    };

    template<typename T>
    class Matcher : public MatcherBase, public ca::MatchFinder::MatchCallback
    {
    public:
        virtual void run(const ca::MatchFinder::MatchResult &result);
    };

    // Implementation of template method

    template<typename T>
    void Matcher<T>::run(const ca::MatchFinder::MatchResult &result)
    {
        // TODO: do not duplicate code
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

        // Parse function signature from AST (there is no method to get it!)
        std::string signature = utils::parseSignature(fd);
        if (signature.empty())
        {
            return;
        }

        const clang::SourceManager &sm = result.Context->getSourceManager();
        const clang::SourceLocation loc = node->getLocStart();

        // Fill map key
        FunctionId fid;
        fid.path = sm.getFilename(fd->getLocation());
        fid.name = fd->getQualifiedNameAsString();
        fid.signature = signature;

        // Log match
        utils::logMatch(fid.path, sm.getSpellingLineNumber(loc), sm.getSpellingColumnNumber(loc), nodeStr);

        // Increase cyclomatic number
        ++stats[fid].cyclomaticN;
    }

} // namespace match

#endif // EXTRACTOR_MATCHER
