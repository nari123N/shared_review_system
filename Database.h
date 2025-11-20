#pragma once
#include <unordered_map>
#include <string>
#include <iostream>
#include <vector>
#include "Table.h"

class Database {
    std::unordered_map<std::string, Table> tables;

public:
    void createTable(const std::string& name, const std::vector<Column>& columns);
    void insertInto(const std::string& tableName, const std::vector<std::string>& values);
    void insertPartial(const std::string& tableName, 
                       const std::vector<std::string>& specifiedCols, 
                       const std::vector<std::string>& values) {
        auto it = tables.find(tableName);
        if (it == tables.end()) {
            std::cout << "Error: Table not found. \n";
            return;
        }
       it->second.insertIntoPartial(specifiedCols, values);

    }
    void selectAll(const std::string& tableName) const;
    //void selectColumns(const std::string& tableName, const std::vector<std::string>& colNames) const;
    void selectColumns(const std::string& tableName, const std::vector<std::string>& selectedCols) const;
void selectAllWhere(const std::string& tableName,
                    const std::string& whereCol,
                    const std::string& whereVal) {
    if (tables.find(tableName) == tables.end()) {
        std::cout << "Error: Table '" << tableName << "' not found.\n";
        return;
    }

    const Table& table = tables.at(tableName);
    const auto& colNames = table.getColumnNames();
    const auto& rows = table.getRows();

    int whereIdx = -1;
    for (size_t i = 0; i < colNames.size(); ++i) {
        if (colNames[i] == whereCol) {
            whereIdx = i;
            break;
        }
    }

    if (whereIdx == -1) {
        std::cout << "Error: Column '" << whereCol << "' not found.\n";
        return;
    }

    // Clean the whereVal
    std::string cleanWhereVal = whereVal;
    cleanWhereVal.erase(0, cleanWhereVal.find_first_not_of(" \t\n\r\f\v\"'"));
    cleanWhereVal.erase(cleanWhereVal.find_last_not_of(" \t\n\r\f\v\"'") + 1);

    // Print header
    for (const auto& col : colNames)
        std::cout << col << "\t";
    std::cout << "\n";

    // Print filtered rows
    for (const auto& row : rows) {
        std::string cell = row[whereIdx];
        cell.erase(0, cell.find_first_not_of(" \t\n\r\f\v\"'"));
        cell.erase(cell.find_last_not_of(" \t\n\r\f\v\"'") + 1);

        if (cell == cleanWhereVal) {
            for (const auto& val : row)
                std::cout << val << "\t";
            std::cout << "\n";
        }
    }
}
void selectColumnsWhere(const std::string& tableName,
                        const std::vector<std::string>& selectedCols,
                        const std::string& whereCol,
                        const std::string& whereVal) {
    if (tables.find(tableName) == tables.end()) {
        std::cout << "Error: Table '" << tableName << "' does not exist.\n";
        return;
    }

    const Table& table = tables.at(tableName);
    const auto& colNames = table.getColumnNames();
    const auto& rows = table.getRows();

    std::unordered_map<std::string, int> colIndexMap;
    for (size_t i = 0; i < colNames.size(); ++i) {
        colIndexMap[colNames[i]] = i;
    }

    // Validate and collect indexes for selected columns
    std::vector<int> selectedIndexes;
    for (const auto& colName : selectedCols) {
        if (colIndexMap.find(colName) == colIndexMap.end()) {
            std::cout << "Error: Column '" << colName << "' does not exist.\n";
            return;
        }
        selectedIndexes.push_back(colIndexMap[colName]);
    }

    // Validate WHERE column
    if (colIndexMap.find(whereCol) == colIndexMap.end()) {
        std::cout << "Error: Column '" << whereCol << "' not found.\n";
        return;
    }
    int whereIdx = colIndexMap[whereCol];

    std::string cleanWhereVal = whereVal;
    cleanWhereVal.erase(0, cleanWhereVal.find_first_not_of(" \t\n\r\f\v\"'"));
    cleanWhereVal.erase(cleanWhereVal.find_last_not_of(" \t\n\r\f\v\"'") + 1);

    // Print header
    for (const auto& colName : selectedCols)
        std::cout << colName << "\t";
    std::cout << "\n";

    // Print filtered rows
    for (const auto& row : rows) {
        std::string cell = row[whereIdx];
        cell.erase(0, cell.find_first_not_of(" \t\n\r\f\v\"'"));
        cell.erase(cell.find_last_not_of(" \t\n\r\f\v\"'") + 1);

        if (cell == cleanWhereVal) {
            for (int idx : selectedIndexes)
                std::cout << row[idx] << "\t";
            std::cout << "\n";
        }
    }
}




    std::vector<std::string> listTables() const;

};