#include "./include/LinkedList.hpp"
#include "./include/MemoryResource.hpp"

#include <iostream>
#include <memory_resource>
#include <string>
#include <iomanip>
#include <cmath>
#include <sstream>

void demonstrateSimpleTypes() {
    std::cout << "\n========== ДЕМОНСТРАЦИЯ ПРОСТЫХ ТИПОВ ==========\n" << std::endl;
    
    MemoryResource mres;
    
    // --------- int ---------
    std::cout << "[1] LinkedList<int>\n";
    std::pmr::polymorphic_allocator<ListItem<int>> allocInt(&mres);
    LinkedList<int, std::pmr::polymorphic_allocator<ListItem<int>>> intList({10, 20, 30, 40, 50}, allocInt);
    
    std::cout << "    Инициализация списка: { 10, 20, 30, 40, 50 }\n";
    std::cout << "    Размер: " << intList.getSize() << "\n";
    std::cout << "    Элементы: ";
    for (size_t i = 0; i < intList.getSize(); ++i) {
        std::cout << intList[i].value << " ";
    }
    std::cout << "\n";
    
    int x = 15;
    intList.pushFront(x);
    std::cout << "    После pushFront(15): ";
    for (size_t i = 0; i < intList.getSize(); ++i) {
        std::cout << intList[i].value << " ";
    }
    std::cout << "\n";
    
    int y = 60;
    intList.pushBack(y);
    std::cout << "    После pushBack(60): ";
    for (size_t i = 0; i < intList.getSize(); ++i) {
        std::cout << intList[i].value << " ";
    }
    std::cout << "\n";
    
    int popped = intList.popFront();
    std::cout << "    Удален с начала: " << popped << ", остаток: ";
    for (size_t i = 0; i < intList.getSize(); ++i) {
        std::cout << intList[i].value << " ";
    }
    std::cout << "\n" << std::endl;
    
    std::cout << "[2] LinkedList<double>\n";
    std::pmr::polymorphic_allocator<ListItem<double>> allocDouble(&mres);
    LinkedList<double, std::pmr::polymorphic_allocator<ListItem<double>>> doubleList({1.5, 2.7, 3.14, 4.2}, allocDouble);
    
    std::cout << "    Инициализация списка: { 1.5, 2.7, 3.14, 4.2 }\n";
    std::cout << "    Размер: " << doubleList.getSize() << "\n";
    std::cout << "    Элементы: ";
    for (size_t i = 0; i < doubleList.getSize(); ++i) {
        std::cout << std::fixed << std::setprecision(2) << doubleList[i].value << " ";
    }
    std::cout << "\n";
    
    double d1 = 0.99;
    doubleList.pushFront(d1);
    std::cout << "    После pushFront(0.99): ";
    for (size_t i = 0; i < doubleList.getSize(); ++i) {
        std::cout << std::fixed << std::setprecision(2) << doubleList[i].value << " ";
    }
    std::cout << "\n" << std::endl;
    
    // --------- std::string ---------
    std::cout << "[3] LinkedList<std::string>\n";
    std::pmr::polymorphic_allocator<ListItem<std::string>> allocString(&mres);
    LinkedList<std::string, std::pmr::polymorphic_allocator<ListItem<std::string>>> stringList(
        {"Hello", "World", "LinkedList", "C++"}, allocString
    );
    
    std::cout << "    Инициализация списка: { \"Hello\", \"World\", \"LinkedList\", \"C++\" }\n";
    std::cout << "    Размер: " << stringList.getSize() << "\n";
    std::cout << "    Элементы: ";
    for (size_t i = 0; i < stringList.getSize(); ++i) {
        std::cout << "\"" << stringList[i].value << "\" ";
    }
    std::cout << "\n";
    
    std::string s = "OOP";
    stringList.pushBack(s);
    std::cout << "    После pushBack(\"OOP\"): ";
    for (size_t i = 0; i < stringList.getSize(); ++i) {
        std::cout << "\"" << stringList[i].value << "\" ";
    }
    std::cout << "\n" << std::endl;
}

// ============== Сложные типы ==============

// Структура 1: Студент
struct Student {
    std::string name;
    int age;
    double gpa;
    
    // Для вывода
    std::string toString() const {
        std::stringstream ss;
        ss << "{ \n\t\tname=\"" << name << "\",\n\t\tage=" << age << ",\n\t\tgpa=" << std::fixed << std::setprecision(2) << gpa << " \n\t}";
        return ss.str();
    }
};

// Структура 2: Точка в 3D пространстве
struct Point3D {
    double x, y, z;
    
    double distance() const {
        return std::sqrt(x*x + y*y + z*z);
    }
    
    std::string toString() const {
        std::stringstream ss;
        ss << "{ \n\t\tx=" << std::fixed << std::setprecision(2) << x 
           << ",\n\t\ty=" << y << ",\n\t\tz=" << z 
           << ",\n\t\tdistance_from_origin=" << distance() << "\n\t}";
        return ss.str();
    }
};

// Структура 3: Книга
struct Book {
    std::string title;
    std::string author;
    int year;
    int pages;
    
    std::string toString() const {
        std::stringstream ss;
        ss << "{\n\t\ttitle=\"" << title << "\",\n\t\tauthor=\"" << author 
           << "\",\n\t\tyear=" << year << ",\n\t\tpages=" << pages << " \n\t}";
        return ss.str();
    }
};

void demonstrateComplexTypes() {
    std::cout << "\n========== ДЕМОНСТРАЦИЯ СЛОЖНЫХ ТИПОВ ==========\n" << std::endl;
    
    MemoryResource mres;
    
    // --------- Student ---------
    std::cout << "[1] LinkedList<Student>\n";
    std::pmr::polymorphic_allocator<ListItem<Student>> allocStudent(&mres);
    LinkedList<Student, std::pmr::polymorphic_allocator<ListItem<Student>>> studentList(allocStudent);
    
    Student s1{"Alice", 20, 3.8};
    Student s2{"Bob", 21, 3.5};
    Student s3{"Charlie", 19, 3.9};
    
    studentList.pushBack(s1);
    studentList.pushBack(s2);
    studentList.pushBack(s3);
    
    std::cout << "    Добавлено 3 студента\n";
    std::cout << "    Размер: " << studentList.getSize() << "\n";
    std::cout << "    Элементы:\n";
    for (size_t i = 0; i < studentList.getSize(); ++i) {
        std::cout << "      [" << i << "] " << studentList[i].value.toString() << "\n";
    }
    
    Student newStudent{"David", 20, 3.7};
    studentList.pushFront(newStudent);
    std::cout << "\n    После pushFront(David, 20, 3.7):\n";
    for (size_t i = 0; i < studentList.getSize(); ++i) {
        std::cout << "      [" << i << "] " << studentList[i].value.toString() << "\n";
    }
    
    Student poppedStudent = studentList.popBack();
    std::cout << "\n    Удален с конца: " << poppedStudent.toString() << "\n\n";
    
    std::cout << "[2] LinkedList<Point3D>\n";
    std::pmr::polymorphic_allocator<ListItem<Point3D>> allocPoint(&mres);
    LinkedList<Point3D, std::pmr::polymorphic_allocator<ListItem<Point3D>>> pointList(allocPoint);
    
    Point3D p1{3.0, 4.0, 0.0};
    Point3D p2{1.0, 1.0, 1.0};
    Point3D p3{5.0, 12.0, 0.0};
    
    pointList.pushBack(p1);
    pointList.pushBack(p2);
    pointList.pushBack(p3);
    
    std::cout << "    Добавлено 3 точки в 3D пространстве\n";
    std::cout << "    Размер: " << pointList.getSize() << "\n";
    std::cout << "    Элементы:\n";
    for (size_t i = 0; i < pointList.getSize(); ++i) {
        std::cout << "      [" << i << "] " << pointList[i].value.toString() << "\n";
    }
    
    Point3D p4{2.0, 3.0, 6.0};
    pointList.pushFront(p4);
    std::cout << "\n    После pushFront({2.0, 3.0, 6.0}):\n";
    for (size_t i = 0; i < pointList.getSize(); ++i) {
        std::cout << "      [" << i << "] " << pointList[i].value.toString() << "\n";
    }
    std::cout << std::endl;
    
    // --------- Book ---------
    std::cout << "[3] LinkedList<Book>\n";
    std::pmr::polymorphic_allocator<ListItem<Book>> allocBook(&mres);
    LinkedList<Book, std::pmr::polymorphic_allocator<ListItem<Book>>> bookList(allocBook);
    
    Book b1{"C++ Primer", "Stanley Lippman", 2012, 1077};
    Book b2{"The C++ Programming Language", "Bjarne Stroustrup", 2013, 1366};
    Book b3{"Effective C++", "Scott Meyers", 2005, 320};
    
    bookList.pushBack(b1);
    bookList.pushBack(b2);
    bookList.pushBack(b3);
    
    std::cout << "    Добавлено 3 книги\n";
    std::cout << "    Размер: " << bookList.getSize() << "\n";
    std::cout << "    Элементы:\n";
    for (size_t i = 0; i < bookList.getSize(); ++i) {
        std::cout << "      [" << i << "] " << bookList[i].value.toString() << "\n";
    }
    
    Book newBook{"Modern C++", "Herb Sutter", 2018, 752};
    bookList.pushFront(newBook);
    std::cout << "\n    После pushFront(Modern C++, Herb Sutter, 2018, 752):\n";
    for (size_t i = 0; i < bookList.getSize(); ++i) {
        std::cout << "      [" << i << "] " << bookList[i].value.toString() << "\n";
    }
    
    Book poppedBook = bookList.popFront();
    std::cout << "\n    Удален с начала: " << poppedBook.toString() << "\n";
    std::cout << "\n    Результирующий список:\n";
    for (size_t i = 0; i < bookList.getSize(); ++i) {
        std::cout << "      [" << i << "] " << bookList[i].value.toString() << "\n";
    }
    std::cout << std::endl;
}

int main() {
    demonstrateSimpleTypes();
    demonstrateComplexTypes();

    return 0;
}