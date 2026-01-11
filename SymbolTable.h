#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <string>
#include <map>

constexpr int totalRegister = 15;
class SymbolTable
{
public:
    SymbolTable(); // Constructor to initialize pre-defined symbols
    void addEntry(std::string symbol, int address);
    bool contains(std::string symbol);
    int getAddress(std::string symbol);

private:
    std::map<std::string, int> symbols;
};

#endif