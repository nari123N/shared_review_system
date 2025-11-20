#include "Database.h"
#include <iostream>
#include<algorithm>


void Database::createTable(const std::string& name, const std::vector<Column>& columns) {
    if (tables.find(name) != tables.end()) {
        std::cout << "Error: Table already exists.\n";
        return;
    }
    Table newTable(name, columns);
    tables.insert({name, newTable});  // ✅ no default constructor required
    std::cout << "Table '" << name << "' created.\n";
}

void Database::insertInto(const std::string& tableName, const std::vector<std::string>& values) {
    auto it = tables.find(tableName);
    if (it == tables.end()) {
        std::cout << "Error: Table not found.\n";
        return;
    }
    it->second.insertRow(values);
}

void Database::selectAll(const std::string& tableName) const {
    auto it = tables.find(tableName);
    if (it == tables.end()) {
        std::cout << "Error: Table not found.\n";
        return;
    }
    it->second.printAll();
}
void Database::selectColumns(const std::string& tableName, const std::vector<std::string>& selectedCols) const {
    auto it = tables.find(tableName);
    if (it == tables.end()) {
        std::cout << "Error: Table not found.\n";
        return;
    }

    const Table& table = it->second;
    std::vector<std::string> columnNames = table.getColumnNames();
    std::vector<int> colIndices;

        for (const auto& col : selectedCols) {
        std::string cleanedCol = col;
         // Trim leading/trailing whitespace and semicolon
    cleanedCol.erase(0, cleanedCol.find_first_not_of(" \t\n\r\f\v"));
    cleanedCol.erase(cleanedCol.find_last_not_of(" \t\n\r\f\v") + 1);
        if (!cleanedCol.empty() && cleanedCol.back() == ';')
            cleanedCol.pop_back();

        auto iter = std::find(columnNames.begin(), columnNames.end(), cleanedCol);
        if (iter == columnNames.end()) {
            std::cout << "Error: Column '" << cleanedCol << "' does not exist.\n";
            return;
        }
        colIndices.push_back(std::distance(columnNames.begin(), iter));
    }


    // Print header
    for (const auto& col : selectedCols)
        std::cout << col << " ";
    std::cout << "\n";

    // Print rows
    for (const auto& row : table.getRows()) {
        for (int idx : colIndices)
            std::cout << row[idx] << " ";
        std::cout << "\n";
    }
}


std::vector<std::string> Database::listTables() const {
    std::vector<std::string> names;
    for (const auto& pair : tables) {
        names.push_back(pair.first);
    }
    return names;
}

