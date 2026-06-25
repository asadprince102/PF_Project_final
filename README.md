Campus Analytics Engine
Project Overview
The Campus Analytics Engine is a multi-file, menu-driven data analytics system for a fictional university campus. This system manages student records, course enrollments, attendance, grades, and fee transactions by loading data from TXT files.

This project was built strictly using foundational C++ programming constructs. It adheres strictly to the following technical constraints:

No Object-Oriented Programming (OOP): class keywords are avoided; structured using struct and standard arrays.
No STL Algorithms: No <algorithm>, <map>, or <unordered_map>. All sorting (Selection, Bubble) and searching (Linear) are implemented manually.
Manual String Parsing: No getline splits or standard library string tokenizers; all file parsing is done using manual character loops.
No <ctime>: All date arithmetic for late fee calculations is processed manually via string parsing.
Features List
The system is divided into 7 core modules:

File Handler (M1): Custom CSV/TXT parsing and writing logic.
Student Operations (M2): Add, search, update, and soft-delete students. Features manual substring matching for search-as-you-type.
Course Operations (M3): Enroll/drop courses, validate credit loads (max 21 hours), and check prerequisites.
Attendance Module (M4): Mark attendance, compute percentages, flag shortages (< 75%), and undo previous sessions.
Grades Module (M5): Enter marks, apply best "Three of Five" logic manually, compute weighted totals, GPA, and apply attendance penalties.
Fee Tracker (M6): Record payments, calculate late fines based on manual date arithmetic (2% per complete week), and generate formatted receipts.
Reports Dashboard (M7): Generate formatted console and text-file reports including Merit Lists, Defaulter Lists, and Department Summaries.
Compile the code: Open your terminal/command prompt, navigate to the project directory, and run the following command to link and compile all source files:

g++ main.cpp filehandler.cpp student_ops.cpp course_ops.cpp attendance.cpp grades.cpp fee_tracker.cpp reports.cpp -o CampusAnalytics
