#include "Table.h"
#include <iostream>
#include <unordered_map>
#include <algorithm>

Table::Table(const std::string& tableName, const std::vector<Column>& cols)
    : name(tableName), columns(cols) {}

void Table::insertRow(const std::vector<std::string>& values) {
    if (values.size() != columns.size()) {
        std::cout << "Error: Column count doesn't match.\n";
        return;
    }
    rows.push_back(values);
    std::cout << "Row inserted into '" << name << "'.\n";
}
void Table::insertIntoPartial(const std::vector<std::string>& columns,
                              const std::vector<std::string>& values) {
    std::unordered_map<std::string, std::string> colValueMap;
    for (size_t i = 0; i < columns.size(); ++i) {
        colValueMap[columns[i]] = values[i];
    }

    std::vector<std::string> newRow;
   for (const auto& col : this->columns) {
    const std::string& colName = col.name;  // Access the column's name
    if (colValueMap.count(colName)) {
        newRow.push_back(colValueMap[colName]);
    } else {
        newRow.push_back("");  // or "NULL"
    }
}


    this->rows.push_back(newRow);
}




std::vector<std::string> Table::getColumnNames() const {
    std::vector<std::string> names;
    for (const auto& col : columns)
        names.push_back(col.name);
    return names;
}

const std::vector<std::vector<std::string>>& Table::getRows() const {
    return rows;
}



// void Table::selectColumns(const std::vector<std::string>& colNames) const {
//     std::vector<int> colIndices;

//     // Find indices of requested columns
//     for (const auto& name : colNames) {
//         bool found = false;
//         for (int i = 0; i < columns.size(); ++i) {
//             if (columns[i].name == name) {
//                 colIndices.push_back(i);
//                 found = true;
//                 break;
//             }
//         }
//         if (!found) {
//             std::cout << "Error: Column '" << name << "' does not exist.\n";
//             return;
//         }
//     }

//     // Print header
//     for (int idx : colIndices)
//         std::cout << columns[idx].name << " ";
//     std::cout << "\n";

//     // Print rows
//     for (const auto& row : rows) {
//         for (int idx : colIndices)
//             std::cout << row[idx] << " ";
//         std::cout << "\n";
//     }
// }

void Table::printAll() const {
    for (const auto& col : columns)
        std::cout << col.name << " ";
    std::cout << "\n";

    for (const auto& row : rows) {
        for (const auto& val : row)
            std::cout << val << " ";
        std::cout << "\n";
    }
}