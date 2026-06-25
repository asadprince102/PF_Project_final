#ifndef STUDENT_OPS_H
#define STUDENT_OPS_H

#include "../M1_filehandler/filehandler.h"

#include <string>

bool validateRollFormat(const std::string& roll);
bool validateStudentName(const std::string& name);
bool addStudent(const std::string& roll, const std::string& name,
                const std::string& department, int semester, double cgpa);
Row searchByRoll(const std::string& roll);
Table searchByName(const std::string& namePart);
bool updateStudent(const std::string& roll, const std::string& fieldName,
                   const std::string& newValue);
bool softDelete(const std::string& roll);
Table listActiveStudents();
bool isStudentActive(const std::string& roll);
std::string getStudentName(const std::string& roll);

#endif
