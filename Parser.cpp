#include "Parser.h"
#include <algorithm>

Parser::Parser(std::string filename) : file(filename) {}

bool Parser::hasMoreLines()
{
    return !file.eof();
}

void Parser::advance()
{
    std::getline(file, currentLine);
    cleanLine();
    // If the line was just a comment or empty, skip it
    if (currentLine.empty() && hasMoreLines())
    {
        advance();
    }
}

void Parser::cleanLine()
{
    // 1. Remove comments (anything after //)
    size_t commentPos{ currentLine.find("//") };
    if (commentPos != std::string::npos)
    {
        currentLine = currentLine.substr(0, commentPos);
    }

    // 2. Remove all whitespace
    currentLine.erase(std::remove_if(currentLine.begin(), currentLine.end(), ::isspace), currentLine.end());
}

CommandType Parser::commandType()
{
    if (currentLine.empty()) return INVALID;
    if (currentLine[0] == '@') return A_COMMAND;
    if (currentLine[0] == '(') return L_COMMAND;
    return C_COMMAND;
}

std::string Parser::symbol()
{
    if (commandType() == A_COMMAND)
    {
        return currentLine.substr(1); // Remove '@'
    }
    else if (commandType() == L_COMMAND)
    {
        return currentLine.substr(1, currentLine.length() - 2); // Remove '(' and ')'
    }
    return "";
}

std::string Parser::comp()
{
    // Logic: find the part between '=' and ';'
    size_t eq{ currentLine.find('=') };
    size_t sc{ currentLine.find(';') };

    size_t start{ (eq == std::string::npos) ? 0 : eq + 1 };
    size_t end{ (sc == std::string::npos) ? currentLine.length() : sc };

    return currentLine.substr(start, end - start);
}

std::string Parser::dest()
{
    size_t eq{ currentLine.find('=') };
    return (eq == std::string::npos) ? "" : currentLine.substr(0, eq);
}

std::string Parser::jump()
{
    size_t sc{ currentLine.find(';') };
    return (sc == std::string::npos) ? "" : currentLine.substr(sc + 1);
}