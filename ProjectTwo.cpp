//============================================================================
// Name        : ProjectTwo.cpp
// Author      : Valeria Duharte
//============================================================================
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

// Course structure to hold course details
struct Course {
    string courseID;
    string courseName;
    vector<string> prerequisites;
};

class BinarySearchTree {
private:
    struct Node {
        Course course;
        Node* left;
        Node* right;
        Node(Course aCourse) : course(aCourse), left(nullptr), right(nullptr) {}
    };
    Node* root;
    void DestroyBST(Node* node);
    void addNode(Node* node, Course course);
    void inOrder(Node* node);

public:
    int size;
    BinarySearchTree();
    ~BinarySearchTree();
    void Insert(Course course);
    void InOrder();
    Course Search(string courseId);
};

BinarySearchTree::BinarySearchTree() : root(nullptr), size(0) {}

BinarySearchTree::~BinarySearchTree() {
    DestroyBST(root);
}

void BinarySearchTree::DestroyBST(Node* node) {
    if (node != nullptr) {
        DestroyBST(node->left);
        DestroyBST(node->right);
        delete node;
    }
}

void BinarySearchTree::Insert(Course course) {
    if (root == nullptr) {
        root = new Node(course);
    } else {
        addNode(root, course);
    }
    size++;
}

void BinarySearchTree::addNode(Node* node, Course course) {
    Node* curr = node;
    while (curr != nullptr) {
        if (course.courseId < curr->course.courseId) {
            if (curr->left == nullptr) {
                curr->left = new Node(course);
                return;
            } else {
                curr = curr->left;
            }
        } else {
            if (curr->right == nullptr) {
                curr->right = new Node(course);
                return;
            } else {
                curr = curr->right;
            }
        }
    }
}

void BinarySearchTree::InOrder() {
    inOrder(root);
}

void BinarySearchTree::inOrder(Node* node) {
    if (node != nullptr) {
        inOrder(node->left);
        cout << node->course.courseId << ": " << node->course.title << " | Prerequisites: ";
        if (node->course.prereq.empty()) {
            cout << "None";
        } else {
            cout << node->course.prereq[0];
            for (size_t i = 1; i < node->course.prereq.size(); i++) {
                cout << ", " << node->course.prereq[i];
            }
        }
        cout << endl;
        inOrder(node->right);
    }
}

Course BinarySearchTree::Search(string courseId) {
    Node* curr = root;
    while (curr != nullptr) {
        if (curr->course.courseId == courseId) return curr->course;
        else if (courseId < curr->course.courseId) curr = curr->left;
        else curr = curr->right;
    }
    return Course(); // empty Course
}

void parseFile(const string& fileName, BinarySearchTree* bst) {
    ifstream inputFile(fileName);
    if (!inputFile.is_open()) {
        cout << "ERROR: File could not be opened." << endl;
        return;
    }
    string line;
    while (getline(inputFile, line)) {
        Course course;
        stringstream ss(line);
        string token;
        getline(ss, course.courseId, ',');
        getline(ss, course.title, ',');
        if (course.courseId.empty() || course.title.empty()) {
            cout << "Invalid line format, skipping." << endl;
            continue;
        }
        while (getline(ss, token, ',')) {
            course.prereq.push_back(token);
        }
        bst->Insert(course);
    }
    inputFile.close();
}

void displayCourse(const Course& course) {
    cout << course.courseId << ", " << course.title << endl;
    cout << "Prerequisites: ";
    if (course.prereq.empty()) {
        cout << "None";
    } else {
        cout << course.prereq[0];
        for (size_t i = 1; i < course.prereq.size(); i++) {
            cout << ", " << course.prereq[i];
        }
    }
    cout << endl;
}

void displayMenu(BinarySearchTree* bst) {
    string csvPath = "courses.csv";
    int choice = 0;
    while (choice != 9) {
        cout << "\nMenu:\n";
        cout << "  1. Load Courses\n";
        cout << "  2. Print Schedule\n";
        cout << "  3. Print Course\n";
        cout << "  9. Exit\n";
        cout << "Enter selection: ";
        if (!(cin >> choice)) {
            cout << "Invalid input. Try again." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        switch (choice) {
        case 1:
            parseFile(csvPath, bst);
            cout << bst->size << " courses loaded." << endl;
            break;
        case 2:
            bst->InOrder();
            break;
        case 3: {
            string id;
            cout << "Enter course ID: ";
            cin >> id;
            transform(id.begin(), id.end(), id.begin(), ::toupper);
            Course c = bst->Search(id);
            if (!c.courseId.empty()) displayCourse(c);
            else cout << "Course not found." << endl;
            break;
        }
        case 9:
            cout << "Exiting program." << endl;
            break;
        default:
            cout << "Invalid option. Try again." << endl;
        }
    }
}

int main() {
    BinarySearchTree bst;
    displayMenu(&bst);
    return 0;
}
