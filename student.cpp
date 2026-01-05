#include <iostream>
#include <fstream>
#include <cstring>
#include <iomanip>

using namespace std;

// Student Class
class Student {
private:
    int rollNo;
    char name[50];
    float marks[5];
    float percentage;
    char grade;

public:
    // Constructor
    Student() {
        rollNo = 0;
        strcpy(name, "");
        for(int i = 0; i < 5; i++) {
            marks[i] = 0.0;
        }
        percentage = 0.0;
        grade = 'F';
    }

    // Input student details
    void inputDetails() {
        cout << "\nEnter Roll Number: ";
        cin >> rollNo;
        cin.ignore();
        
        cout << "Enter Name: ";
        cin.getline(name, 50);
        
        cout << "Enter marks for 5 subjects (0-100):\n";
        for(int i = 0; i < 5; i++) {
            do {
                cout << "  Subject " << (i + 1) << ": ";
                cin >> marks[i];
                if(marks[i] < 0 || marks[i] > 100) {
                    cout << "  Invalid! Enter marks between 0-100.\n";
                }
            } while(marks[i] < 0 || marks[i] > 100);
        }
        
        calculateGrade();
    }

    // Calculate percentage and grade
    void calculateGrade() {
        float total = 0;
        for(int i = 0; i < 5; i++) {
            total += marks[i];
        }
        percentage = total / 5.0;
        
        if(percentage >= 90)
            grade = 'A';
        else if(percentage >= 75)
            grade = 'B';
        else if(percentage >= 60)
            grade = 'C';
        else if(percentage >= 40)
            grade = 'D';
        else
            grade = 'F';
    }

    // Display student details
    void display() const {
        cout << left << setw(10) << rollNo 
             << setw(20) << name 
             << setw(12) << fixed << setprecision(2) << percentage << "%" 
             << setw(8) << grade << endl;
    }

    // Get roll number
    int getRollNo() const {
        return rollNo;
    }

    // Get percentage
    float getPercentage() const {
        return percentage;
    }

    // Get name
    const char* getName() const {
        return name;
    }

    // Get grade
    char getGrade() const {
        return grade;
    }

    // Write to file
    void writeToFile(ofstream &fout) const {
        fout << rollNo << " ";
        // Replace spaces with underscore for name
        for(int i = 0; name[i] != '\0'; i++) {
            if(name[i] == ' ')
                fout << '_';
            else
                fout << name[i];
        }
        fout << " ";
        for(int i = 0; i < 5; i++) {
            fout << marks[i] << " ";
        }
        fout << endl;
    }

    // Read from file
    void readFromFile(ifstream &fin) {
        fin >> rollNo;
        char temp[50];
        fin >> temp;
        // Replace underscore back to space
        int j = 0;
        for(int i = 0; temp[i] != '\0'; i++) {
            if(temp[i] == '_')
                name[j++] = ' ';
            else
                name[j++] = temp[i];
        }
        name[j] = '\0';
        
        for(int i = 0; i < 5; i++) {
            fin >> marks[i];
        }
        calculateGrade();
    }
};

// Global variables
Student students[100];
int studentCount = 0;

// Function prototypes
void displayMenu();
void addStudent();
void displayAllStudents();
void searchStudent();
void deleteStudent();
void updateStudent();
void saveToFile();
void loadFromFile();

// Main function
int main() {
    loadFromFile();
    
    int choice;
    
    do {
        displayMenu();
        cout << "\nEnter your choice: ";
        cin >> choice;
        
        switch(choice) {
            case 1:
                addStudent();
                break;
            case 2:
                displayAllStudents();
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
                saveToFile();
                cout << "\n✓ Data saved successfully!\n";
                cout << "\nThank you for using Student Management System!\n";
                break;
            default:
                cout << "\n✗ Invalid choice! Please try again.\n";
        }
        
        if(choice != 6) {
            cout << "\nPress Enter to continue...";
            cin.ignore();
            cin.get();
        }
        
    } while(choice != 6);
    
    return 0;
}

// Display menu
void displayMenu() {
    cout << "\n";
    cout << "========================================\n";
    cout << "    STUDENT MANAGEMENT SYSTEM\n";
    cout << "========================================\n";
    cout << "1. Add New Student\n";
    cout << "2. Display All Students\n";
    cout << "3. Search Student by Roll Number\n";
    cout << "4. Update Student Details\n";
    cout << "5. Delete Student\n";
    cout << "6. Save & Exit\n";
    cout << "========================================\n";
}

// Add new student
void addStudent() {
    if(studentCount >= 100) {
        cout << "\n✗ Database is full! Cannot add more students.\n";
        return;
    }
    
    cout << "\n----- Add New Student -----\n";
    
    students[studentCount].inputDetails();
    
    // Check for duplicate roll number
    int roll = students[studentCount].getRollNo();
    for(int i = 0; i < studentCount; i++) {
        if(students[i].getRollNo() == roll) {
            cout << "\n✗ Error! Roll number " << roll << " already exists.\n";
            return;
        }
    }
    
    studentCount++;
    cout << "\n✓ Student added successfully!\n";
    cout << "   Percentage: " << fixed << setprecision(2) << students[studentCount-1].getPercentage() << "%\n";
    cout << "   Grade: " << students[studentCount-1].getGrade() << "\n";
}

// Display all students
void displayAllStudents() {
    if(studentCount == 0) {
        cout << "\n✗ No student records found!\n";
        return;
    }
    
    cout << "\n----- All Student Records -----\n\n";
    cout << left << setw(10) << "Roll No" 
         << setw(20) << "Name" 
         << setw(12) << "Percentage" 
         << setw(8) << "Grade" << endl;
    cout << "-----------------------------------------------------------\n";
    
    for(int i = 0; i < studentCount; i++) {
        students[i].display();
    }
    
    cout << "\nTotal Students: " << studentCount << endl;
}

// Search student by roll number
void searchStudent() {
    if(studentCount == 0) {
        cout << "\n✗ No student records found!\n";
        return;
    }
    
    int roll;
    cout << "\n----- Search Student -----\n";
    cout << "Enter Roll Number: ";
    cin >> roll;
    
    bool found = false;
    for(int i = 0; i < studentCount; i++) {
        if(students[i].getRollNo() == roll) {
            cout << "\n✓ Student Found!\n\n";
            cout << left << setw(10) << "Roll No" 
                 << setw(20) << "Name" 
                 << setw(12) << "Percentage" 
                 << setw(8) << "Grade" << endl;
            cout << "-----------------------------------------------------------\n";
            students[i].display();
            found = true;
            break;
        }
    }
    
    if(!found) {
        cout << "\n✗ Student with Roll Number " << roll << " not found!\n";
    }
}

// Update student details
void updateStudent() {
    if(studentCount == 0) {
        cout << "\n✗ No student records found!\n";
        return;
    }
    
    int roll;
    cout << "\n----- Update Student -----\n";
    cout << "Enter Roll Number: ";
    cin >> roll;
    
    bool found = false;
    for(int i = 0; i < studentCount; i++) {
        if(students[i].getRollNo() == roll) {
            cout << "\n✓ Student Found! Enter new details:\n";
            students[i].inputDetails();
            cout << "\n✓ Student details updated successfully!\n";
            found = true;
            break;
        }
    }
    
    if(!found) {
        cout << "\n✗ Student with Roll Number " << roll << " not found!\n";
    }
}

// Delete student
void deleteStudent() {
    if(studentCount == 0) {
        cout << "\n✗ No student records found!\n";
        return;
    }
    
    int roll;
    cout << "\n----- Delete Student -----\n";
    cout << "Enter Roll Number: ";
    cin >> roll;
    
    bool found = false;
    for(int i = 0; i < studentCount; i++) {
        if(students[i].getRollNo() == roll) {
            // Shift all elements after i to left
            for(int j = i; j < studentCount - 1; j++) {
                students[j] = students[j + 1];
            }
            studentCount--;
            cout << "\n✓ Student with Roll Number " << roll << " deleted successfully!\n";
            found = true;
            break;
        }
    }
    
    if(!found) {
        cout << "\n✗ Student with Roll Number " << roll << " not found!\n";
    }
}

// Save data to file
void saveToFile() {
    ofstream fout("students.txt");
    
    if(!fout) {
        cout << "\n✗ Error opening file for writing!\n";
        return;
    }
    
    fout << studentCount << endl;
    for(int i = 0; i < studentCount; i++) {
        students[i].writeToFile(fout);
    }
    
    fout.close();
}

// Load data from file
void loadFromFile() {
    ifstream fin("students.txt");
    
    if(!fin) {
        // File doesn't exist yet, start with empty database
        return;
    }
    
    fin >> studentCount;
    for(int i = 0; i < studentCount; i++) {
        students[i].readFromFile(fin);
    }
    
    fin.close();
}
