/*
====================================================================================================
                         EMPLOYEE MANAGEMENT SYSTEM  (Single File C++ OOP Project)
====================================================================================================

1. CLASS & OBJECT            -> Every blueprint below (Person, Employee, Manager...) is a class.
                                 Objects are created of these classes inside main().

2. ENCAPSULATION              -> Data members are kept 'private' or 'protected' and can only be
                                 accessed/changed through public Setter and Getter functions.
                                 (See: Person, Employee classes)

3. ABSTRACTION                -> Hiding internal implementation and only exposing what is needed.
                                 Achieved using an ABSTRACT CLASS (a class with at least one
                                 pure virtual function). See: class Person.

4. INHERITANCE                -> A class (child) can reuse/extend another class (parent).
                                 Single Inheritance   : Employee inherits from Person
                                 Multilevel Inheritance: Manager/Developer/Intern inherit from
                                                         Employee, which inherits from Person
                                 (Person -> Employee -> Manager/Developer/Intern)

5. POLYMORPHISM                -> "One name, many forms". Two types are demonstrated:
     a) Compile-Time (Static) Polymorphism = FUNCTION OVERLOADING + TEMPLATES
        (decided by the compiler BEFORE the program runs)
     b) Run-Time (Dynamic) Polymorphism = FUNCTION OVERRIDING using VIRTUAL FUNCTIONS
        (decided WHILE the program is running, using a base class pointer/reference)

6. VIRTUAL FUNCTION  vs  ABSTRACT (PURE VIRTUAL) FUNCTION  -> *** Important difference ***
     - A VIRTUAL function HAS a body/definition in the base class.
       The derived class MAY override it, but does NOT have to (overriding is OPTIONAL).
       Example: Employee::calculateSalary()
     - An ABSTRACT / PURE VIRTUAL function has NO body (it ends with "= 0;").
       Every derived concrete class MUST override it (overriding is COMPULSORY).
       A class containing even ONE pure virtual function becomes an ABSTRACT CLASS and
       CANNOT be instantiated directly (you cannot write "Person p;").
       Example: Person::display() = 0;

7. FUNCTION OVERLOADING        -> Same function name, DIFFERENT parameter list, in the SAME class.
                                 Resolved at COMPILE TIME. Example: Employee::addBonus(...)
                                 and the multiple (overloaded) constructors of Employee.

8. FUNCTION OVERRIDING          -> Same function name AND same parameters, but redefined inside a
                                 DERIVED class (the base version must be declared virtual).
                                 Resolved at RUN TIME. Example: calculateSalary() and display()
                                 are overridden in Manager, Developer, and Intern.

9. CONSTRUCTORS                 -> Special function that runs automatically when an object is created.
     - Default Constructor      : takes no arguments.
     - Parameterized Constructor: takes arguments to initialize an object directly.
     - Constructor Overloading  : having both of the above in the same class.
     - Copy Constructor         : creates a new object as a DEEP COPY of an existing object.
     - Move Constructor         : "steals"/transfers resources from a temporary object instead
                                   of copying them (faster, used with std::move).

10. DESTRUCTOR                  -> Special function that runs automatically when an object is
                                 destroyed, used to free memory / resources. MUST be declared
                                 'virtual' in a base class if you plan to delete derived objects
                                 through a base class pointer - otherwise only the base part gets
                                 destroyed (memory leak / undefined behaviour).

11. TEMPLATES                   -> A "blueprint" that lets you write ONE piece of code that works
                                 with MANY data types. The compiler generates a specific version
                                 for whichever type you actually use, at compile time.
                                 Example: template class Repository<T> and template function getMax<T>.

12. OPERATOR OVERLOADING (Bonus) -> Giving an existing operator (like << ) a new, custom meaning
                                 when used with our own class type. Example: operator<< for Employee.

13. STATIC MEMBERS (Bonus)       -> A variable/function that belongs to the CLASS itself, not to
                                 any one object. Shared by ALL objects. Example: Employee::employeeCount.

14. MEMBER FUNCTIONS / display() -> Normal functions written inside a class that operate on that
                                 object's own data. Every class below has a display() member
                                 function (overridden polymorphically) to print its details.

15. AGGREGATION / COMPOSITION    -> The Department class "has-a" collection of Person objects
                                 (built using pointers), and ManagementSystem "has-a" Department.
====================================================================================================
*/

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <limits>
using namespace std;   // used so we can write "string" / "cout" instead of "std::string" / "std::cout"


// ====================================================================================================
// SECTION 1: ABSTRACT BASE CLASS -> "Person"
// ====================================================================================================
// [CONCEPT: ABSTRACTION + ABSTRACT CLASS]
// Person is an ABSTRACT class because it contains a PURE VIRTUAL function: display() = 0;
// An abstract class only gives a common "skeleton"/contract that every child class MUST follow.
// You CANNOT create an object of an abstract class directly, e.g. "Person p;" -> COMPILE ERROR.
// It exists ONLY so other classes can inherit from it.
class Person {
protected:
    // [CONCEPT: ENCAPSULATION] - data is 'protected', not 'public'.
    // Protected means: visible to this class AND to its child classes, but hidden from outside code.
    string id;
    string name;

public:
    // [CONCEPT: CONSTRUCTOR - Default Constructor]
    // Runs when no arguments are given, e.g. "Person p;" (not usable here since class is abstract,
    // but derived class constructors still call this automatically if no other ctor is chosen).
    Person() {
        id = "N/A";
        name = "N/A";
    }

    // [CONCEPT: CONSTRUCTOR - Parameterized Constructor]
    // [CONCEPT: CONSTRUCTOR OVERLOADING] - having Person() AND Person(id, name) is overloading,
    // because both share the same name "Person" but have different parameter lists.
    Person(string personId, string personName) {
        id = personId;
        name = personName;
    }

    // [CONCEPT: DESTRUCTOR + VIRTUAL DESTRUCTOR]
    // This destructor is marked 'virtual'. This is EXTREMELY important:
    // Later we will store Manager/Developer/Intern objects using a "Person*" pointer.
    // If this destructor were NOT virtual, then "delete personPointer;" would only call
    // ~Person() and skip ~Manager()/~Developer()/~Intern(), causing memory leaks and bugs.
    // Marking it virtual guarantees the CORRECT (derived) destructor always runs.
    virtual ~Person() {
        // Nothing to clean up here, but printing a trace helps you SEE the destruction order.
        cout << "[Destructor] Person base part of \"" << name << "\" destroyed.\n";
    }

    // [CONCEPT: SETTERS AND GETTERS] (Encapsulation - controlled access to private/protected data)
    string getId() const { return id; }
    void setId(const string& newId) { id = newId; }

    string getName() const { return name; }
    void setName(const string& newName) { name = newName; }

    // [CONCEPT: ABSTRACT / PURE VIRTUAL FUNCTION]
    // "= 0" means THIS function has NO BODY in this class.
    // Every class that inherits from Person and wants to be a usable (concrete) class
    // MUST provide its own definition of display(). This is COMPULSORY overriding.
    virtual void display() const = 0;

    // Another pure virtual function -> also forces every child to define its own "role".
    virtual string getRole() const = 0;
};


// ====================================================================================================
// SECTION 2: "Employee" CLASS  -> inherits from Person  (SINGLE INHERITANCE)
// ====================================================================================================
// [CONCEPT: INHERITANCE - Single Inheritance]
// "class Employee : public Person" means Employee automatically GETS the id, name, getters,
// setters, and the "contract" (pure virtual functions) of Person, and adds its own extra data.
class Employee : public Person {
protected:
    double baseSalary;
    string department;

private:
    // We deliberately use a DYNAMICALLY ALLOCATED (heap) array here on purpose, so that we can
    // properly demonstrate Copy Constructor vs Move Constructor below (this is the classic
    // textbook scenario where copy/move constructors actually matter).
    string* skills;     // pointer to a heap array of skill names
    int skillCount;     // how many skills are stored

    // [CONCEPT: STATIC MEMBER] (Bonus)
    // 'static' means this variable is shared by ALL Employee objects - there is only ONE copy
    // of employeeCount in memory no matter how many Employee objects exist.
    static int employeeCount;

public:
    // ---------------------------------------------------------------------------
    // [CONCEPT: CONSTRUCTOR OVERLOADING] - Default + Parameterized constructors
    // ---------------------------------------------------------------------------

    // Default Constructor
    Employee() : Person(), baseSalary(0.0), department("Unassigned"), skills(nullptr), skillCount(0) {
        employeeCount++;   // every time an Employee is born, increase the shared static counter
        cout << "[Constructor] Default Employee constructor called.\n";
    }

    // Parameterized Constructor
    // ": Person(empId, empName)" calls the PARENT constructor first (base class is always
    // constructed before the derived class body runs).
    Employee(string empId, string empName, double salary, string dept, vector<string> skillList)
        : Person(empId, empName), baseSalary(salary), department(dept) {

        skillCount = (int)skillList.size();
        skills = new string[skillCount];           // allocate heap memory ourselves
        for (int i = 0; i < skillCount; i++) {
            skills[i] = skillList[i];
        }
        employeeCount++;
        cout << "[Constructor] Parameterized Employee constructor called for \"" << name << "\".\n";
    }

    // ---------------------------------------------------------------------------
    // [CONCEPT: COPY CONSTRUCTOR]
    // ---------------------------------------------------------------------------
    // Runs when a NEW object is created FROM an EXISTING object, e.g. "Employee e2(e1);"
    // We must write this OURSELVES because "skills" is a raw pointer. If we let the compiler
    // generate a default copy constructor, it would just copy the POINTER (shallow copy) -
    // meaning both e1 and e2 would point to the SAME memory, and deleting one would corrupt
    // the other! So instead we do a DEEP COPY: allocate brand-new memory and copy the values.
    Employee(const Employee& other) : Person(other.id, other.name) {
        baseSalary = other.baseSalary;
        department = other.department;
        skillCount = other.skillCount;
        skills = new string[skillCount];           // NEW, separate memory block
        for (int i = 0; i < skillCount; i++) {
            skills[i] = other.skills[i];            // copy each value one by one (deep copy)
        }
        employeeCount++;
        cout << "[Copy Constructor] Deep copy created for \"" << name
             << "\" (new memory address: " << skills << ")\n";
    }

    // ---------------------------------------------------------------------------
    // [CONCEPT: MOVE CONSTRUCTOR]
    // ---------------------------------------------------------------------------
    // Runs when a new object is built from a TEMPORARY/disposable object using std::move().
    // Instead of copying the skills array (slow, wastes memory), we simply STEAL the pointer
    // from "other" and set other's pointer to nullptr. This is much FASTER than a deep copy
    // because no new memory allocation or value-copying happens - we just transfer ownership.
    // "noexcept" tells the compiler this function will not throw an error (good practice).
    Employee(Employee&& other) noexcept : Person(other.id, other.name) {
        baseSalary = other.baseSalary;
        department = other.department;
        skillCount = other.skillCount;
        skills = other.skills;        // steal the pointer (same memory address, no copying!)

        // Leave "other" in a safe, empty state so its destructor doesn't double-delete memory.
        other.skills = nullptr;
        other.skillCount = 0;

        employeeCount++;
        cout << "[Move Constructor] Resources MOVED (stolen) for \"" << name
             << "\" (re-used memory address: " << skills << ")\n";
    }

    // ---------------------------------------------------------------------------
    // [CONCEPT: DESTRUCTOR] (overriding the base virtual destructor)
    // ---------------------------------------------------------------------------
    // "override" keyword tells the compiler "I am intentionally overriding a virtual function
    // from the base class" - if we mis-type the signature, the compiler will give an error
    // instead of silently creating an unrelated new function.
    ~Employee() override {
        if (skills != nullptr) {
            delete[] skills;     // free the heap array WE allocated (prevents memory leak)
        }
        cout << "[Destructor] Employee part of \"" << name << "\" destroyed.\n";
    }

    // ---------------------------------------------------------------------------
    // [CONCEPT: SETTERS AND GETTERS]
    // ---------------------------------------------------------------------------
    double getBaseSalary() const { return baseSalary; }
    void setBaseSalary(double salary) { baseSalary = salary; }

    string getDepartment() const { return department; }
    void setDepartment(const string& dept) { department = dept; }

    int getSkillCount() const { return skillCount; }
    string getSkillAt(int index) const {
        if (index >= 0 && index < skillCount) return skills[index];
        return "";
    }

    // [CONCEPT: STATIC MEMBER FUNCTION] - can be called WITHOUT any object: Employee::getEmployeeCount()
    static int getEmployeeCount() { return employeeCount; }

    // ---------------------------------------------------------------------------
    // [CONCEPT: VIRTUAL FUNCTION] (NOT pure virtual - it HAS a body!)
    // ---------------------------------------------------------------------------
    // This is a NORMAL virtual function. Unlike Person::display(), this one is NOT forced
    // to be overridden - it already has a working default behaviour. Derived classes
    // (Manager/Developer/Intern) are FREE to override it if they need different salary logic,
    // but they are not required to. This is the key difference from a pure virtual function.
    virtual double calculateSalary() const {
        return baseSalary;   // default: a plain Employee earns exactly their base salary
    }

    // ---------------------------------------------------------------------------
    // [CONCEPT: FUNCTION OVERRIDING] - overriding Person's pure virtual display()
    // ---------------------------------------------------------------------------
    void display() const override {
        cout << "----------------------------------------------------\n";
        cout << "Role        : " << getRole() << "\n";
        cout << "ID          : " << id << "\n";
        cout << "Name        : " << name << "\n";
        cout << "Department  : " << department << "\n";
        cout << "Base Salary : $" << fixed << setprecision(2) << baseSalary << "\n";
        cout << "Skills      : ";
        for (int i = 0; i < skillCount; i++) cout << skills[i] << (i < skillCount - 1 ? ", " : "");
        if (skillCount == 0) cout << "(none)";
        cout << "\n";
        // calculateSalary() is called here. Because it is VIRTUAL, if this display() is called
        // through a base class pointer pointing at a Manager/Developer/Intern object, C++ will
        // automatically run THAT derived class's overridden calculateSalary() instead of this
        // one. THIS is RUN-TIME POLYMORPHISM in action.
        cout << "Final Salary: $" << fixed << setprecision(2) << calculateSalary() << "\n";
    }

    string getRole() const override { return "Employee"; }

    // ---------------------------------------------------------------------------
    // [CONCEPT: FUNCTION OVERLOADING]
    // ---------------------------------------------------------------------------
    // Two member functions with the SAME NAME "addBonus" but DIFFERENT parameter lists.
    // The compiler decides WHICH one to call based on how many/what type of arguments you pass
    // - this decision happens at COMPILE TIME, which is why it's called STATIC polymorphism.
    void addBonus(double amount) {
        baseSalary += amount;
        cout << name << " received a bonus of $" << amount << ".\n";
    }
    void addBonus(double amount, const string& reason) {     // <-- overloaded version
        baseSalary += amount;
        cout << name << " received a bonus of $" << amount << " for: " << reason << ".\n";
    }

    // ---------------------------------------------------------------------------
    // [CONCEPT: OPERATOR OVERLOADING] (Bonus)
    // ---------------------------------------------------------------------------
    // We are giving the "<<" operator a NEW meaning when used with cout and an Employee object,
    // e.g. "cout << someEmployee;" will now print a short summary line automatically.
    // It must be a "friend" function because it needs to access private/protected data,
    // but it is not a member function itself (the left-hand side is cout, not an Employee).
    friend ostream& operator<<(ostream& os, const Employee& e) {
        os << "[" << e.getRole() << "] " << e.id << " - " << e.name
           << " ($" << fixed << setprecision(2) << e.calculateSalary() << ")";
        return os;
    }
};

// A static member variable must be defined ONCE outside the class (this just reserves its memory).
int Employee::employeeCount = 0;


// ====================================================================================================
// SECTION 3: "Manager", "Developer", "Intern"  -> inherit from Employee (MULTILEVEL INHERITANCE)
// ====================================================================================================
// [CONCEPT: MULTILEVEL / HIERARCHICAL INHERITANCE]
// The chain is: Person -> Employee -> Manager (3 levels deep = multilevel).
// Manager, Developer and Intern all inherit from the SAME parent (Employee) = hierarchical inheritance.

class Manager : public Employee {
private:
    int teamSize;
    double bonusPercent;   // extra % of base salary given as a management bonus

public:
    Manager() : Employee(), teamSize(0), bonusPercent(0.0) {
        cout << "[Constructor] Default Manager constructor called.\n";
    }

    Manager(string id, string name, double salary, string dept, vector<string> skills,
            int team, double bonusPct)
        : Employee(id, name, salary, dept, skills), teamSize(team), bonusPercent(bonusPct) {
        cout << "[Constructor] Parameterized Manager constructor called for \"" << name << "\".\n";
    }

    // [CONCEPT: DESTRUCTOR] - runs automatically AFTER Manager's own cleanup is done,
    // C++ will then automatically call ~Employee() and finally ~Person() (reverse order
    // of construction: most-derived destructor runs first, base destructor runs last).
    ~Manager() override {
        cout << "[Destructor] Manager part of \"" << getName() << "\" destroyed.\n";
    }

    int getTeamSize() const { return teamSize; }
    void setTeamSize(int size) { teamSize = size; }

    double getBonusPercent() const { return bonusPercent; }
    void setBonusPercent(double pct) { bonusPercent = pct; }

    // [CONCEPT: FUNCTION OVERRIDING + RUN-TIME POLYMORPHISM]
    // We are EXTENDING the base behaviour: first call Employee's version (base salary),
    // then add the management bonus on top of it.
    double calculateSalary() const override {
        double base = Employee::calculateSalary();           // re-use the parent's logic
        return base + (base * bonusPercent / 100.0) + (teamSize * 50.0); // + $50 per team member
    }

    void display() const override {
        Employee::display();    // re-use Employee's display() instead of repeating that code
        cout << "Team Size   : " << teamSize << "\n";
        cout << "Mgmt Bonus  : " << bonusPercent << "%\n";
        cout << "----------------------------------------------------\n";
    }

    string getRole() const override { return "Manager"; }
};


class Developer : public Employee {
private:
    string primaryLanguage;
    int projectsCompleted;

public:
    Developer() : Employee(), primaryLanguage("N/A"), projectsCompleted(0) {
        cout << "[Constructor] Default Developer constructor called.\n";
    }

    Developer(string id, string name, double salary, string dept, vector<string> skills,
               string lang, int projects)
        : Employee(id, name, salary, dept, skills), primaryLanguage(lang), projectsCompleted(projects) {
        cout << "[Constructor] Parameterized Developer constructor called for \"" << name << "\".\n";
    }

    ~Developer() override {
        cout << "[Destructor] Developer part of \"" << getName() << "\" destroyed.\n";
    }

    string getPrimaryLanguage() const { return primaryLanguage; }
    void setPrimaryLanguage(const string& lang) { primaryLanguage = lang; }

    int getProjectsCompleted() const { return projectsCompleted; }
    void setProjectsCompleted(int count) { projectsCompleted = count; }

    // [CONCEPT: FUNCTION OVERRIDING] - a DIFFERENT salary formula than Manager's, proving that
    // each class can interpret the SAME function name (calculateSalary) in its OWN way.
    // This is the heart of polymorphism: same function call, different result, depending on
    // the ACTUAL object type at run time.
    double calculateSalary() const override {
        double base = Employee::calculateSalary();
        return base + (projectsCompleted * 200.0);   // $200 bonus per completed project
    }

    void display() const override {
        Employee::display();
        cout << "Language    : " << primaryLanguage << "\n";
        cout << "Projects    : " << projectsCompleted << "\n";
        cout << "----------------------------------------------------\n";
    }

    string getRole() const override { return "Developer"; }
};


class Intern : public Employee {
private:
    double stipend;
    int durationMonths;

public:
    Intern() : Employee(), stipend(0.0), durationMonths(0) {
        cout << "[Constructor] Default Intern constructor called.\n";
    }

    Intern(string id, string name, string dept, vector<string> skills, double stip, int months)
        : Employee(id, name, 0.0, dept, skills), stipend(stip), durationMonths(months) {
        cout << "[Constructor] Parameterized Intern constructor called for \"" << name << "\".\n";
    }

    ~Intern() override {
        cout << "[Destructor] Intern part of \"" << getName() << "\" destroyed.\n";
    }

    double getStipend() const { return stipend; }
    void setStipend(double s) { stipend = s; }

    int getDurationMonths() const { return durationMonths; }
    void setDurationMonths(int months) { durationMonths = months; }

    // [CONCEPT: FUNCTION OVERRIDING] - this override COMPLETELY REPLACES the base logic
    // (it does not call Employee::calculateSalary() at all) - an intern earns ONLY a stipend,
    // proving that overriding can either EXTEND (like Manager/Developer) or fully REPLACE
    // (like here) the parent's behaviour. Both are valid uses of overriding.
    double calculateSalary() const override {
        return stipend;
    }

    void display() const override {
        Employee::display();
        cout << "Stipend     : $" << fixed << setprecision(2) << stipend << "\n";
        cout << "Duration    : " << durationMonths << " month(s)\n";
        cout << "----------------------------------------------------\n";
    }

    string getRole() const override { return "Intern"; }
};


// ====================================================================================================
// SECTION 4: TEMPLATES
// ====================================================================================================

// [CONCEPT: TEMPLATE FUNCTION]
// A normal function only works with ONE specific data type. A TEMPLATE function works with
// ANY data type (int, double, string...) because "T" is just a placeholder that the compiler
// replaces with the real type WHEN you actually call the function (this is called "template
// instantiation" and happens at compile time - so this is ALSO a form of compile-time/static
// polymorphism, just like function overloading).
template <typename T>
T getMax(T a, T b) {
    return (a > b) ? a : b;
}

// [CONCEPT: TEMPLATE CLASS]
// Just like a template function, a TEMPLATE CLASS is a "blueprint" class that can store/manage
// ANY data type. Here Repository<T> can hold a list of ints, strings, doubles, or even pointers
// to our own Employee objects - all using the EXACT SAME code, written only once.
template <typename T>
class Repository {
private:
    vector<T> items;
    string repositoryName;

public:
    Repository(string name) : repositoryName(name) {}

    void addItem(T item) {
        items.push_back(item);
    }

    int count() const {
        return (int)items.size();
    }

    T getItemAt(int index) const {
        return items[index];
    }

    void displaySummary() const {
        cout << "Repository \"" << repositoryName << "\" currently holds " << count() << " item(s).\n";
    }
};


// ====================================================================================================
// SECTION 5: "Department" CLASS  -> demonstrates AGGREGATION/COMPOSITION + POLYMORPHIC STORAGE
// ====================================================================================================
// [CONCEPT: AGGREGATION/COMPOSITION]
// A Department "has" many Person objects. We store them as "Person*" (base class pointers) so that
// ONE single vector can hold Manager, Developer AND Intern objects together, side by side.
// Calling display()/calculateSalary() through these base pointers is the CLASSIC example of
// RUN-TIME POLYMORPHISM: the SAME line of code "p->display();" produces DIFFERENT output
// depending on whether "p" is actually pointing to a Manager, Developer, or Intern object.
class Department {
private:
    string departmentName;
    vector<Person*> employees;   // polymorphic container: holds objects of DIFFERENT derived types

public:
    Department(string name) : departmentName(name) {
        cout << "[Constructor] Department \"" << departmentName << "\" created.\n";
    }

    // [CONCEPT: DESTRUCTOR - cleaning up dynamically allocated objects]
    // We manually "new"-ed every employee, so we must manually "delete" them here, otherwise
    // we get a memory leak. Because Person's destructor is VIRTUAL, each "delete employees[i];"
    // call correctly triggers the FULL chain of destructors (e.g. ~Manager -> ~Employee -> ~Person).
    ~Department() {
        cout << "[Destructor] Cleaning up Department \"" << departmentName << "\"...\n";
        for (Person* p : employees) {
            delete p;
        }
        employees.clear();
    }

    void addEmployee(Person* p) {
        employees.push_back(p);
    }

    bool removeEmployeeById(const string& id) {
        for (size_t i = 0; i < employees.size(); i++) {
            if (employees[i]->getId() == id) {
                cout << "Removing \"" << employees[i]->getName() << "\" from " << departmentName << "...\n";
                delete employees[i];                 // virtual destructor -> correct cleanup
                employees.erase(employees.begin() + i);
                return true;
            }
        }
        return false;
    }

    Person* findById(const string& id) const {
        for (Person* p : employees) {
            if (p->getId() == id) return p;
        }
        return nullptr;
    }

    // [CONCEPT: RUN-TIME POLYMORPHISM]
    // "p->display()" looks like ONE function call, but at run time C++ checks the REAL type
    // of the object "p" is pointing to, and calls THAT class's display() automatically.
    void displayAll() const {
        cout << "\n========== Employees in \"" << departmentName << "\" ==========\n";
        if (employees.empty()) {
            cout << "(No employees yet)\n";
            return;
        }
        for (Person* p : employees) {
            p->display();   // <-- polymorphic call
        }
    }

    // [CONCEPT: RUN-TIME POLYMORPHISM]
    // Same idea: "p->calculateSalary()" automatically uses the Manager/Developer/Intern
    // version, even though the loop only ever talks about a generic "Person*".
    double calculateTotalPayroll() const {
        double total = 0.0;
        for (Person* p : employees) {
            // calculateSalary() is declared in Employee (not Person), so we must check/cast.
            Employee* emp = dynamic_cast<Employee*>(p);
            if (emp != nullptr) {
                total += emp->calculateSalary();
            }
        }
        return total;
    }

    int getEmployeeListCount() const { return (int)employees.size(); }
    string getDepartmentName() const { return departmentName; }
};


// ====================================================================================================
// SECTION 6: SMALL CONSOLE INPUT HELPERS (just for a smooth menu experience)
// ====================================================================================================
int readInt(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
        // Safety check: if the input stream has truly ended (EOF), stop looping forever
        // and shut down gracefully instead of spamming error messages endlessly.
        if (cin.eof()) {
            cout << "\n>>> Input stream ended. Closing program.\n";
            exit(0);
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << ">>> Invalid input, please enter a whole number.\n";
    }
}

double readDouble(const string& prompt) {
    double value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
        if (cin.eof()) {
            cout << "\n>>> Input stream ended. Closing program.\n";
            exit(0);
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << ">>> Invalid input, please enter a number.\n";
    }
}

string readLine(const string& prompt) {
    string value;
    cout << prompt;
    if (!getline(cin, value)) {
        cout << "\n>>> Input stream ended. Closing program.\n";
        exit(0);
    }
    return value;
}


// ====================================================================================================
// SECTION 7: "ManagementSystem" CLASS  -> the main menu-driven controller of the whole program
// ====================================================================================================
class ManagementSystem {
private:
    Department department;   // [CONCEPT: COMPOSITION] - ManagementSystem "owns" one Department

public:
    ManagementSystem() : department("Head Office") {}

    void run() {
        int choice;
        do {
            cout << "\n===================== EMPLOYEE MANAGEMENT SYSTEM =====================\n"
                 << "1. Add a Manager\n"
                 << "2. Add a Developer\n"
                 << "3. Add an Intern\n"
                 << "4. Display All Employees (Polymorphism Demo)\n"
                 << "5. Show Total Monthly Payroll (Polymorphism Demo)\n"
                 << "6. Search Employee by ID\n"
                 << "7. Remove Employee by ID\n"
                 << "8. Demonstrate Copy Constructor vs Move Constructor\n"
                 << "9. Demonstrate Templates (Repository<T> + getMax<T>)\n"
                 << "10. Show Total Employees Ever Created (Static Member Demo)\n"
                 << "11. Exit\n"
                 << "========================================================================\n";
            choice = readInt("Enter your choice (1-11): ");

            switch (choice) {
                case 1: addManager(); break;
                case 2: addDeveloper(); break;
                case 3: addIntern(); break;
                case 4: department.displayAll(); break;
                case 5:
                    cout << "\nTotal Monthly Payroll for \"" << department.getDepartmentName()
                         << "\": $" << fixed << setprecision(2)
                         << department.calculateTotalPayroll() << "\n";
                    break;
                case 6: searchEmployee(); break;
                case 7: removeEmployee(); break;
                case 8: demonstrateCopyAndMove(); break;
                case 9: demonstrateTemplates(); break;
                case 10:
                    // Static member function call WITHOUT any object -> Employee::getEmployeeCount()
                    cout << "Total Employee objects ever constructed (static counter): "
                         << Employee::getEmployeeCount() << "\n";
                    break;
                case 11: cout << "Exiting... Goodbye!\n"; break;
                default: cout << ">>> Invalid choice, try again.\n";
            }
        } while (choice != 11);
    }

private:
    vector<string> readSkills() {
        vector<string> skills;
        int n = readInt("How many skills to add? ");
        for (int i = 0; i < n; i++) {
            skills.push_back(readLine("  Enter skill #" + to_string(i + 1) + ": "));
        }
        return skills;
    }

    void addManager() {
        string id = readLine("Enter Employee ID: ");
        string name = readLine("Enter Name: ");
        double salary = readDouble("Enter Base Salary: $");
        string dept = readLine("Enter Department: ");
        vector<string> skills = readSkills();
        int team = readInt("Enter Team Size: ");
        double bonus = readDouble("Enter Bonus Percentage: ");

        // "new" creates the object on the heap; we store the result as a Person* (upcasting),
        // which is exactly what allows polymorphism to work later.
        Person* m = new Manager(id, name, salary, dept, skills, team, bonus);
        department.addEmployee(m);
        cout << ">>> Manager added successfully!\n";
    }

    void addDeveloper() {
        string id = readLine("Enter Employee ID: ");
        string name = readLine("Enter Name: ");
        double salary = readDouble("Enter Base Salary: $");
        string dept = readLine("Enter Department: ");
        vector<string> skills = readSkills();
        string lang = readLine("Enter Primary Programming Language: ");
        int projects = readInt("Enter Number of Projects Completed: ");

        Person* d = new Developer(id, name, salary, dept, skills, lang, projects);
        department.addEmployee(d);
        cout << ">>> Developer added successfully!\n";
    }

    void addIntern() {
        string id = readLine("Enter Employee ID: ");
        string name = readLine("Enter Name: ");
        string dept = readLine("Enter Department: ");
        vector<string> skills = readSkills();
        double stipend = readDouble("Enter Monthly Stipend: $");
        int months = readInt("Enter Internship Duration (months): ");

        Person* i = new Intern(id, name, dept, skills, stipend, months);
        department.addEmployee(i);
        cout << ">>> Intern added successfully!\n";
    }

    void searchEmployee() {
        string id = readLine("Enter Employee ID to search: ");
        Person* found = department.findById(id);
        if (found != nullptr) {
            cout << "\n>>> Employee found:\n";
            found->display();   // polymorphic call again
        } else {
            cout << ">>> No employee found with ID: " << id << "\n";
        }
    }

    void removeEmployee() {
        string id = readLine("Enter Employee ID to remove: ");
        if (department.removeEmployeeById(id)) {
            cout << ">>> Employee removed successfully.\n";
        } else {
            cout << ">>> No employee found with that ID.\n";
        }
    }

    // [CONCEPT DEMO] Copy Constructor vs Move Constructor, side by side, with proof via memory address.
    void demonstrateCopyAndMove() {
        cout << "\n--- Building a sample Employee to copy/move ---\n";
        Employee original("DEMO-1", "Sample Employee", 50000, "Demo Dept",
                           vector<string>{"C++", "OOP", "Problem Solving"});

        cout << "\n--- COPY CONSTRUCTOR (Employee copy = original;) ---\n";
        Employee copy = original;   // invokes the COPY constructor (deep copy, NEW memory address)

        cout << "\n--- MOVE CONSTRUCTOR (Employee moved = std::move(original);) ---\n";
        Employee moved = std::move(original);   // invokes the MOVE constructor (steals memory, SAME address)

        cout << "\nNotice above: the Copy Constructor printed a DIFFERENT memory address than the "
             << "original (because it made a brand-new deep copy), while the Move Constructor "
             << "printed the SAME memory address as the original (because it just transferred "
             << "ownership instead of copying anything).\n";

        cout << "\nFinal state of \"copy\" object:\n";
        copy.display();
    }

    // [CONCEPT DEMO] Templates working with completely different data types using the SAME code.
    void demonstrateTemplates() {
        cout << "\n--- Template Function getMax<T>() ---\n";
        cout << "getMax(10, 25)         -> " << getMax(10, 25) << " (works with int)\n";
        cout << "getMax(3.5, 2.1)       -> " << getMax(3.5, 2.1) << " (works with double)\n";
        cout << "getMax(string(\"abc\"), string(\"xyz\")) -> "
             << getMax(string("abc"), string("xyz")) << " (works with string)\n";

        cout << "\n--- Template Class Repository<T> ---\n";
        Repository<int> intRepo("Employee IDs (numbers)");
        intRepo.addItem(101);
        intRepo.addItem(102);
        intRepo.displaySummary();

        Repository<string> stringRepo("Department Names");
        stringRepo.addItem("Engineering");
        stringRepo.addItem("Sales");
        stringRepo.addItem("HR");
        stringRepo.displaySummary();

        cout << "Same Repository<T> template class is being reused for BOTH int and string "
             << "without writing any extra code - that is the power of templates.\n";
    }
};


// ====================================================================================================
// SECTION 8: main()  -> program entry point
// ====================================================================================================
int main() {
    cout << "========================================================\n"
         << "   WELCOME TO THE C++ OOP EMPLOYEE MANAGEMENT SYSTEM\n"
         << "========================================================\n";

    ManagementSystem system;   // [CONCEPT: OBJECT CREATION] - object of ManagementSystem class
    system.run();

    return 0;
}   // When main() ends, "system" goes out of scope, its Department member is destroyed,
    // which automatically deletes every remaining Employee object - watch the destructor
    // trace messages print in the console to see the full cleanup chain in action!
