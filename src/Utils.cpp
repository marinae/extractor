#include <iostream>
#include <string>

#include "extractor/Utils.hpp"
#include "llvm/Support/raw_ostream.h"
#include "clang/Tooling/CompilationDatabase.h"
#include "clang/Tooling/JSONCompilationDatabase.h"

namespace utils
{

    void generateUsageStr()
    {
        const std::string usage("Usage: ./extractor <path-to-project-dir> -- <compiler-arguments>");
        std::cout << usage << "\n";
    }

    std::unique_ptr<ct::JSONCompilationDatabase> loadDB(const std::string &path)
    {
        std::string errorMessage;

        auto db = ct::JSONCompilationDatabase::loadFromFile(path, errorMessage);
        if (!db)
        {
            std::cerr << "Error while parsing JSON file: '" << errorMessage << "'\n";
            return {};
        }
        else
        {
            return db;
        }
    }

    template<>
    bool isValidStmt(const clang::CaseStmt *stmt)
    {
        // TODO: try not to hardcode
        std::string buf;
        llvm::raw_string_ostream ostr(buf);

        stmt->getSubStmt()->dump(ostr);
        const std::string &out = ostr.str();

        return (out.substr(0, 8) != "CaseStmt");
    }

    template<>
    bool isValidStmt(const clang::BinaryOperator *stmt)
    {
        clang::BinaryOperator::Opcode op = stmt->getOpcode();

        return (op == clang::BinaryOperator::Opcode::BO_LAnd) ||
               (op == clang::BinaryOperator::Opcode::BO_LOr);
    }

    std::string parseSignature(const clang::FunctionDecl *decl)
    {
        // TODO: try doing it other way
        std::string buf;
        llvm::raw_string_ostream ostr(buf);

        decl->dump(ostr);
        const std::string &out = ostr.str();

        size_t pos = out.find("\n");
        if (pos == std::string::npos)
        {
            std::cerr << "Error while parsing AST (first line not found)\n";
            return std::string();
        }

        std::string line = out.substr(0, pos);
        size_t lastQuote = line.find_last_of("'");
        size_t firstQuote = line.find_last_of("'", lastQuote - 1);

        if (firstQuote == std::string::npos || lastQuote == std::string::npos)
        {
            std::cerr << "Error while parsing AST (signature not found)\n";
            return std::string();
        }

        return line.substr(firstQuote, lastQuote);
    }

    void logMatch(const std::string &path, unsigned line, unsigned column, const std::string &object)
    {
        std::cout << "Match at " << path << ", line " << line << ", col " << column << ":\t'" << object << "'\n";
    }

} // namespace utils
