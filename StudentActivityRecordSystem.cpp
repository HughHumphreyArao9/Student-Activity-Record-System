#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <sstream>
#include <fstream>

using namespace std;

class Student {
private:
    string name;
    string section;
    vector<string> activities;

public:
    Student(const string& n, const string& s) : name(n), section(s) {}

    string get_name() const {
        return name;
    }

    string get_section() const {
        return section;
    }

    const vector<string>& get_activities() const {
        return activities;
    }

    void add_activity(const string& activity) {
        activities.push_back(activity);
    }

    void delete_activity(const string& activity) {
        activities.erase(remove(activities.begin(), activities.end(), activity), activities.end());
    }

    void print_activities() const {
        cout << name << " participates in the following activities:" << endl;
        for (const string& activity : activities) {
            cout << "- " << activity << endl;
        }
    }

    vector<string> get_missing_tasks(const vector<string>& all_tasks) const {
        unordered_set<string> completed_tasks(activities.begin(), activities.end());
        vector<string> missing_tasks;
        for (const string& task : all_tasks) {
            if (completed_tasks.find(task) == completed_tasks.end()) {
                missing_tasks.push_back(task);
            }
        }
        return missing_tasks;
    }
};

void print_students(const vector<Student>& students) {
    cout << "----- Student List -----" << endl;
    for (size_t i = 0; i < students.size(); ++i) {
        const Student& student = students[i];
        cout << i + 1 << ". " << student.get_name() << endl;
        cout << "   Section: " << student.get_section() << endl;
    }
    cout << endl;
}

void save_data(const vector<Student>& students) {
    ofstream outputFile("output.txt");
    if (!outputFile) {
        cerr << "Error opening output file." << endl;
        return;
    }

    for (const Student& student : students) {
        outputFile << "Name: " << student.get_name() << endl;
        outputFile << "Section: " << student.get_section() << endl;
        outputFile << "Activities: ";
        for (const string& activity : student.get_activities()) {
            outputFile << activity << ", ";
        }
        outputFile << endl;
        outputFile << "--------------------" << endl;
    }

    outputFile.close();
    cout << "Data saved successfully!" << endl;
}

void load_data(vector<Student>& students, vector<string>& all_tasks) {
    ifstream inputFile("output.txt");
    if (!inputFile) {
        cerr << "No saved data found." << endl;
        return;
    }

    students.clear();
    all_tasks.clear();

    string line;
    string name, section;
    vector<string> activities;
    while (getline(inputFile, line)) {
        if (line.find("Name: ") != string::npos) {
            name = line.substr(6);
        }
        else if (line.find("Section: ") != string::npos) {
            section = line.substr(9);
        }
        else if (line.find("Activities: ") != string::npos) {
            string activitiesStr = line.substr(12);
            istringstream iss(activitiesStr);
            string activity;
            while (getline(iss, activity, ',')) {
                activity = activity.substr(1); // Remove leading space
                activities.push_back(activity);
            }
        }
        else if (line.find("--------------------") != string::npos) {
            students.emplace_back(name, section);
            for (const string& activity : activities) {
                students.back().add_activity(activity);
            }
            activities.clear();
        }
    }

    for (const Student& student : students) {
        for (const string& activity : student.get_activities()) {
            all_tasks.push_back(activity);
        }
    }

    inputFile.close();
    cout << "Data loaded successfully!" << endl;
}

int main() {
    vector<Student> students;
    vector<string> all_tasks = {"Task1", "Task2", "Task3", "PIT", "Task4"};

    load_data(students, all_tasks);

    bool showMenu = true;
    while (true) {
        if (showMenu) {
            cout << "----- Menu -----" << endl;
            cout << "1. Add/Delete a student" << endl;
            cout << "2. View student list and details" << endl;
            cout << "3. View all tasks" << endl;
            cout << "4. Add/Delete task for a student" << endl;
            cout << "5. Save data" << endl;
            cout << "6. Exit" << endl;
        }

        cout << "Enter your choice (0 to show the menu): ";
        int choice;
        cin >> choice;
        cin.ignore();

        if (choice == 0) {
            showMenu = true;
            continue;
        }
        else if (choice == 1) {
            showMenu = false;
            cout << "----- Add/Delete a Student -----" << endl;
            cout << "1. Add a student" << endl;
            cout << "2. Delete a student" << endl;
            cout << "Enter your choice: ";

            int addDeleteChoice;
            cin >> addDeleteChoice;
            cin.ignore();

            if (addDeleteChoice == 1) {
                cout << "Enter student name: ";
                string name;
                getline(cin, name);

                cout << "Enter student section: ";
                string section;
                getline(cin, section);
                Student student(name, section);

                cout << "Available tasks: ";
                for (const string& task : all_tasks) {
                    cout << task << ", ";
                }
                cout << endl;

                cout << "Enter student tasks (separated by spaces): ";
                string tasksInput;
                getline(cin, tasksInput);
                istringstream iss(tasksInput);
                string task;
                while (iss >> task) {
                    if (find(all_tasks.begin(), all_tasks.end(), task) != all_tasks.end()) {
                        student.add_activity(task);
                        cout << "Task '" << task << "' added successfully!" << endl;
                    }
                    else {
                        cout << "Invalid task: '" << task << "'. Please enter a task from the available tasks." << endl;
                    }
                }

                students.push_back(student);
                cout << "Student added successfully!" << endl;
            }
            else if (addDeleteChoice == 2) {
                cout << "----- Delete a Student -----" << endl;
                if (students.empty()) {
                    cout << "No students available for deletion." << endl;
                }
                else {
                    print_students(students);
                    cout << "Enter student index to delete (1-" << students.size() << "): ";
                    int index;
                    cin >> index;
                    cin.ignore();

                    if (index >= 1 && index <= students.size()) {
                        students.erase(students.begin() + index - 1);
                        cout << "Student deleted successfully!" << endl;
                    }
                    else {
                        cout << "Invalid student index!" << endl;
                    }
                }
            }
            else {
                cout << "Invalid choice! Please try again." << endl;
            }
        }
        else if (choice == 2) {
            if (students.empty()) {
                cout << "No students available to view." << endl;
            }
            else {
                print_students(students);
                cout << "Choose from the list above (1-" << students.size() << "): ";
                int index;
                cin >> index;
                cin.ignore();

                if (index >= 1 && index <= students.size()) {
                    const Student& student = students[index - 1];

                    cout << "----- Student's Finished and Missing Tasks -----" << endl;
                    cout << "Student Name: " << student.get_name() << endl;
                    cout << "Student Section: " << student.get_section() << endl;

                    vector<string> missing_tasks = student.get_missing_tasks(all_tasks);
                    cout << "Finished tasks: ";
                    for (const string& activity : student.get_activities()) {
                        cout << activity << ", ";
                    }
                    cout << endl;
                    cout << "Missing tasks: ";
                    for (const string& task : missing_tasks) {
                        cout << task << ", ";
                    }
                    cout << endl;
                }
                else {
                    cout << "Invalid student index!" << endl;
                }
            }
        }
        else if (choice == 3) {
            cout << "----- All Tasks -----" << endl;
            cout << "Available tasks: ";
            for (const string& task : all_tasks) {
                cout << task << ", ";
            }
            cout << endl;
        }
        else if (choice == 4) {
            if (students.empty()) {
                cout << "No students available to add/delete tasks." << endl;
            }
            else {
                print_students(students);
                cout << "Choose from the list above (1-" << students.size() << "): ";
                int index;
                cin >> index;
                cin.ignore();

                if (index >= 1 && index <= students.size()) {
                    Student& student = students[index - 1];

                    cout << "Available tasks: ";
                    for (const string& task : all_tasks) {
                        cout << task << ", ";
                    }
                    cout << endl;

                    cout << "1. Add task" << endl;
                    cout << "2. Delete task" << endl;
                    cout << "Enter your choice: ";

                    int addDeleteTaskChoice;
                    cin >> addDeleteTaskChoice;
                    cin.ignore();

                    if (addDeleteTaskChoice == 1) {
                        cout << "Enter task to add: ";
                        string task;
                        getline(cin, task);

                        if (find(all_tasks.begin(), all_tasks.end(), task) != all_tasks.end()) {
                            student.add_activity(task);
                            cout << "Task '" << task << "' added successfully!" << endl;
                        }
                        else {
                            cout << "Invalid task: '" << task << "'. Please enter a task from the available tasks." << endl;
                        }
                    }
                    else if (addDeleteTaskChoice == 2) {
                        cout << "Enter task to delete: ";
                        string task;
                        getline(cin, task);
                        if (find(all_tasks.begin(), all_tasks.end(), task) != all_tasks.end()) {
                            student.delete_activity(task);
                            cout << "Task '" << task << "' deleted successfully!" << endl;
                        }
                        else {
                            cout << "Invalid task: '" << task << "'. Please enter a task from the available tasks." << endl;
                        }
                    }
                    else {
                        cout << "Invalid choice! Please try again." << endl;
                    }
                }
                else {
                    cout << "Invalid student index!" << endl;
                }
            }
        }
        else if (choice == 5) {
            save_data(students);
        }
        else if (choice == 6) {
            break;
        }
        else {
            cout << "Invalid choice! Please try again." << endl;
        }
    }

    return 0;
}