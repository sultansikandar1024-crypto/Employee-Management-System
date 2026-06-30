# Employee-Management-System
It involves basics of C++ Object Oriented Programming's Concepts embodying the essential sequence of using these concepts in a practical way to create decency in making the work throughout the hustle, easier and more approachable. 
C++ Employee Management System#
A simple console-based C++ application that demonstrates object-oriented programming concepts through an employee management system.

Features#
Abstract base class Person
Single inheritance with Employee
Multilevel inheritance with Manager, Developer, and Intern
Polymorphism using virtual functions
Deep copy and move semantics for Employee
Template function getMax<T>()
Template class Repository<T> for reusable collections
Aggregation/composition with Department
Console input helpers for safe user entry
Project Structure#
employee_management.cpp - Main program, class definitions, and application logic
Build Instructions#
Using g++ (MinGW or other GCC on Windows)#
g++ -std=c++17 -o employee_management employee_management.cpp
Using Microsoft Visual C++ (MSVC)#
cl /EHsc /std:c++17 employee_management.cpp
Run#
./employee_management
On Windows with MSVC, run:

employee_management.exe
Usage#
The program presents a menu that allows you to:

Add a Manager
Add a Developer
Add an Intern
Display all employees
Show total payroll
Search employee by ID
Remove employee by ID
Demonstrate copy and move constructors
Demonstrate templates
Show total employee objects created
Exit
Notes#
The application uses polymorphic storage to manage different employee types through a common Person pointer.
Department owns employee objects and deletes them automatically when the program ends.
Employee tracks the total number of employee objects created using a static member.
License#
No license required.

