#pragma once
#include <string>
#include <vector>
#include<iostream>
#include "Column.h"

class Table {
    std::string name;
    std::vector<Column> columns;
    std::vector<std::vector<std::string>> rows;

public:
    Table(const std::string& tableName, const std::vector<Column>& cols);
    void insertRow(const std::vector<std::string>& values);
    void insertIntoPartial(const std::vector<std::string>& columns,
                           const std::vector<std::string>& values);

  //  void selectColumns(const std::vector<std::string>& colNames) const;
    std::vector<std::string> getColumnNames() const;
   const std::vector<std::vector<std::string>>& getRows() const;


    void printAll() const;
};