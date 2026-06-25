#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <string>
#include <vector>

typedef std::vector<std::string> Row;
typedef std::vector<Row> Table;

Table readTXT(const std::string& filename);
Row readHeader(const std::string& filename);
bool writeTXT(const std::string& filename, const Row& header, const Table& rows);
bool appendTXT(const std::string& filename, const Row& row);
Row findRow(const std::string& filename, int colIndex, const std::string& value);
bool rowExists(const std::string& filename, int colIndex, const std::string& value);
Row parseCSVLine(const std::string& line);
std::string makeCSVLine(const Row& row);

#endif
