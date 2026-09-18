# 🏦 Bank Management System

A console-based **Bank Management System** developed in C++ as a university programming project.

The project demonstrates core C++ programming concepts through a simple banking application that allows users to manage accounts, perform transactions, and store account information using a file-based data system.

## 📌 Features

* Create and manage bank accounts
* Deposit money
* Withdraw money
* Check account balance
* Search for account information
* Store and retrieve account data
* File-based data persistence
* Console-based user interface
* Input validation and basic error handling

## 🛠️ Technologies Used

* **C++**
* **Object-Oriented Programming**
* **File Handling**
* **Console-based Interface**

## 📂 Project Structure

```text
Bank-Management-System/
│
├── project.cpp
├── project_by_me.cpp
├── bank_data.txt
├── .gitignore
└── README.md
```

### File Description

| File                     | Description                                    |
| ------------------------ | ---------------------------------------------- |
| `project.cpp`            | Main project implementation                    |
| `project_by_me.cpp`      | Personal implementation/version of the project |
| `bank_data.txt`          | File used for storing bank/account data        |
| `.gitignore`             | Specifies files that Git should ignore         |

## ⚙️ Requirements

To compile and run the project, you need:

* A C++ compiler such as **G++ / MinGW**
* VS Code or another C++-compatible IDE/editor
* Windows, Linux, or another platform with a compatible C++ compiler

## 🚀 How to Run

### 1. Clone the repository

```bash
git clone https://github.com/Doodlehood/Bank-Management-System.git
```

### 2. Open the project

Open the cloned folder in VS Code.

### 3. Compile the program

For example, using G++:

```bash
g++ project.cpp -o project
```

### 4. Run the program

On Windows:

```bash
.\project.exe
```

On Linux/macOS:

```bash
./project
```

> **Note:** The exact compilation command may vary depending on which `.cpp` implementation you want to run.

## 💾 Data Storage

The project uses `bank_data.txt` for file-based storage.

This allows account information to persist after the program is closed and reopened.

**Important:** Do not store real financial information, passwords, or sensitive personal data in this project or its repository.

## 🎯 Learning Objectives

This project was created to practice and demonstrate:

* C++ fundamentals
* Classes and objects
* Functions
* Conditional statements and loops
* File handling
* Data management
* Object-oriented programming concepts
* Basic software/project organization

## 🔮 Possible Future Improvements

Some features that could be added in future versions include:

* Account deletion
* Account modification
* Transaction history
* Improved input validation
* Password/PIN authentication
* Better user interface
* Separate administrator and customer functionality
* Database integration instead of text-file storage
* More robust error handling

## 👨‍💻 Author

**Doodlehood**

GitHub: https://github.com/Doodlehood

## 📄 License

This project was created for educational purposes.
