#include <iostream>
#include <fstream>
#include <string>
#include <bitset>
#include "Parser.h"
#include "SymbolTable.h"
#include "Code.h"

/**
 * Helper function to convert a decimal string into a 15-bit binary string.
 * This is used for A-instructions (@value).
 */
std::string decimalToBinary(std::string decimalStr)
{
    int value{ std::stoi(decimalStr) };
    // std::bitset<15> converts the integer into exactly 15 binary bits
    return std::bitset<15>(value).to_string();
}

int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <filename.asm>" << std::endl;
        return 1;
    }

    std::string inputFile{ argv[1] };
    std::cout << inputFile;

    std::string outputFile{ inputFile.substr(0, inputFile.find_last_of(".")) + ".hack" };

    // Initialize components
    SymbolTable st{};

    // --- FIRST PASS: Build Symbol Table ---
    Parser parser1(inputFile);
    int romAddr{};

    while (parser1.hasMoreLines())
    {
        parser1.advance();
        CommandType type{ parser1.commandType() };

        if (type == L_COMMAND)
        {
            // Label (XXX) points to the address of the NEXT instruction
            st.addEntry(parser1.symbol(), romAddr);
        }
        else if (type == A_COMMAND || type == C_COMMAND)
        {
            // Only actual instructions occupy space in memory
            romAddr++;
        }
    }

    // --- SECOND PASS: Translation ---
    Parser parser2(inputFile);
    std::ofstream outFile(outputFile);
    int ramAddr{ 16 }; // New variables start at RAM address 16

    while (parser2.hasMoreLines())
    {
        parser2.advance();
        CommandType type{ parser2.commandType() };

        if (type == A_COMMAND) {
            std::string symbol{ parser2.symbol() };
            std::string binary{};

            // Check if symbol is numeric (e.g., @100) or symbolic (e.g., @sum)
            if (std::isdigit(symbol[0]))
            {
                binary = decimalToBinary(symbol);
            }
            else
            {
                if (!st.contains(symbol))
                {
                    st.addEntry(symbol, ramAddr++);
                }
                binary = decimalToBinary(std::to_string(st.getAddress(symbol)));
            }
            // A-instruction format: 0 + 15 bits
            outFile << "0" << binary << "\n";
        }
        else if (type == C_COMMAND)
        {
            // C-instruction format: 111 + comp(7) + dest(3) + jump(3)
            std::string binaryLine{ "111"
                + Code::comp(parser2.comp())
                + Code::dest(parser2.dest())
                + Code::jump(parser2.jump()) };
            outFile << binaryLine << "\n";
        }
        // L_COMMAND and INVALID lines are ignored in Pass 2
    }

    std::cout << "Completed. Output written to: " << outputFile << '\n';
    outFile.close();

    return 0;
}