
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
using namespace std;

struct Student {
    int id;
    string name;
    int semester;
    double gpa;
};

class StudentManagementSystem {
private:
    Student students[100];
    int count;
    string fileName;

    int readInt(string message, int minValue, int maxValue) {
        int value;

        while (true) {
            cout << message;

            if (cin >> value && value >= minValue && value <= maxValue) {
                cin.ignore(1000, '\n');
                return value;
            }

            cout << "Invalid input. Enter a number between "
                 << minValue << " and " << maxValue << ".\n";

            cin.clear();
            cin.ignore(1000, '\n');
        }
    }

    double readGPA(string message) {
        double value;

        while (true) {
            cout << message;

            if (cin >> value && value >= 0.0 && value <= 4.0) {
                cin.ignore(1000, '\n');
                return value;
            }

            cout << "Invalid GPA. Enter a value from 0.0 to 4.0.\n";

            cin.clear();
            cin.ignore(1000, '\n');
        }
    }

    string readName(string message) {
        string name;

        while (true) {
            cout << message;
            getline(cin, name);

            if (name.find_first_not_of(" \t") != string::npos) {
                return name;
            }

            cout << "Name cannot be empty. Try again.\n";
        }
    }

    int findStudent(int id) {
        for (int i = 0; i < count; i++) {
            if (students[i].id == id) {
                return i;
            }
        }

        return -1;
    }

    void loadFromFile() {
        ifstream inFile(fileName);

        if (!inFile) {
            return;
        }

        count = 0;

        while (count < 100) {
            Student s;

            if (!(inFile >> s.id >> s.semester >> s.gpa)) {
                break;
            }

            inFile.ignore(1000, '\n');
            getline(inFile, s.name);

            if (s.id > 0 && s.semester >= 1 &&
                s.semester <= 12 && s.gpa >= 0.0 &&
                s.gpa <= 4.0 && findStudent(s.id) == -1 &&
                s.name.find_first_not_of(" \t") != string::npos) {

                students[count] = s;
                count++;
            }
        }

        inFile.close();
    }

    void saveToFile() {
        ofstream outFile(fileName);

        if (!outFile) {
            cout << "Error: Unable to save records.\n";
            return;
        }

        for (int i = 0; i < count; i++) {
            outFile << students[i].id << " "
                    << students[i].semester << " "
                    << fixed << setprecision(2)
                    << students[i].gpa << "\n";

            outFile << students[i].name << "\n";
        }

        outFile.close();
    }

public:
    StudentManagementSystem() {
        count = 0;
        fileName = "students.txt";
        loadFromFile();
    }

    void addStudent() {
        if (count >= 100) {
            cout << "\nStudent limit reached. Cannot add more.\n";
            return;
        }

        cout << "\n--- Add New Student ---\n";

        int id = readInt("Enter Student ID: ", 1, 1000000000);

        if (findStudent(id) != -1) {
            cout << "A student with this ID already exists.\n";
            return;
        }

        students[count].id = id;
        students[count].name = readName("Enter Student Name: ");
        students[count].semester =
            readInt("Enter Semester (1-12): ", 1, 12);
        students[count].gpa = readGPA("Enter GPA (0.0-4.0): ");

        count++;
        saveToFile();

        cout << "\nStudent added successfully!\n";
    }

    void displayStudents() {
        cout << "\n--- All Students ---\n";

        if (count == 0) {
            cout << "No student records found.\n";
            return;
        }

        cout << left
             << setw(12) << "ID"
             << setw(25) << "Name"
             << setw(12) << "Semester"
             << setw(10) << "GPA" << "\n";

        cout << string(59, '-') << "\n";

        for (int i = 0; i < count; i++) {
            cout << left
                 << setw(12) << students[i].id
                 << setw(25) << students[i].name
                 << setw(12) << students[i].semester
                 << fixed << setprecision(2)
                 << students[i].gpa << "\n";
        }
    }

    void searchStudent() {
        cout << "\n--- Search Student ---\n";

        int id = readInt("Enter Student ID: ", 1, 1000000000);
        int index = findStudent(id);

        if (index == -1) {
            cout << "Student not found.\n";
            return;
        }

        cout << "\nStudent Found!\n";
        cout << "ID: " << students[index].id << "\n";
        cout << "Name: " << students[index].name << "\n";
        cout << "Semester: " << students[index].semester << "\n";
        cout << "GPA: " << fixed << setprecision(2)
             << students[index].gpa << "\n";
    }

    void updateStudent() {
        cout << "\n--- Update Student ---\n";

        int id = readInt("Enter Student ID: ", 1, 1000000000);
        int index = findStudent(id);

        if (index == -1) {
            cout << "Student not found.\n";
            return;
        }

        cout << "\nCurrent student details:\n";
        cout << "Name: " << students[index].name << "\n";
        cout << "Semester: " << students[index].semester << "\n";
        cout << "GPA: " << students[index].gpa << "\n";

        cout << "\nEnter updated details:\n";

        students[index].name = readName("Enter New Name: ");
        students[index].semester =
            readInt("Enter New Semester (1-12): ", 1, 12);
        students[index].gpa = readGPA("Enter New GPA (0.0-4.0): ");

        saveToFile();

        cout << "\nStudent updated successfully!\n";
    }

    void deleteStudent() {
        cout << "\n--- Delete Student ---\n";

        int id = readInt("Enter Student ID: ", 1, 1000000000);
        int index = findStudent(id);

        if (index == -1) {
            cout << "Student not found.\n";
            return;
        }

        for (int i = index; i < count - 1; i++) {
            students[i] = students[i + 1];
        }

        count--;
        saveToFile();

        cout << "Student deleted successfully!\n";
    }

    void calculateAverageGPA() {
        cout << "\n--- Average GPA ---\n";

        if (count == 0) {
            cout << "No student records available.\n";
            return;
        }

        double total = 0.0;

        for (int i = 0; i < count; i++) {
            total += students[i].gpa;
        }

        double average = total / count;

        cout << "Total Students: " << count << "\n";
        cout << "Average GPA: " << fixed << setprecision(2)
             << average << "\n";
    }

    void run() {
        int choice;

        do {
            cout << "\n====================================\n";
            cout << "     STUDENT MANAGEMENT SYSTEM\n";
            cout << "====================================\n";
            cout << "1. Add Student\n";
            cout << "2. Display All Students\n";
            cout << "3. Search Student\n";
            cout << "4. Update Student\n";
            cout << "5. Delete Student\n";
            cout << "6. Calculate Average GPA\n";
            cout << "0. Save and Exit\n";
            cout << "====================================\n";

            choice = readInt("Enter your choice: ", 0, 6);

            switch (choice) {
                case 1:
                    addStudent();
                    break;

                case 2:
                    displayStudents();
                    break;

                case 3:
                    searchStudent();
                    break;

                case 4:
                    updateStudent();
                    break;

                case 5:
                    deleteStudent();
                    break;

                case 6:
                    calculateAverageGPA();
                    break;

                case 0:
                    saveToFile();
                    cout << "\nRecords saved successfully.\n";
                    cout << "Thank you for using the system!\n";
                    break;
            }

        } while (choice != 0);
    }
};

int main() {
    StudentManagementSystem system;
    system.run();

    return 0;
}
