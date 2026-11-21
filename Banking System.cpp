
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>
using namespace std;

class Customer
{
public:
    int customerID;
    string name;

    Customer(int id, string n)
    {
        customerID = id;
        name = n;
    }

    void display()
    {
        cout << "\nCustomer ID: " << customerID;
        cout << "\nCustomer Name: " << name;
    }
};

class Transaction
{
public:
    string type;
    double amount;

    Transaction(string t, double a)
    {
        type = t;
        amount = a;
    }

    void display()
    {
        cout << "\n" << type << ": " << amount;
    }
};

class Account
{
public:
    int accountNumber;
    int customerID;
    double balance;

    vector<Transaction> history;

    Account(int acc, int cust, double initialBalance)
    {
        accountNumber = acc;
        customerID = cust;
        balance = initialBalance;
    }

    void deposit(double amount)
    {
        if (amount <= 0)
        {
            cout << "Invalid amount!\n";
            return;
        }

        balance += amount;

        history.push_back(Transaction("Deposit", amount));

        saveTransaction("Deposit", amount);

        cout << "Deposit successful!\n";
    }

    bool withdraw(double amount)
    {
        if (amount <= 0)
        {
            cout << "Invalid amount!\n";
            return false;
        }

        if (amount > balance)
        {
            cout << "Insufficient balance!\n";
            return false;
        }

        balance -= amount;

        history.push_back(Transaction("Withdrawal", amount));

        saveTransaction("Withdrawal", amount);

        cout << "Withdrawal successful!\n";

        return true;
    }

    void saveTransaction(string type, double amount)
    {
        ofstream file("transactions.txt", ios::app);

        file << "Account: " << accountNumber
             << " | " << type
             << " | Amount: " << amount
             << " | Balance: " << balance << endl;

        file.close();
    }

    void displayAccount()
    {
        cout << "\nAccount Number: " << accountNumber;
        cout << "\nCustomer ID: " << customerID;
        cout << "\nCurrent Balance: " << balance << endl;
    }

    void displayHistory()
    {
        cout << "\n===== TRANSACTION HISTORY =====\n";

        if (history.empty())
        {
            cout << "No transactions available.\n";
            return;
        }

        for (int i = 0; i < history.size(); i++)
        {
            cout << i + 1 << ". ";
            history[i].display();
            cout << endl;
        }
    }
};

int main()
{
    vector<Customer> customers;
    vector<Account> accounts;

    int choice;

    do
    {
        cout << "\n\n===== BANKING SYSTEM =====\n";

        cout << "1. Create Customer and Account\n";
        cout << "2. Deposit Money\n";
        cout << "3. Withdraw Money\n";
        cout << "4. Transfer Money\n";
        cout << "5. Display Account Information\n";
        cout << "6. View Transaction History\n";
        cout << "7. Exit\n";

        cout << "\nEnter your choice: ";
        cin >> choice;

        if (choice == 1)
        {
            int customerID, accountNumber;
            string name;
            double initialBalance;

            cout << "Enter Customer ID: ";
            cin >> customerID;

            bool duplicate = false;

            for (int i = 0; i < customers.size(); i++)
            {
                if (customers[i].customerID == customerID)
                {
                    duplicate = true;
                    break;
                }
            }

            if (duplicate)
            {
                cout << "Customer ID already exists!\n";
                continue;
            }

            cout << "Enter Customer Name: ";
            cin >> name;

            cout << "Enter Account Number: ";
            cin >> accountNumber;

            bool accountExists = false;

            for (int i = 0; i < accounts.size(); i++)
            {
                if (accounts[i].accountNumber == accountNumber)
                {
                    accountExists = true;
                    break;
                }
            }

            if (accountExists)
            {
                cout << "Account number already exists!\n";
                continue;
            }

            cout << "Enter Initial Balance: ";
            cin >> initialBalance;

            if (initialBalance < 0)
            {
                cout << "Invalid initial balance!\n";
                continue;
            }

            customers.push_back(Customer(customerID, name));

            accounts.push_back(
                Account(accountNumber, customerID, initialBalance)
            );

            cout << "Customer and Account created successfully!\n";
        }

        else if (choice == 2)
        {
            int acc;
            double amount;

            cout << "Enter Account Number: ";
            cin >> acc;

            bool found = false;

            for (int i = 0; i < accounts.size(); i++)
            {
                if (accounts[i].accountNumber == acc)
                {
                    cout << "Enter Deposit Amount: ";
                    cin >> amount;

                    accounts[i].deposit(amount);

                    found = true;
                    break;
                }
            }

            if (!found)
                cout << "Account not found!\n";
        }

        else if (choice == 3)
        {
            int acc;
            double amount;

            cout << "Enter Account Number: ";
            cin >> acc;

            bool found = false;

            for (int i = 0; i < accounts.size(); i++)
            {
                if (accounts[i].accountNumber == acc)
                {
                    cout << "Enter Withdrawal Amount: ";
                    cin >> amount;

                    accounts[i].withdraw(amount);

                    found = true;
                    break;
                }
            }

            if (!found)
                cout << "Account not found!\n";
        }

        else if (choice == 4)
        {
            int sender, receiver;
            double amount;

            cout << "Enter Sender Account Number: ";
            cin >> sender;

            cout << "Enter Receiver Account Number: ";
            cin >> receiver;

            if (sender == receiver)
            {
                cout << "Cannot transfer to the same account!\n";
                continue;
            }

            int senderIndex = -1;
            int receiverIndex = -1;

            for (int i = 0; i < accounts.size(); i++)
            {
                if (accounts[i].accountNumber == sender)
                    senderIndex = i;

                if (accounts[i].accountNumber == receiver)
                    receiverIndex = i;
            }

            if (senderIndex == -1 || receiverIndex == -1)
            {
                cout << "Account not found!\n";
                continue;
            }

            cout << "Enter Transfer Amount: ";
            cin >> amount;

            if (amount <= 0)
            {
                cout << "Invalid transfer amount!\n";
                continue;
            }

            if (accounts[senderIndex].balance >= amount)
            {
                accounts[senderIndex].withdraw(amount);

                accounts[receiverIndex].deposit(amount);

                cout << "Transfer successful!\n";
            }
            else
            {
                cout << "Insufficient balance!\n";
            }
        }

        else if (choice == 5)
        {
            int acc;

            cout << "Enter Account Number: ";
            cin >> acc;

            bool found = false;

            for (int i = 0; i < accounts.size(); i++)
            {
                if (accounts[i].accountNumber == acc)
                {
                    for (int j = 0; j < customers.size(); j++)
                    {
                        if (customers[j].customerID ==
                            accounts[i].customerID)
                        {
                            customers[j].display();
                            break;
                        }
                    }

                    accounts[i].displayAccount();

                    found = true;
                    break;
                }
            }

            if (!found)
                cout << "Account not found!\n";
        }

        else if (choice == 6)
        {
            int acc;

            cout << "Enter Account Number: ";
            cin >> acc;

            bool found = false;

            for (int i = 0; i < accounts.size(); i++)
            {
                if (accounts[i].accountNumber == acc)
                {
                    accounts[i].displayHistory();

                    found = true;
                    break;
                }
            }

            if (!found)
                cout << "Account not found!\n";
        }

        else if (choice == 7)
        {
            cout << "Thank you for using our Banking System!\n";
        }

        else
        {
            cout << "Invalid choice!\n";
        }

    } while (choice != 7);

    return 0;
}
