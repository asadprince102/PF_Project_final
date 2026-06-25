#include "filehandler.h"

#include <fstream>

using namespace std;

Row parseCSVLine(const string& line) {
    Row fields;
    string field;
    bool insideQuotes = false;

    for (int i = 0; i < static_cast<int>(line.length()); i++) {
        char current = line[i];

        if (current == '"') {
            if (insideQuotes && i + 1 < static_cast<int>(line.length())
                && line[i + 1] == '"') {
                field += '"';
                i++;
            } else {
                insideQuotes = !insideQuotes;
            }
        } else if (current == ',' && !insideQuotes) {
            fields.push_back(field);
            field = "";
        } else {
            field += current;
        }
    }

    fields.push_back(field);
    return fields;
}

static string encodeField(const string& field) {
    bool needsQuotes = false;
    string encoded;

    for (int i = 0; i < static_cast<int>(field.length()); i++) {
        if (field[i] == ',' || field[i] == '"'
            || field[i] == '\n' || field[i] == '\r') {
            needsQuotes = true;
        }

        if (field[i] == '"') {
            encoded += "\"\"";
        } else {
            encoded += field[i];
        }
    }

    if (needsQuotes) {
        return "\"" + encoded + "\"";
    }
    return encoded;
}

string makeCSVLine(const Row& row) {
    string line;

    for (int i = 0; i < static_cast<int>(row.size()); i++) {
        if (i > 0) {
            line += ",";
        }
        line += encodeField(row[i]);
    }

    return line;
}

Table readTXT(const string& filename) {
    Table rows;
    ifstream input(filename.c_str());
    string line;

    if (!input.is_open()) {
        return rows;
    }

    getline(input, line);
    while (getline(input, line)) {
        if (!line.empty() && line[line.length() - 1] == '\r') {
            line.erase(line.length() - 1);
        }
        if (!line.empty()) {
            rows.push_back(parseCSVLine(line));
        }
    }

    input.close();
    return rows;
}

Row readHeader(const string& filename) {
    ifstream input(filename.c_str());
    string line;

    if (!input.is_open() || !getline(input, line)) {
        return Row();
    }

    if (!line.empty() && line[line.length() - 1] == '\r') {
        line.erase(line.length() - 1);
    }
    input.close();
    return parseCSVLine(line);
}

bool writeTXT(const string& filename, const Row& header, const Table& rows) {
    ofstream output(filename.c_str());

    if (!output.is_open()) {
        return false;
    }

    output << makeCSVLine(header) << "\n";
    for (int i = 0; i < static_cast<int>(rows.size()); i++) {
        output << makeCSVLine(rows[i]) << "\n";
    }

    output.close();
    return true;
}

bool appendTXT(const string& filename, const Row& row) {
    ofstream output(filename.c_str(), ios::app);

    if (!output.is_open()) {
        return false;
    }

    output << makeCSVLine(row) << "\n";
    output.close();
    return true;
}

Row findRow(const string& filename, int colIndex, const string& value) {
    Table rows = readTXT(filename);

    for (int i = 0; i < static_cast<int>(rows.size()); i++) {
        if (colIndex >= 0 && colIndex < static_cast<int>(rows[i].size())
            && rows[i][colIndex] == value) {
            return rows[i];
        }
    }

    return Row();
}

bool rowExists(const string& filename, int colIndex, const string& value) {
    return !findRow(filename, colIndex, value).empty();
}
