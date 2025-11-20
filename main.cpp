#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "engine/DBMS.h"
// Function to execute commands from a file
// This function reads commands from a file and executes them using the DBMS instance
void executeFromFile(const std::string& filename, DBMS& dbms) {
    std::ifstream infile(filename);
    if (!infile.is_open()) {
        std::cerr << "Error: Could not open file '" << filename << "'\n";
        return;
    }

    std::string line, commandBuffer;

    while (std::getline(infile, line)) {
        line.erase(0, line.find_first_not_of(" \t\r\n"));
        line.erase(line.find_last_not_of(" \t\r\n") + 1);

        if (line.empty()) continue;

        commandBuffer += line + " ";

        size_t semicolonPos;
        while ((semicolonPos = commandBuffer.find(';')) != std::string::npos) {
            std::string command = commandBuffer.substr(0, semicolonPos);
            commandBuffer = commandBuffer.substr(semicolonPos + 1);

            // Trim
            command.erase(0, command.find_first_not_of(" \t\r\n"));
            command.erase(command.find_last_not_of(" \t\r\n") + 1);

            if (!command.empty()) {
                std::cout << ">>> " << command << std::endl;
                dbms.executeCommand(command);
            }
        }
    }

    // Handle final leftover command without semicolon
    commandBuffer.erase(0, commandBuffer.find_first_not_of(" \t\r\n"));
    commandBuffer.erase(commandBuffer.find_last_not_of(" \t\r\n") + 1);
    if (!commandBuffer.empty()) {
        std::cout << ">>> " << commandBuffer << std::endl;
        dbms.executeCommand(commandBuffer);
    }
}

int main() {
    DBMS dbms;
    std::string line;
    std::string commandBuffer;

    std::cout << "Welcome to In-Memory SQL DB. Type commands below." << std::endl;

    while (true) {
    std::cout << ">>> ";
    std::getline(std::cin, line);

    if (line == "exit") break;

    if (line.rfind("\\i ", 0) == 0) {
        std::string filename = line.substr(3);
        executeFromFile(filename, dbms);
        continue;
    }

    commandBuffer += line + " ";

    size_t semicolonPos;
    while ((semicolonPos = commandBuffer.find(';')) != std::string::npos) {
        std::string command = commandBuffer.substr(0, semicolonPos);
        commandBuffer = commandBuffer.substr(semicolonPos + 1);

        // Trim
        command.erase(0, command.find_first_not_of(" \t\r\n"));
        command.erase(command.find_last_not_of(" \t\r\n") + 1);

        if (!command.empty()) {
            dbms.executeCommand(command);
        }
    }
}
    // Handle final leftover command without semicolon
    commandBuffer.erase(0, commandBuffer.find_first_not_of(" \t\r\n"));
    commandBuffer.erase(commandBuffer.find_last_not_of(" \t\r\n") + 1);
    if (!commandBuffer.empty()) {
        dbms.executeCommand(commandBuffer);
    }


    return 0;
}
