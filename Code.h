#ifndef CODE_H
#define CODE_H

#include <string>
#include <map>

class Code
{
public:
    static std::string dest(std::string mnemonic);
    static std::string comp(std::string mnemonic);
    static std::string jump(std::string mnemonic);

private:
    // Internal maps to store the binary translations
    static std::map<std::string, std::string> destMap;
    static std::map<std::string, std::string> compMap;
    static std::map<std::string, std::string> jumpMap;
};

#endif