#include "extractor/Matcher.hpp"

namespace match
{

    bool operator<(const FunctionId &first, const FunctionId &second)
    {
        if (first.path != second.path)
        {
            return (first.path < second.path);
        }
        else if (first.name != second.name)
        {
            return (first.name < second.name);
        }
        else if (first.signature != second.signature)
        {
            return (first.signature < second.signature);
        }
        return false;
    }

    std::map<FunctionId, FunctionInfo> MatcherBase::stats;

    FunctionInfo::FunctionInfo()
    : cyclomaticN(0)
    {}

    MatcherBase::~MatcherBase()
    {}

    std::map<FunctionId, FunctionInfo> MatcherBase::getStats()
    {
        return stats;
    }

    void SimpleMatcher::run(const ca::MatchFinder::MatchResult &result)
    {
        // Get matched AST node
        std::string functionStr = utils::getStringByType<clang::FunctionDecl>();
        const clang::FunctionDecl *fd = result.Nodes.getNodeAs<clang::FunctionDecl>(functionStr);

        if (!fd)
        {
            return;
        }

        // Check if declaration was implicitly generated
        if (fd->isImplicit())
        {
            return;
        }

        // Check if it is not a function template
        auto functionTemplate = clang::FunctionDecl::TemplatedKind::TK_FunctionTemplate;

        if (fd->getTemplatedKind() == functionTemplate)
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
        const clang::SourceLocation loc = fd->getLocStart();

        // Fill map key
        FunctionId fid;
        fid.path = sm.getFilename(fd->getLocation());
        fid.name = fd->getQualifiedNameAsString();
        fid.signature = signature;

        // Log match
        utils::logMatch(fid.path, sm.getSpellingLineNumber(loc), sm.getSpellingColumnNumber(loc), functionStr);

        // Increase cyclomatic number
        ++stats[fid].cyclomaticN;
    }

} // namespace match
