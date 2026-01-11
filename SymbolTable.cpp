#include "SymbolTable.h"

SymbolTable::SymbolTable()
{
    // Pre-defined R0-R15 symbols
    for (int i{ 0 }; i <= totalRegister; ++i)
    {
        symbols["R" + std::to_string(i)] = i;
    }

    // Other pre-defined pointers and I/O
    symbols["SP"] = 0;
    symbols["LCL"] = 1;
    symbols["ARG"] = 2;
    symbols["THIS"] = 3;
    symbols["THAT"] = 4;
    symbols["SCREEN"] = 16384;
    symbols["KBD"] = 24576;
}

void SymbolTable::addEntry(std::string symbol, int address)
{
    symbols[symbol] = address;
}

bool SymbolTable::contains(std::string symbol)
{
    return symbols.find(symbol) != symbols.end();
}

int SymbolTable::getAddress(std::string symbol)
{
    return symbols[symbol];
}