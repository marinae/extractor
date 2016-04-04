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
    : cyclomaticComplexity(1)
    {}

    MatcherBase::~MatcherBase()
    {}

    std::map<FunctionId, FunctionInfo> MatcherBase::getStats()
    {
        return stats;
    }

} // namespace match
