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
        unsigned line;
        unsigned column;
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
        std::string getSignature(const clang::FunctionDecl *decl);

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
        std::string functionStr = utils::getStringByType<clang::FunctionDecl>();
        std::string nodeStr = utils::getStringByType<T>();

        // Get matched AST node
        const clang::FunctionDecl *fd = result.Nodes.getNodeAs<clang::FunctionDecl>(functionStr);
        const T *node = result.Nodes.getNodeAs<T>(nodeStr);

        // Check if node is valid
        if (!node || !utils::isValidStmt<T>(node))
        {
            return;
        }

        // Check if object is valid and get function signature
        std::string signature = getSignature(fd);
        if (signature.empty())
        {
            return;
        }

        const clang::SourceManager &sm = result.Context->getSourceManager();
        const clang::SourceLocation loc = node->getLocStart();

        // Fill map key
        FunctionId fid;
        fid.path      = sm.getFilename(fd->getLocation());
        fid.name      = fd->getQualifiedNameAsString();
        fid.signature = signature;
        fid.line      = sm.getSpellingLineNumber(loc);
        fid.column    = sm.getSpellingColumnNumber(loc);

        // Log match
        utils::logMatch(fid.path, fid.line, fid.column, nodeStr);

        // Increase cyclomatic number
        ++stats[fid].cyclomaticN;
    }

} // namespace match

#endif // EXTRACTOR_MATCHER
