#include "student_ops.h"

#include <cctype>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <sstream>

using namespace std;

static const string STUDENTS_FILE = "students.txt";

static string lowerText(string text) {
    for (int i = 0; i < static_cast<int>(text.length()); i++) {
        text[i] = static_cast<char>(tolower(static_cast<unsigned char>(text[i])));
    }
    return text;
}

static bool parseNumber(const string& text, double& value) {
    stringstream parser(text);
    char extra;
    if (!(parser >> value)) {
        return false;
    }
    return !(parser >> extra);
}

static bool parseWholeNumber(const string& text, int& value) {
    stringstream parser(text);
    char extra;
    if (!(parser >> value)) {
        return false;
    }
    return !(parser >> extra);
}

bool validateRollFormat(const string& roll) {
    if (roll.length() != 11) {
        return false;
    }

    if (roll.substr(0, 5) != "BSAI-"
        || roll[7] != '-') {
        return false;
    }

    if (!isdigit(static_cast<unsigned char>(roll[5]))
        || !isdigit(static_cast<unsigned char>(roll[6]))) {
        return false;
    }

    for (int i = 8; i <= 10; i++) {
        if (!isdigit(static_cast<unsigned char>(roll[i]))) {
            return false;
        }
    }

    return true;
}

bool validateStudentName(const string& name) {
    bool hasLetter = false;

    if (name.empty()) {
        return false;
    }

    for (int i = 0; i < static_cast<int>(name.length()); i++) {
        unsigned char current = static_cast<unsigned char>(name[i]);
        if (isdigit(current)) {
            return false;
        }
        if (isalpha(current)) {
            hasLetter = true;
        }
    }

    return hasLetter;
}

bool addStudent(const string& roll, const string& name,
                const string& department, int semester, double cgpa) {
    if (!validateRollFormat(roll)) {
        cout << "Invalid roll number. Required format: BSAI-YY-XXX\n";
        return false;
    }
    if (rowExists(STUDENTS_FILE, 0, roll)) {
        cout << "A student with this roll number already exists.\n";
        return false;
    }
    if (!validateStudentName(name)) {
        cout << "Name must contain letters and cannot contain digits.\n";
        return false;
    }
    if (department.empty() || semester < 1 || semester > 12
        || cgpa < 0.0 || cgpa > 4.0) {
        cout << "Department, semester, or CGPA is outside the valid range.\n";
        return false;
    }

    stringstream semesterText;
    stringstream cgpaText;
    semesterText << semester;
    cgpaText << fixed << setprecision(2) << cgpa;

    Row row;
    row.push_back(roll);
    row.push_back(name);
    row.push_back(department);
    row.push_back(semesterText.str());
    row.push_back(cgpaText.str());
    row.push_back("active");

    return appendTXT(STUDENTS_FILE, row);
}

Row searchByRoll(const string& roll) {
    return findRow(STUDENTS_FILE, 0, roll);
}

Table searchByName(const string& namePart) {
    Table rows = readTXT(STUDENTS_FILE);
    Table matches;
    string wanted = lowerText(namePart);

    for (int i = 0; i < static_cast<int>(rows.size()); i++) {
        if (rows[i].size() > 1
            && lowerText(rows[i][1]).find(wanted) != string::npos) {
            matches.push_back(rows[i]);
        }
    }

    return matches;
}

bool updateStudent(const string& roll, const string& fieldName,
                   const string& newValue) {
    Table rows = readTXT(STUDENTS_FILE);
    Row header = readHeader(STUDENTS_FILE);
    int targetColumn = -1;

    if (fieldName == "name") {
        targetColumn = 1;
        if (!validateStudentName(newValue)) {
            return false;
        }
    } else if (fieldName == "department" || fieldName == "dept") {
        targetColumn = 2;
        if (newValue.empty()) {
            return false;
        }
    } else if (fieldName == "semester") {
        int semester = 0;
        targetColumn = 3;
        if (!parseWholeNumber(newValue, semester)
            || semester < 1 || semester > 12) {
            return false;
        }
    } else if (fieldName == "cgpa") {
        double cgpa = 0.0;
        targetColumn = 4;
        if (!parseNumber(newValue, cgpa) || cgpa < 0.0 || cgpa > 4.0) {
            return false;
        }
    } else if (fieldName == "status") {
        targetColumn = 5;
        if (newValue != "active" && newValue != "inactive") {
            return false;
        }
    } else {
        return false;
    }

    for (int i = 0; i < static_cast<int>(rows.size()); i++) {
        if (!rows[i].empty() && rows[i][0] == roll) {
            while (static_cast<int>(rows[i].size()) <= targetColumn) {
                rows[i].push_back("");
            }
            rows[i][targetColumn] = newValue;
            return writeTXT(STUDENTS_FILE, header, rows);
        }
    }

    return false;
}

bool softDelete(const string& roll) {
    return updateStudent(roll, "status", "inactive");
}

Table listActiveStudents() {
    Table rows = readTXT(STUDENTS_FILE);
    Table active;

    for (int i = 0; i < static_cast<int>(rows.size()); i++) {
        if (rows[i].size() > 5 && rows[i][5] == "active") {
            active.push_back(rows[i]);
        }
    }

    for (int i = 0; i < static_cast<int>(active.size()); i++) {
        int smallest = i;
        for (int j = i + 1; j < static_cast<int>(active.size()); j++) {
            if (active[j][0] < active[smallest][0]) {
                smallest = j;
            }
        }
        if (smallest != i) {
            Row temporary = active[i];
            active[i] = active[smallest];
            active[smallest] = temporary;
        }
    }

    return active;
}

bool isStudentActive(const string& roll) {
    Row student = searchByRoll(roll);
    return student.size() > 5 && student[5] == "active";
}

string getStudentName(const string& roll) {
    Row student = searchByRoll(roll);
    if (student.size() > 1) {
        return student[1];
    }
    return "Unknown";
}
