#include <windows.h>
#include <commctrl.h>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>

#pragma comment(lib, "Comctl32.lib")

#define IDC_TYPE_COMBO      101
#define IDC_ID_EDIT         102
#define IDC_NAME_EDIT       103
#define IDC_DEPT_EDIT       104
#define IDC_SALARY_EDIT     105
#define IDC_SKILLS_EDIT     106
#define IDC_EXTRA_EDIT      107
#define IDC_ADD_BUTTON      108
#define IDC_REMOVE_BUTTON   109
#define IDC_REFRESH_BUTTON  110
#define IDC_EMPLOYEE_LIST   111
#define IDC_PAYROLL_BUTTON  112
#define IDC_STATUS_STATIC   113

class Person {
protected:
    std::wstring id;
    std::wstring name;

public:
    Person(const std::wstring& personId, const std::wstring& personName)
        : id(personId), name(personName) {}
    virtual ~Person() {}
    std::wstring getId() const { return id; }
    std::wstring getName() const { return name; }
    virtual std::wstring getRole() const = 0;
    virtual double calculateSalary() const = 0;
    virtual std::wstring getDisplayText() const = 0;
};

class Employee : public Person {
protected:
    double baseSalary;
    std::wstring department;
    std::vector<std::wstring> skills;

public:
    Employee(const std::wstring& empId, const std::wstring& empName,
             double salary, const std::wstring& dept,
             const std::vector<std::wstring>& skillList)
        : Person(empId, empName), baseSalary(salary), department(dept), skills(skillList) {}

    virtual ~Employee() {}

    double calculateSalary() const override {
        return baseSalary;
    }

    std::wstring getDisplayText() const override {
        std::wostringstream text;
        text << getRole() << L": " << name << L" (" << id << L")";
        return text.str();
    }
};

class Manager : public Employee {
    int teamSize;
    double bonusPercent;

public:
    Manager(const std::wstring& id, const std::wstring& name, double salary,
            const std::wstring& dept, const std::vector<std::wstring>& skills,
            int team, double bonusPct)
        : Employee(id, name, salary, dept, skills), teamSize(team), bonusPercent(bonusPct) {}

    std::wstring getRole() const override { return L"Manager"; }
    double calculateSalary() const override {
        return baseSalary + (baseSalary * bonusPercent / 100.0) + teamSize * 50.0;
    }

    std::wstring getDisplayText() const override {
        std::wostringstream text;
        text << getRole() << L": " << name << L" (" << id << L")";
        return text.str();
    }
};

class Developer : public Employee {
    std::wstring primaryLanguage;
    int projectsCompleted;

public:
    Developer(const std::wstring& id, const std::wstring& name, double salary,
              const std::wstring& dept, const std::vector<std::wstring>& skills,
              const std::wstring& language, int projects)
        : Employee(id, name, salary, dept, skills), primaryLanguage(language), projectsCompleted(projects) {}

    std::wstring getRole() const override { return L"Developer"; }
    double calculateSalary() const override {
        return baseSalary + projectsCompleted * 200.0;
    }

    std::wstring getDisplayText() const override {
        std::wostringstream text;
        text << getRole() << L": " << name << L" (" << id << L")";
        return text.str();
    }
};

class Intern : public Employee {
    double stipend;
    int durationMonths;

public:
    Intern(const std::wstring& id, const std::wstring& name,
           const std::wstring& dept, const std::vector<std::wstring>& skills,
           double stip, int months)
        : Employee(id, name, 0.0, dept, skills), stipend(stip), durationMonths(months) {}

    std::wstring getRole() const override { return L"Intern"; }
    double calculateSalary() const override {
        return stipend;
    }

    std::wstring getDisplayText() const override {
        std::wostringstream text;
        text << getRole() << L": " << name << L" (" << id << L")";
        return text.str();
    }
};

class Department {
    std::vector<Person*> employees;

public:
    ~Department() {
        for (Person* person : employees) {
            delete person;
        }
        employees.clear();
    }

    void addEmployee(Person* person) {
        employees.push_back(person);
    }

    void removeAt(int index) {
        if (index >= 0 && index < (int)employees.size()) {
            delete employees[index];
            employees.erase(employees.begin() + index);
        }
    }

    Person* getAt(int index) const {
        if (index >= 0 && index < (int)employees.size()) {
            return employees[index];
        }
        return nullptr;
    }

    int count() const {
        return (int)employees.size();
    }

    double calculateTotalPayroll() const {
        double total = 0.0;
        for (Person* person : employees) {
            total += person->calculateSalary();
        }
        return total;
    }
};

Department g_department;
HWND g_statusBar = nullptr;

std::wstring ToWString(const std::wstring& text) {
    return text;
}

std::vector<std::wstring> SplitSkills(const std::wstring& text) {
    std::vector<std::wstring> result;
    std::wistringstream stream(text);
    std::wstring skill;
    while (std::getline(stream, skill, L',')) {
        if (!skill.empty()) {
            while (!skill.empty() && skill.front() == L' ') skill.erase(skill.begin());
            while (!skill.empty() && skill.back() == L' ') skill.pop_back();
            if (!skill.empty()) result.push_back(skill);
        }
    }
    return result;
}

void UpdateStatus(HWND hwnd, const std::wstring& message) {
    SetWindowTextW(g_statusBar, message.c_str());
    InvalidateRect(g_statusBar, nullptr, TRUE);
}

void RefreshEmployeeList(HWND hwnd) {
    HWND list = GetDlgItem(hwnd, IDC_EMPLOYEE_LIST);
    SendMessageW(list, LB_RESETCONTENT, 0, 0);
    for (int i = 0; i < g_department.count(); ++i) {
        Person* person = g_department.getAt(i);
        std::wstring text = person->getDisplayText();
        SendMessageW(list, LB_ADDSTRING, 0, (LPARAM)text.c_str());
    }
    UpdateStatus(hwnd, L"Employees: " + std::to_wstring(g_department.count()));
}

void AddEmployeeFromForm(HWND hwnd) {
    std::wstring type;
    int selectedType = (int)SendDlgItemMessageW(hwnd, IDC_TYPE_COMBO, CB_GETCURSEL, 0, 0);
    if (selectedType == CB_ERR) {
        UpdateStatus(hwnd, L"Select an employee type first.");
        return;
    }
    type.resize(256);
    SendDlgItemMessageW(hwnd, IDC_TYPE_COMBO, CB_GETLBTEXT, selectedType, (LPARAM)type.data());
    type = type.c_str();

    wchar_t buffer[256];
    GetDlgItemTextW(hwnd, IDC_ID_EDIT, buffer, 256);
    std::wstring id = buffer;
    GetDlgItemTextW(hwnd, IDC_NAME_EDIT, buffer, 256);
    std::wstring name = buffer;
    GetDlgItemTextW(hwnd, IDC_DEPT_EDIT, buffer, 256);
    std::wstring dept = buffer;
    GetDlgItemTextW(hwnd, IDC_SALARY_EDIT, buffer, 256);
    std::wstring salaryText = buffer;
    GetDlgItemTextW(hwnd, IDC_SKILLS_EDIT, buffer, 256);
    std::vector<std::wstring> skills = SplitSkills(buffer);
    GetDlgItemTextW(hwnd, IDC_EXTRA_EDIT, buffer, 256);
    std::wstring extra = buffer;

    if (id.empty() || name.empty()) {
        UpdateStatus(hwnd, L"ID and Name are required.");
        return;
    }

    try {
        if (type == L"Manager") {
            double salary = std::stod(salaryText);
            int teamSize = 0;
            double bonus = 0.0;
            std::wistringstream extraStream(extra);
            extraStream >> teamSize >> bonus;
            g_department.addEmployee(new Manager(id, name, salary, dept, skills, teamSize, bonus));
            UpdateStatus(hwnd, L"Manager added.");
        } else if (type == L"Developer") {
            double salary = std::stod(salaryText);
            int projects = 0;
            std::wstring language = extra;
            std::wistringstream extraStream(extra);
            extraStream >> projects;
            if (std::iswalpha(extra[0])) {
                // If user entered language first, swap to parse projects later
                language = extra;
                projects = 0;
            }
            if (language.empty()) {
                UpdateStatus(hwnd, L"Developer language required in extra field.");
                return;
            }
            g_department.addEmployee(new Developer(id, name, salary, dept, skills, language, projects));
            UpdateStatus(hwnd, L"Developer added.");
        } else {
            double stipend = std::stod(salaryText);
            int duration = 0;
            std::wistringstream extraStream(extra);
            extraStream >> duration;
            g_department.addEmployee(new Intern(id, name, dept, skills, stipend, duration));
            UpdateStatus(hwnd, L"Intern added.");
        }
    } catch (...) {
        UpdateStatus(hwnd, L"Invalid numeric values. Check Salary and extra fields.");
        return;
    }

    RefreshEmployeeList(hwnd);
}

void RemoveSelectedEmployee(HWND hwnd) {
    HWND list = GetDlgItem(hwnd, IDC_EMPLOYEE_LIST);
    int index = (int)SendMessageW(list, LB_GETCURSEL, 0, 0);
    if (index == LB_ERR) {
        UpdateStatus(hwnd, L"Select an employee to remove.");
        return;
    }
    g_department.removeAt(index);
    RefreshEmployeeList(hwnd);
    UpdateStatus(hwnd, L"Employee removed.");
}

void ShowTotalPayroll(HWND hwnd) {
    double total = g_department.calculateTotalPayroll();
    std::wostringstream message;
    message << L"Total Payroll: $" << std::fixed << std::setprecision(2) << total;
    MessageBoxW(hwnd, message.str().c_str(), L"Payroll", MB_OK | MB_ICONINFORMATION);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CREATE: {
        CreateWindowW(L"STATIC", L"Employee Type:", WS_VISIBLE | WS_CHILD, 20, 20, 120, 20, hwnd, nullptr, nullptr, nullptr);
        HWND typeCombo = CreateWindowW(L"COMBOBOX", nullptr,
            WS_CHILD | WS_VISIBLE | WS_TABSTOP | CBS_DROPDOWNLIST,
            150, 18, 180, 120, hwnd, (HMENU)IDC_TYPE_COMBO, nullptr, nullptr);
        SendMessageW(typeCombo, CB_ADDSTRING, 0, (LPARAM)L"Manager");
        SendMessageW(typeCombo, CB_ADDSTRING, 0, (LPARAM)L"Developer");
        SendMessageW(typeCombo, CB_ADDSTRING, 0, (LPARAM)L"Intern");
        SendMessageW(typeCombo, CB_SETCURSEL, 0, 0);

        CreateWindowW(L"STATIC", L"ID:", WS_VISIBLE | WS_CHILD, 20, 60, 120, 20, hwnd, nullptr, nullptr, nullptr);
        CreateWindowW(L"EDIT", nullptr, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
            150, 58, 180, 22, hwnd, (HMENU)IDC_ID_EDIT, nullptr, nullptr);

        CreateWindowW(L"STATIC", L"Name:", WS_VISIBLE | WS_CHILD, 20, 100, 120, 20, hwnd, nullptr, nullptr, nullptr);
        CreateWindowW(L"EDIT", nullptr, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
            150, 98, 180, 22, hwnd, (HMENU)IDC_NAME_EDIT, nullptr, nullptr);

        CreateWindowW(L"STATIC", L"Department:", WS_VISIBLE | WS_CHILD, 20, 140, 120, 20, hwnd, nullptr, nullptr, nullptr);
        CreateWindowW(L"EDIT", nullptr, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
            150, 138, 180, 22, hwnd, (HMENU)IDC_DEPT_EDIT, nullptr, nullptr);

        CreateWindowW(L"STATIC", L"Salary / Stipend:", WS_VISIBLE | WS_CHILD, 20, 180, 120, 20, hwnd, nullptr, nullptr, nullptr);
        CreateWindowW(L"EDIT", nullptr, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
            150, 178, 180, 22, hwnd, (HMENU)IDC_SALARY_EDIT, nullptr, nullptr);

        CreateWindowW(L"STATIC", L"Skills (comma separated):", WS_VISIBLE | WS_CHILD, 20, 220, 200, 20, hwnd, nullptr, nullptr, nullptr);
        CreateWindowW(L"EDIT", nullptr, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
            20, 240, 310, 22, hwnd, (HMENU)IDC_SKILLS_EDIT, nullptr, nullptr);

        CreateWindowW(L"STATIC", L"Extra Info:", WS_VISIBLE | WS_CHILD, 20, 280, 120, 20, hwnd, nullptr, nullptr, nullptr);
        CreateWindowW(L"EDIT", nullptr, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
            150, 278, 180, 22, hwnd, (HMENU)IDC_EXTRA_EDIT, nullptr, nullptr);
        CreateWindowW(L"STATIC", L"Manager: team bonus | Dev: language/project | Intern: months", WS_VISIBLE | WS_CHILD, 20, 305, 420, 20, hwnd, nullptr, nullptr, nullptr);

        CreateWindowW(L"BUTTON", L"Add Employee", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
            20, 340, 120, 28, hwnd, (HMENU)IDC_ADD_BUTTON, nullptr, nullptr);
        CreateWindowW(L"BUTTON", L"Remove Selected", WS_CHILD | WS_VISIBLE,
            150, 340, 120, 28, hwnd, (HMENU)IDC_REMOVE_BUTTON, nullptr, nullptr);
        CreateWindowW(L"BUTTON", L"Show Payroll", WS_CHILD | WS_VISIBLE,
            280, 340, 120, 28, hwnd, (HMENU)IDC_PAYROLL_BUTTON, nullptr, nullptr);

        CreateWindowW(L"LISTBOX", nullptr, WS_CHILD | WS_VISIBLE | WS_BORDER | LBS_NOTIFY | WS_VSCROLL,
            360, 20, 300, 320, hwnd, (HMENU)IDC_EMPLOYEE_LIST, nullptr, nullptr);

        g_statusBar = CreateWindowW(L"STATIC", L"Ready.", WS_CHILD | WS_VISIBLE | SS_SUNKEN,
            0, 380, 680, 24, hwnd, (HMENU)IDC_STATUS_STATIC, nullptr, nullptr);
        break;
    }

    case WM_COMMAND: {
        switch (LOWORD(wParam)) {
        case IDC_ADD_BUTTON:
            AddEmployeeFromForm(hwnd);
            break;
        case IDC_REMOVE_BUTTON:
            RemoveSelectedEmployee(hwnd);
            break;
        case IDC_PAYROLL_BUTTON:
            ShowTotalPayroll(hwnd);
            break;
        }
        break;
    }

    case WM_SIZE: {
        if (g_statusBar) {
            RECT rc;
            GetClientRect(hwnd, &rc);
            SetWindowPos(g_statusBar, nullptr, 0, rc.bottom - 24, rc.right, 24, SWP_NOZORDER);
        }
        break;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int nCmdShow) {
    INITCOMMONCONTROLSEX icex = { sizeof(icex), ICC_WIN95_CLASSES };
    InitCommonControlsEx(&icex);

    const wchar_t CLASS_NAME[] = L"EmployeeManagementGUI";

    WNDCLASSW wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    RegisterClassW(&wc);

    HWND hwnd = CreateWindowExW(0, CLASS_NAME, L"Employee Management System",
        WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME,
        CW_USEDEFAULT, CW_USEDEFAULT, 700, 460,
        nullptr, nullptr, hInstance, nullptr);

    if (!hwnd) {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg = {};
    while (GetMessageW(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return 0;
}
