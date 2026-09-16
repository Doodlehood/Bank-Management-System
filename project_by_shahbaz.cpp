#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <limits>

using namespace std;

string currentDateTime() {
    time_t now = time(0);
    char buf[80];
    struct tm tstruct;
#ifdef _WIN32
    localtime_s(&tstruct, &now);
#else
    tstruct = *localtime(&now);
#endif
    strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
    return string(buf);
}

// Trim whitespace from both ends of a string
string trim(const string& str) {
    size_t first = str.find_first_not_of(' ');
    if (first == string::npos)
        return "";
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

// Read a line and validate it is non-empty
string readNonEmptyLine(const string& prompt) {
    string input;
    while (true) {
        cout << prompt;
        getline(cin, input);
        input = trim(input);
        if (input.empty()) {
            cout << "Input cannot be empty. Please try again.\n";
        } else {
            return input;
        }
    }
}

// Read and validate integer input within a range
int readIntInRange(const string& prompt, int minVal, int maxVal) {
    int value;
    while (true) {
        cout << prompt;
        cin >> value;
        if (cin.fail() || value < minVal || value > maxVal) {
            cout << "Invalid input. Please enter a number between " << minVal << " and " << maxVal << ".\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clear buffer
            return value;
        }
    }
}

// Read and validate double input greater than or equal to minVal
double readDoubleMin(const string& prompt, double minVal) {
    double value;
    while (true) {
        cout << prompt;
        cin >> value;
        if (cin.fail() || value < minVal) {
            cout << "Invalid input. Please enter a number greater than or equal to " << minVal << ".\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clear buffer
            return value;
        }
    }
}

// Read and validate account type
string readAccountType() {
    string type;
    while (true) {
        cout << "Enter Account Type (Savings / Current): ";
        getline(cin, type);
        type = trim(type);
        for (auto & c : type) c = tolower(c); // convert to lowercase
        if (type == "savings" || type == "current") {
            // Capitalize first letter
            type[0] = toupper(type[0]);
            return type;
        } else {
            cout << "Invalid account type. Please enter 'Savings' or 'Current'.\n";
        }
    }
}

// Transaction class
class Transaction {
public:
    string dateTime;
    string type; // Deposit, Withdraw, Transfer
    double amount;
    string details; // For transfer: to/from account

    Transaction() {}
    Transaction(string t, double amt, string det) {
        dateTime = currentDateTime();
        type = t;
        amount = amt;
        details = det;
    }

    void showTransaction() const {
        cout << setw(20) << dateTime << " | "
             << setw(10) << type << " | "
             << setw(10) << fixed << setprecision(2) << amount << " | "
             << details << endl;
    }
};

// Account class
class Account {
private:
    string accountNumber;
    string accountType; // Savings, Current
    double balance;
    vector<Transaction> transactions;

public:
    Account() {}
    Account(string accNo, string accType, double initialDeposit) {
        accountNumber = accNo;
        accountType = accType;
        balance = initialDeposit;
        transactions.push_back(Transaction("Deposit", initialDeposit, "Initial Deposit"));
    }

    string getAccountNumber() const { return accountNumber; }
    string getAccountType() const { return accountType; }
    double getBalance() const { return balance; }

    void deposit(double amount) {
        if (amount <= 0) {
            cout << "Invalid deposit amount.\n";
            return;
        }
        balance += amount;
        transactions.push_back(Transaction("Deposit", amount, "Amount deposited"));
        cout << "Deposit successful.\n";
    }

    bool withdraw(double amount) {
        if (amount <= 0) {
            cout << "Invalid withdrawal amount.\n";
            return false;
        }
        if (amount > balance) {
            cout << "Insufficient balance.\n";
            return false;
        }
        balance -= amount;
        transactions.push_back(Transaction("Withdraw", amount, "Amount withdrawn"));
        cout << "Withdrawal successful.\n";
        return true;
    }

    bool transfer(Account &toAccount, double amount) {
        if (amount <= 0) {
            cout << "Invalid transfer amount.\n";
            return false;
        }
        if (amount > balance) {
            cout << "Insufficient balance.\n";
            return false;
        }
        balance -= amount;
        toAccount.balance += amount;

        transactions.push_back(Transaction("Transfer", amount, "Transferred to acc " + toAccount.getAccountNumber()));
        toAccount.transactions.push_back(Transaction("Transfer", amount, "Received from acc " + accountNumber));
        cout << "Transfer successful.\n";
        return true;
    }

    void showAccountDetails() const {
        cout << "Account Number: " << accountNumber << endl;
        cout << "Account Type  : " << accountType << endl;
        cout << "Balance       : " << fixed << setprecision(2) << balance << endl;
    }

    void showTransactionHistory() const {
        cout << "Transaction History for Account " << accountNumber << ":\n";
        cout << setw(20) << "Date & Time" << " | "
             << setw(10) << "Type" << " | "
             << setw(10) << "Amount" << " | Details" << endl;
        cout << string(70, '-') << endl;
        for (const Transaction &t : transactions) {
            t.showTransaction();
        }
        cout << string(70, '-') << endl;
    }

    void saveToFile(ofstream &outFile) const {
        outFile << accountNumber << '\n' << accountType << '\n' << balance << '\n' << transactions.size() << '\n';
        for (const Transaction &t : transactions) {
            outFile << t.dateTime << '\n' << t.type << '\n' << t.amount << '\n' << t.details << '\n';
        }
    }

    void loadFromFile(ifstream &inFile) {
        size_t tCount;
        getline(inFile, accountNumber);
        getline(inFile, accountType);
        inFile >> balance;
        inFile >> tCount;
        inFile.ignore();
        transactions.clear();
        for (size_t i = 0; i < tCount; i++) {
            Transaction t;
            getline(inFile, t.dateTime);
            getline(inFile, t.type);
            inFile >> t.amount;
            inFile.ignore();
            getline(inFile, t.details);
            transactions.push_back(t);
        }
    }
};

// Customer class
class Customer {
private:
    string name;
    string address;
    string contact;
    string customerID;
    vector<Account> accounts;

public:
    Customer() {}
    Customer(string id, string n, string addr, string cont) : customerID(id), name(n), address(addr), contact(cont) {}

    string getCustomerID() const { return customerID; }
    string getName() const { return name; }

    void addAccount(const Account& acc) { accounts.push_back(acc); }
    vector<Account>& getAccounts() { return accounts; }
    const vector<Account>& getAccounts() const { return accounts; }

    void showCustomerDetails() const {
        cout << "Customer ID : " << customerID << endl;
        cout << "Name        : " << name << endl;
        cout << "Address     : " << address << endl;
        cout << "Contact     : " << contact << endl;
        cout << "Accounts    : " << accounts.size() << endl;
        for (const auto &acc : accounts) {
            cout << " - " << acc.getAccountNumber() << " (" << acc.getAccountType() << ") Balance: " << fixed << setprecision(2) << acc.getBalance() << endl;
        }
    }

    void saveToFile(ofstream &outFile) const {
        outFile << customerID << '\n' << name << '\n' << address << '\n' << contact << '\n' << accounts.size() << '\n';
        for (const Account &acc : accounts) {
            acc.saveToFile(outFile);
        }
    }

    void loadFromFile(ifstream &inFile) {
        size_t accCount;
        getline(inFile, customerID);
        getline(inFile, name);
        getline(inFile, address);
        getline(inFile, contact);
        inFile >> accCount;
        inFile.ignore();
        accounts.clear();
        for (size_t i = 0; i < accCount; i++) {
            Account acc;
            acc.loadFromFile(inFile);
            accounts.push_back(acc);
        }
    }
};

// Bank class
class Bank {
private:
    vector<Customer> customers;

    string generateCustomerID() {
        return "CUST" + to_string(1000 + (int)customers.size() + 1);
    }

    string generateAccountNumber() {
        int count = 0;
        for (const auto &cust : customers) {
            count += (int)cust.getAccounts().size();
        }
        return "ACC" + to_string(5000 + count + 1);
    }

public:
    Bank() {}

    void loadData() {
        ifstream inFile("bank_data.txt");
        if (!inFile) return;

        size_t custCount;
        inFile >> custCount;
        inFile.ignore();
        customers.clear();
        for (size_t i = 0; i < custCount; i++) {
            Customer cust;
            cust.loadFromFile(inFile);
            customers.push_back(cust);
        }
        inFile.close();
    }

    void saveData() {
        ofstream outFile("bank_data.txt");
        if (!outFile) {
            cout << "Error opening file for writing.\n";
            return;
        }
        outFile << customers.size() << '\n';
        for (const Customer &cust : customers) {
            cust.saveToFile(outFile);
        }
        outFile.close();
    }

    void createCustomer() {
        string name = readNonEmptyLine("Enter Customer Name: ");
        string address = readNonEmptyLine("Enter Address: ");
        string contact = readNonEmptyLine("Enter Contact Number: ");

        string custID = generateCustomerID();
        Customer newCustomer(custID, name, address, contact);
        customers.push_back(newCustomer);
        cout << "Customer created successfully with ID: " << custID << endl;
    }

    Customer* findCustomerByID(const string &custID) {
        for (auto &cust : customers) {
            if (cust.getCustomerID() == custID)
                return &cust;
        }
        return nullptr;
    }

    Account* findAccountByNumber(const string &accNo) {
        for (auto &cust : customers) {
            for (auto &acc : cust.getAccounts()) {
                if (acc.getAccountNumber() == accNo)
                    return &acc;
            }
        }
        return nullptr;
    }

    void createAccount() {
        string custID = readNonEmptyLine("Enter Customer ID to create account for: ");
        Customer* cust = findCustomerByID(custID);
        if (!cust) {
            cout << "Customer not found.\n";
            return;
        }
        string accType = readAccountType();
        double initialDeposit = readDoubleMin("Enter Initial Deposit Amount: ", 0.0);
        string accNo = generateAccountNumber();
        Account newAcc(accNo, accType, initialDeposit);
        cust->addAccount(newAcc);
        cout << "Account created successfully with Account Number: " << accNo << endl;
    }

    void depositMoney() {
        string accNo = readNonEmptyLine("Enter Account Number: ");
        Account* acc = findAccountByNumber(accNo);
        if (!acc) {
            cout << "Account not found.\n";
            return;
        }
        double amount = readDoubleMin("Enter amount to deposit: ", 0.01);
        acc->deposit(amount);
    }

    void withdrawMoney() {
        string accNo = readNonEmptyLine("Enter Account Number: ");
        Account* acc = findAccountByNumber(accNo);
        if (!acc) {
            cout << "Account not found.\n";
            return;
        }
        double amount = readDoubleMin("Enter amount to withdraw: ", 0.01);
        acc->withdraw(amount);
    }

    void transferMoney() {
        string fromAccNo = readNonEmptyLine("Enter Source Account Number: ");
        string toAccNo = readNonEmptyLine("Enter Destination Account Number: ");

        Account* fromAcc = findAccountByNumber(fromAccNo);
        Account* toAcc = findAccountByNumber(toAccNo);

        if (!fromAcc) {
            cout << "Source account not found.\n";
            return;
        }
        if (!toAcc) {
            cout << "Destination account not found.\n";
            return;
        }
        double amount = readDoubleMin("Enter amount to transfer: ", 0.01);
        fromAcc->transfer(*toAcc, amount);
    }

    void showCustomerDetails() {
        string custID = readNonEmptyLine("Enter Customer ID: ");
        Customer* cust = findCustomerByID(custID);
        if (!cust) {
            cout << "Customer not found.\n";
            return;
        }
        cust->showCustomerDetails();
    }

    void showAccountDetails() {
        string accNo = readNonEmptyLine("Enter Account Number: ");
        Account* acc = findAccountByNumber(accNo);
        if (!acc) {
            cout << "Account not found.\n";
            return;
        }
        acc->showAccountDetails();
    }

    void showTransactionHistory() {
        string accNo = readNonEmptyLine("Enter Account Number: ");
        Account* acc = findAccountByNumber(accNo);
        if (!acc) {
            cout << "Account not found.\n";
            return;
        }
        acc->showTransactionHistory();
    }

    void menu() {
        while (true) {
            cout << "\n====== Bank Management System ======\n";
            cout << "1. Create Customer\n";
            cout << "2. Create Account\n";
            cout << "3. Deposit Money\n";
            cout << "4. Withdraw Money\n";
            cout << "5. Transfer Money\n";
            cout << "6. Show Customer Details\n";
            cout << "7. Show Account Details\n";
            cout << "8. Show Transaction History\n";
            cout << "9. Exit\n";

            int choice = readIntInRange("Enter choice: ", 1, 9);

            switch (choice) {
                case 1: createCustomer(); break;
                case 2: createAccount(); break;
                case 3: depositMoney(); break;
                case 4: withdrawMoney(); break;
                case 5: transferMoney(); break;
                case 6: showCustomerDetails(); break;
                case 7: showAccountDetails(); break;
                case 8: showTransactionHistory(); break;
                case 9: 
                    saveData();
                    cout << "Thank you for using the Bank Management System. Goodbye!\n";
                    return;
                default:
                    cout << "Invalid choice. Please try again.\n";
            }
        }
    }
};

int main() {
    Bank bank;
    bank.loadData();
    bank.menu();
    return 0;
}
