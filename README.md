# C++ Employee Management System

A simple console-based C++ application that demonstrates object-oriented programming concepts through an employee management system.

## Features

- Abstract base class `Person`
- Single inheritance with `Employee`
- Multilevel inheritance with `Manager`, `Developer`, and `Intern`
- Polymorphism using virtual functions
- Deep copy and move semantics for `Employee`
- Template function `getMax<T>()`
- Template class `Repository<T>` for reusable collections
- Aggregation/composition with `Department`
- Console input helpers for safe user entry

## Project Structure

- `employee_management.cpp` - Main program, class definitions, and application logic

## Build Instructions

### Using g++ (MinGW or other GCC on Windows)

```bash
g++ -std=c++17 -o employee_management employee_management.cpp
```

### Using Microsoft Visual C++ (MSVC)

```bash
cl /EHsc /std:c++17 employee_management.cpp
```

## Run

```bash
./employee_management
```

On Windows with MSVC, run:

```bash
employee_management.exe
```

## Usage

The program presents a menu that allows you to:

1. Add a Manager
2. Add a Developer
3. Add an Intern
4. Display all employees
5. Show total payroll
6. Search employee by ID
7. Remove employee by ID
8. Demonstrate copy and move constructors
9. Demonstrate templates
10. Show total employee objects created
11. Exit

## Notes

- The application uses polymorphic storage to manage different employee types through a common `Person` pointer.
- `Department` owns employee objects and deletes them automatically when the program ends.
- `Employee` tracks the total number of employee objects created using a static member.

## License

Add your preferred license here if needed.
