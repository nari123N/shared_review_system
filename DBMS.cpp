#include "DBMS.h"
#include <iostream>
#include <sstream>
#include <algorithm>

void DBMS::createDatabase(const std::string& name) {
    if (databases.find(name) != databases.end()) {
        std::cout << "Error: Database already exists.\n";
        return;
    }
    databases[name] = Database();
    std::cout << "Database '" << name << "' created.\n";
}

void DBMS::useDatabase(const std::string& name) {
    if (databases.find(name) == databases.end()) {
        std::cout << "Error: Database does not exist.\n";
        return;
    }
    currentDB = name;
    std::cout << "Using database '" << name << "'.\n";
}

void DBMS::executeCommand(const std::string& command) {
    std::istringstream iss(command);
    std::string token;
    iss >> token;

    if (token == "CREATE" || token == "create") {
        std::string objType;
        iss >> objType;
        if (objType == "DATABASE" || objType == "database") {
            std::string dbName;
            iss >> dbName;
            createDatabase(dbName);
        } else if (objType == "TABLE" || objType == "table") {
    if (currentDB.empty()) {
        std::cout << "Error: No database selected.\n";
        return;
    }

    std::string tableName;
    iss >> tableName;

    // Get the rest of the line and any remaining stream content
    std::string restOfCommand;
    std::getline(iss, restOfCommand);

    // Append everything after tableName to reconstruct full definition
    std::string fullDef = restOfCommand;

    // Remove all whitespace before '(' (for cases like: `tableName(`)
    size_t openParen = fullDef.find('(');
    size_t closeParen = fullDef.rfind(')');

    if (openParen == std::string::npos || closeParen == std::string::npos || closeParen <= openParen) {
        std::cout << "Error: Invalid syntax. Missing or mismatched parentheses.\n";
        return;
    }

    // Extract column definition string between ( and )
    std::string colsStr = fullDef.substr(openParen + 1, closeParen - openParen - 1);

    // Remove newlines (multi-line handling)
    std::replace(colsStr.begin(), colsStr.end(), '\n', ' ');

    // Parse column names
    std::istringstream colStream(colsStr);
    std::string col;
    std::vector<Column> columns;
    while (std::getline(colStream, col, ',')) {
        // Trim spaces
        col.erase(0, col.find_first_not_of(" \t\n\r\f\v"));
        col.erase(col.find_last_not_of(" \t\n\r\f\v") + 1);
        if (!col.empty()) {
            columns.push_back(Column(col));
        }
    }

    // Finally create the table
    databases[currentDB].createTable(tableName, columns);
}

    } else if (token == "USE" || token == "use") {
        std::string db,dbName;
        iss >> db >> dbName;
        if (dbName.empty()) {
            std::cout << "Error: No database specified.\n";
            return;}
        if(db != "database" && db != "DATABASE") {
            std::cout << "Error: Invalid syntax. Expected 'USE database_name'.\n";
            return;
        }else{
            useDatabase(dbName);
        }

       // useDatabase(dbName);
    } else if (token == "INSERT" || token == "insert") {
    std::string into, tableName;
    iss >> into >> tableName;

    std::string lineRemainder;
    std::getline(iss, lineRemainder);

    // Case-insensitive VALUES search
    std::string upperLine = lineRemainder;
    std::transform(upperLine.begin(), upperLine.end(), upperLine.begin(), ::toupper);
    size_t valStart = upperLine.find("VALUES");
    if (valStart == std::string::npos) {
        std::cout << "Error: Missing VALUES keyword.\n";
        return;
    }

    std::vector<std::string> columns, values;
 

    // Determine if there's a column list before VALUES
    size_t colStart = lineRemainder.find('(');
    size_t colEnd = lineRemainder.find(')', colStart);

    // Extract column list ONLY if it appears before the VALUES keyword
    if (colStart != std::string::npos && colEnd != std::string::npos &&
        static_cast<size_t>(colEnd) < valStart) {

        std::string colList = lineRemainder.substr(colStart + 1, colEnd - colStart - 1);
        std::istringstream colStream(colList);
        std::string col;
        while (std::getline(colStream, col, ',')) {
            col.erase(0, col.find_first_not_of(" \t\n\r\f\v"));
            col.erase(col.find_last_not_of(" \t\n\r\f\v") + 1);
            columns.push_back(col);
        }
    }

    // Extract values list
  
// Remove "VALUES" keyword
std::string valList = lineRemainder.substr(valStart);
std::transform(valList.begin(), valList.end(), valList.begin(), ::tolower);
if (valList.substr(0, 6) == "values") {
    valList = valList.substr(6);
}
valList.erase(0, valList.find_first_not_of(" \t\n\r\f\v"));
valList.erase(valList.find_last_not_of(" \t\n\r\f\v") + 1);

// Remove outer parentheses if double-wrapped like ((...), (...))
if (valList.front() == '(' && valList[1] == '(' && valList.back() == ')') {
    valList = valList.substr(1, valList.size() - 2); // remove outer (
}

std::vector<std::vector<std::string>> rows;
size_t i = 0;

while (i < valList.size()) {
    while (i < valList.size() && valList[i] != '(') i++;
    if (i == valList.size()) break;

    int openParens = 1;
    size_t start = ++i;
    std::string tuple;

    while (i < valList.size() && openParens > 0) {
        if (valList[i] == '(') openParens++;
        else if (valList[i] == ')') openParens--;
        if (openParens > 0)
            tuple += valList[i];
        i++;
    }

    if (openParens != 0) {
        std::cout << "Error: Unbalanced parentheses in VALUES.\n";
        return;
    }

    // Parse individual values
    std::istringstream rowStream(tuple);
    std::string value;
    std::vector<std::string> row;

    while (std::getline(rowStream, value, ',')) {
        value.erase(0, value.find_first_not_of(" \t\n\r\f\v"));
        value.erase(value.find_last_not_of(" \t\n\r\f\v") + 1);
        // Remove quotes if present
        if ((value.front() == '\'' && value.back() == '\'') ||
            (value.front() == '"' && value.back() == '"')) {
            value = value.substr(1, value.size() - 2);
        }
        row.push_back(value);
    }

    if (!row.empty())
        rows.push_back(row);
}


    if (currentDB.empty()) {
        std::cout << "Error: No database selected.\n";
        return;
    }

   if (columns.empty()) {
    for (const auto& row : rows) {
        databases[currentDB].insertInto(tableName, row);
    }
} else {
    for (const auto& row : rows) {
        databases[currentDB].insertPartial(tableName, columns, row);
    }
}

}

 else if (token == "SELECT" || token == "select") {
    std::string nextToken;
    iss >> nextToken;

    if (currentDB.empty()) {
        std::cout << "Error: No database selected.\n";
        return;
    }

    std::vector<std::string> selectedColumns;
    std::string tableName, whereColumn, whereValue;
    bool hasWhere = false;

    if (nextToken == "*") {
        selectedColumns.push_back("*");

        std::string fromKeyword;
        iss >> fromKeyword >> tableName;
        if (fromKeyword != "from") {
            std::cout << "Syntax error: expected 'from'.\n";
            return;
        }
    } else {
        // Collect all selected columns until "from"
        while (nextToken != "from" && !iss.eof()) {
            if (!nextToken.empty() && nextToken.back() == ',')
                nextToken.pop_back();
            selectedColumns.push_back(nextToken);
            iss >> nextToken;
        }

        iss >> tableName;
        if (nextToken != "from" || tableName.empty()) {
            std::cout << "Syntax error: expected 'from <table>'\n";
            return;
        }
    }

    // Check for WHERE clause
    std::string maybeWhere;
    iss >> maybeWhere;
    if (maybeWhere == "where") {
        hasWhere = true;
        std::string condition;
        std::getline(iss, condition);
        condition.erase(0, condition.find_first_not_of(" \t\n\r\f\v"));
        condition.erase(condition.find_last_not_of(" \t\n\r\f\v") + 1);

        // Simple condition parsing: col = val
        size_t eq = condition.find('=');
        if (eq == std::string::npos) {
            std::cout << "Syntax error in WHERE clause.\n";
            return;
        }

        whereColumn = condition.substr(0, eq);
        whereValue = condition.substr(eq + 1);

        // Trim spaces
        whereColumn.erase(0, whereColumn.find_first_not_of(" \t"));
        whereColumn.erase(whereColumn.find_last_not_of(" \t") + 1);
        // Trim leading/trailing whitespace
        whereValue.erase(0, whereValue.find_first_not_of(" \t"));
        whereValue.erase(whereValue.find_last_not_of(" \t") + 1);

        // Remove enclosing single/double quotes if present
        if ((whereValue.front() == '\'' && whereValue.back() == '\'') ||
            (whereValue.front() == '"'  && whereValue.back() == '"')) {
            whereValue = whereValue.substr(1, whereValue.size() - 2);
        }


        // Remove semicolon if present
        if (!whereValue.empty() && whereValue.back() == ';')
            whereValue.pop_back();
    }

    // Final call to DB
    if (selectedColumns.size() == 1 && selectedColumns[0] == "*") {
        if (hasWhere)
            databases[currentDB].selectAllWhere(tableName, whereColumn, whereValue);
        else
            databases[currentDB].selectAll(tableName);
    } else {
        if (hasWhere)
            databases[currentDB].selectColumnsWhere(tableName, selectedColumns, whereColumn, whereValue);
        else
            databases[currentDB].selectColumns(tableName, selectedColumns);
    }
}

    else if(token == "SHOW" || token == "show") {
        std::string objType;
        iss >> objType;
        if (objType == "DATABASES" || objType == "databases") {
        if (databases.empty()) {
            std::cout << "No databases available.\n";
            return;
        }
        std::cout << "Available databases:\n";
        for (const auto& db : databases) {
            std::cout << "- " << db.first << "\n";
        }
        } 
        else if (objType == "TABLES" || objType == "tables") {
            if (currentDB.empty()) {
                std::cout << "Error: No database selected.\n";
                return;
            }
            const auto& tables = databases[currentDB].listTables();
            if (tables.empty()) {
                std::cout << "No tables in the current database.\n";
                return;
            }
            std::cout << "Tables in the database '" << currentDB << "':\n";
            for (const auto& table : tables) {
                std::cout << "- " << table << "\n";
            }
        }
        else{
            std::cout << "Error: Unknown SHOW command.\n";
        }
    }
    else if(token == "DROP" || token == "drop") {
        std::string objType;
        iss >> objType;
        if (objType == "DATABASE" || objType == "database") {
            std::string dbName;
            iss >> dbName;
            if (databases.erase(dbName)) {
                std::cout << "Database '" << dbName << "' dropped.\n";
                if (currentDB == dbName) {
                    currentDB.clear();
                }
            } else {
                std::cout << "Error: Database does not exist.\n";
            }
        }
         
    }
   
    else {
        std::cout << "Error: Unknown command.\n";
    }
}