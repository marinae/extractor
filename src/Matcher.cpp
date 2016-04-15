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

    std::string MatcherBase::getSignature(const clang::FunctionDecl *decl)
    {
        if (!decl)
        {
            return std::string();
        }

        // Check if declaration was implicitly generated
        if (decl->isImplicit())
        {
            return std::string();
        }

        // Check if it is not a function template
        auto functionTemplate = clang::FunctionDecl::TemplatedKind::TK_FunctionTemplate;
        if (decl->getTemplatedKind() == functionTemplate)
        {
            return std::string();
        }

        // Parse function signature from AST (there is no method to get it!)
        return utils::parseSignature(decl);
    }

    void SimpleMatcher::run(const ca::MatchFinder::MatchResult &result)
    {
        // Get matched AST node
        std::string functionStr = utils::getStringByType<clang::FunctionDecl>();
        const clang::FunctionDecl *fd = result.Nodes.getNodeAs<clang::FunctionDecl>(functionStr);

        // Check if object is valid and get function signature
        std::string signature = getSignature(fd);
        if (signature.empty())
        {
            return;
        }

        const clang::SourceManager &sm = result.Context->getSourceManager();
        const clang::SourceLocation loc = fd->getLocStart();

        FunctionId fid;
        fid.path      = sm.getFilename(fd->getLocation());
        fid.name      = fd->getQualifiedNameAsString();
        fid.signature = signature;
        fid.line      = sm.getSpellingLineNumber(loc);
        fid.column    = sm.getSpellingColumnNumber(loc);

        // Log match
        utils::logMatch(fid.path, fid.line, fid.column, functionStr);

        // Increase cyclomatic number
        ++stats[fid].cyclomaticN;
    }

} // namespace match
