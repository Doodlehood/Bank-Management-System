#include <iostream>
#include <string>
#include <vector>
#include <limits>
using namespace std;

class Account {
protected:
    string accountNumber;
    double balance;
    
public:
    Account(string accNo, double bal) : accountNumber(accNo), balance(bal) {}
    
    string getAccountNumber() { return accountNumber; }
    double getBalance() { return balance; }
    
    virtual void deposit(double amount) {
        if (amount > 0) {
            balance += amount;
            cout << "Deposit successful. New balance: " << balance << endl;
        } else {
            cout << "Invalid amount!" << endl;
        }
    }

    virtual bool withdraw(double amount) {
        if (amount > 0 && amount <= balance) {
            balance -= amount;
            cout << "Withdrawal successful. New balance: " << balance << endl;
            return true;
        } else {
            cout << "Invalid amount or insufficient balance!" << endl;
            return false;
        }
    }
    
    virtual void display() {
        cout << "Account: " << accountNumber << ", Balance: " << balance << endl;
    }
};

class SavingsAccount : public Account {
private:
    double interestRate;
    
public:
    SavingsAccount(string accNo, double bal) : Account(accNo, bal), interestRate(0.05) {}
    
    void addInterest() {
        balance += balance * interestRate;
        cout << "Interest added. New balance: " << balance << endl;
    }
    
    void display() override {
        cout << "Savings Account: " << accountNumber << ", Balance: " << balance 
             << ", Interest Rate: " << interestRate * 100 << "%" << endl;
    }
};

class CurrentAccount : public Account {
private:
    double overdraftLimit;
    
public:
    CurrentAccount(string accNo, double bal) : Account(accNo, bal), overdraftLimit(1000) {}
    
    bool withdraw(double amount) override {
        if (amount > 0 && amount <= (balance + overdraftLimit)) {
            balance -= amount;
            cout << "Withdrawal successful. New balance: " << balance << endl;
            return true;
        } else {
            cout << "Invalid amount or overdraft limit exceeded!" << endl;
            return false;
        }
    }
    
    void display() override {
        cout << "Current Account: " << accountNumber << ", Balance: " << balance 
             << ", Overdraft Limit: " << overdraftLimit << endl;
    }
};

class Customer {
private:
    string name;
    string customerID;
    vector<Account*> accounts;
    
public:
    Customer(string id, string n) : customerID(id), name(n) {}
    
    string getCustomerID() { return customerID; }
    string getName() { return name; }
    
    void addAccount(Account* acc) {
        accounts.push_back(acc);
    }
    
    Account* findAccount(string accNo) {
        for (Account* acc : accounts) {
            if (acc->getAccountNumber() == accNo) {
                return acc;
            }
        }
        return nullptr;
    }
    
    void displayCustomer() {
        cout << "Customer ID: " << customerID << ", Name: " << name << endl;
        cout << "Accounts:" << endl;
        for (Account* acc : accounts) {
            acc->display();
        }
    }
};

class Bank {
private:
    vector<Customer*> customers;
    int nextCustomerID;
    int nextAccountID;
    
public:
    Bank() : nextCustomerID(1001), nextAccountID(5001) {}
    
    string validateInput(string prompt) {
        string input;
        do {
            cout << prompt;
            getline(cin, input);
            if (input.empty()) {
                cout << "Input cannot be empty!" << endl;
            }
        } while (input.empty());
        return input;
    }
    
    double validateAmount() {
        double amount;
        while (true) {
            cout << "Enter amount: ";
            cin >> amount;
            if (cin.fail() || amount <= 0) {
                cout << "Invalid amount!" << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            } else {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return amount;
            }
        }
    }
    
    Customer* findCustomer(string custID) {
        for (Customer* cust : customers) {
            if (cust->getCustomerID() == custID) {
                return cust;
            }
        }
        return nullptr;
    }
    
    void createCustomer() {
        string name = validateInput("Enter customer name: ");
        string custID = "CUST" + to_string(nextCustomerID++);
        
        Customer* newCustomer = new Customer(custID, name);
        customers.push_back(newCustomer);
        
        cout << "Customer created with ID: " << custID << endl;
    }
    
    void createAccount() {
        string custID = validateInput("Enter customer ID: ");
        Customer* customer = findCustomer(custID);
        
        if (!customer) {
            cout << "Customer not found!" << endl;
            return;
        }
        
        cout << "Account Type: 1-Savings, 2-Current: ";
        int type;
        cin >> type;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        double initialBalance = validateAmount();
        string accNo = "ACC" + to_string(nextAccountID++);
        
        Account* newAccount;
        if (type == 1) {
            newAccount = new SavingsAccount(accNo, initialBalance);
        } else if (type == 2) {
            newAccount = new CurrentAccount(accNo, initialBalance);
        } else {
            cout << "Invalid account type!" << endl;
            return;
        }
        
        customer->addAccount(newAccount);
        cout << "Account created with number: " << accNo << endl;
    }
    
    void depositMoney() {
        string custID = validateInput("Enter customer ID: ");
        string accNo = validateInput("Enter account number: ");
        
        Customer* customer = findCustomer(custID);
        if (!customer) {
            cout << "Customer not found!" << endl;
            return;
        }
        
        Account* account = customer->findAccount(accNo);
        if (!account) {
            cout << "Account not found!" << endl;
            return;
        }
        
        double amount = validateAmount();
        account->deposit(amount);
    }
    
    void withdrawMoney() {
        string custID = validateInput("Enter customer ID: ");
        string accNo = validateInput("Enter account number: ");
        
        Customer* customer = findCustomer(custID);
        if (!customer) {
            cout << "Customer not found!" << endl;
            return;
        }
        
        Account* account = customer->findAccount(accNo);
        if (!account) {
            cout << "Account not found!" << endl;
            return;
        }
        
        double amount = validateAmount();
        account->withdraw(amount);
    }
    
    void displayCustomer() {
        string custID = validateInput("Enter customer ID: ");
        Customer* customer = findCustomer(custID);
        
        if (customer) {
            customer->displayCustomer();
        } else {
            cout << "Customer not found!" << endl;
        }
    }
    
    void addInterest() {
        string custID = validateInput("Enter customer ID: ");
        string accNo = validateInput("Enter account number: ");
        
        Customer* customer = findCustomer(custID);
        if (!customer) {
            cout << "Customer not found!" << endl;
            return;
        }
        
        Account* account = customer->findAccount(accNo);
        SavingsAccount* savingsAcc = dynamic_cast<SavingsAccount*>(account);
        
        if (savingsAcc) {
            savingsAcc->addInterest();
        } else {
            cout << "Not a savings account!" << endl;
        }
    }
    
    void menu() {
        int choice;
        
        while (true) {
            cout << "\n--- Bank Management System ---" << endl;
            cout << "1. Create Customer" << endl;
            cout << "2. Create Account" << endl;
            cout << "3. Deposit Money" << endl;
            cout << "4. Withdraw Money" << endl;
            cout << "5. Display Customer" << endl;
            cout << "6. Add Interest (Savings)" << endl;
            cout << "7. Exit" << endl;
            cout << "Enter choice: ";
            
            cin >> choice;
            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid choice! Please enter a number between 1-7." << endl;
                continue;
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            
            switch (choice) {
                case 1: createCustomer(); break;
                case 2: createAccount(); break;
                case 3: depositMoney(); break;
                case 4: withdrawMoney(); break;
                case 5: displayCustomer(); break;
                case 6: addInterest(); break;
                case 7: 
                    cout << "Thank you!" << endl;
                    return;
                default:
                    cout << "Invalid choice! Please enter a number between 1-7." << endl;
            }
        }
    }
};

int main() {
    Bank bank;
    bank.menu();
    return 0;
}