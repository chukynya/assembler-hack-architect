#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <fstream>

enum CommandType
{
    A_COMMAND, // @value
    C_COMMAND, // dest=comp;jump
    L_COMMAND, // (LABEL)
    INVALID    // Empty or comment lines
};

class Parser
{
public:
    Parser(std::string filename);
    bool hasMoreLines();
    void advance();
    
    CommandType commandType();
    std::string symbol(); // For A and L commands
    std::string dest();   // For C commands
    std::string comp();   // For C commands
    std::string jump();   // For C commands

private:
    std::ifstream file;
    std::string currentLine;
    void cleanLine(); // Helper to remove comments/whitespace
};

#endif