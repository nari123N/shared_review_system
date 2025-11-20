#pragma once
#include <unordered_map>
#include <string>
#include "Database.h"

class DBMS {
    std::unordered_map<std::string, Database> databases;
    std::string currentDB;

public:
    void createDatabase(const std::string& name);
    void useDatabase(const std::string& name);
    void executeCommand(const std::string& command);
};