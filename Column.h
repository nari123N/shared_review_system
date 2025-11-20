#pragma once
#include <string>

class Column {
public:
    std::string name;
    Column(const std::string& colName) : name(colName) {}
};