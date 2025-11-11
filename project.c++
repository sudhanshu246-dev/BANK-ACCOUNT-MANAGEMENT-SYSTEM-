#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

class Account {
    int accNo;
    char name[50];
    char type;
    double balance;

public:
    void createAccount() {
        cout << "\nEnter Account Number: ";
        cin >> accNo;
        cout << "Enter Account Holder Name: ";
        cin.ignore();
        cin.getline(name, 50);
        cout << "Enter Account Type (S for Savings / C for Current): ";
        cin >> type;
        type = toupper(type);
        cout << "Enter Initial Deposit (minimum 500 for Savings, 1000 for Current): ";
        cin >> balance;
        cout << "\nAccount Created Successfully!\n";
    }

    void showAccount() const {
        cout << "\nAccount No.: " << accNo;
        cout << "\nAccount Holder Name: " << name;
        cout << "\nType of Account: " << type;
        cout << "\nBalance amount: " << balance << endl;
    }

    void modify() {
        cout << "\nModify Account Holder Name: ";
        cin.ignore();
        cin.getline(name, 50);
        cout << "Modify Type of Account: ";
        cin >> type;
        type = toupper(type);
        cout << "Modify Balance amount: ";
        cin >> balance;
    }

    void deposit(double amt) {
        balance += amt;
    }

    void withdraw(double amt) {
        balance -= amt;
    }

    void report() const {
        cout << accNo << setw(15) << name << setw(10) << type << setw(12) << balance << endl;
    }

    int getAccNo() const { return accNo; }
    double getBalance() const { return balance; }
    char getType() const { return type; }
};

// ----- Function Declarations -----
void writeAccount();
void displayAccount(int);
void modifyAccount(int);
void deleteAccount(int);
void displayAll();
void depositWithdraw(int, int);

fstream fp;
Account ac;

int main() {
    char choice;
    int num;
    cout << "==============================================";
    cout << "\n     BANK ACCOUNT MANAGEMENT SYSTEM";
    cout << "\n==============================================";

    do {
        cout << "\n\nMAIN MENU";
        cout << "\n1. NEW ACCOUNT";
        cout << "\n2. DEPOSIT AMOUNT";
        cout << "\n3. WITHDRAW AMOUNT";
        cout << "\n4. BALANCE ENQUIRY";
        cout << "\n5. ALL ACCOUNT HOLDER LIST";
        cout << "\n6. CLOSE AN ACCOUNT";
        cout << "\n7. MODIFY AN ACCOUNT";
        cout << "\n8. EXIT";
        cout << "\nSelect Your Option (1-8): ";
        cin >> choice;

        switch(choice) {
            case '1': writeAccount(); break;
            case '2': cout << "\nEnter Account No: "; cin >> num; depositWithdraw(num, 1); break;
            case '3': cout << "\nEnter Account No: "; cin >> num; depositWithdraw(num, 2); break;
            case '4': cout << "\nEnter Account No: "; cin >> num; displayAccount(num); break;
            case '5': displayAll(); break;
            case '6': cout << "\nEnter Account No: "; cin >> num; deleteAccount(num); break;
            case '7': cout << "\nEnter Account No: "; cin >> num; modifyAccount(num); break;
            case '8': cout << "\nThank you for using Bank Management System.\n"; break;
            default: cout << "\nInvalid Option!";
        }
        cin.ignore();
        cin.get();
    } while(choice != '8');

    return 0;
}

// ----- Function Definitions -----

void writeAccount() {
    fp.open("accounts.dat", ios::out | ios::app);
    ac.createAccount();
    fp.write(reinterpret_cast<char*>(&ac), sizeof(Account));
    fp.close();
}

void displayAccount(int n) {
    bool found = false;
    fp.open("accounts.dat", ios::in);
    if(!fp) {
        cout << "\nFile could not be opened!";
        return;
    }
    while(fp.read(reinterpret_cast<char*>(&ac), sizeof(Account))) {
        if(ac.getAccNo() == n) {
            ac.showAccount();
            found = true;
        }
    }
    fp.close();
    if(!found)
        cout << "\nAccount number does not exist!";
}

void modifyAccount(int n) {
    bool found = false;
    fp.open("accounts.dat", ios::in | ios::out);
    while(!fp.eof() && !found) {
        int pos = fp.tellg();
        fp.read(reinterpret_cast<char*>(&ac), sizeof(Account));
        if(ac.getAccNo() == n) {
            ac.showAccount();
            cout << "\nEnter new details:\n";
            ac.modify();
            fp.seekp(pos);
            fp.write(reinterpret_cast<char*>(&ac), sizeof(Account));
            cout << "\nAccount Updated Successfully!";
            found = true;
        }
    }
    fp.close();
    if(!found)
        cout << "\nAccount not found!";
}

void deleteAccount(int n) {
    fp.open("accounts.dat", ios::in);
    fstream temp;
    temp.open("temp.dat", ios::out);
    fp.seekg(0, ios::beg);
    while(fp.read(reinterpret_cast<char*>(&ac), sizeof(Account))) {
        if(ac.getAccNo() != n)
            temp.write(reinterpret_cast<char*>(&ac), sizeof(Account));
    }
    fp.close();
    temp.close();
    remove("accounts.dat");
    rename("temp.dat", "accounts.dat");
    cout << "\nAccount Deleted Successfully!";
}

void displayAll() {
    fp.open("accounts.dat", ios::in);
    if(!fp) {
        cout << "\nFile could not be opened!";
        return;
    }
    cout << "\n====================================================";
    cout << "\nA/C No.      NAME           TYPE      BALANCE";
    cout << "\n====================================================\n";
    while(fp.read(reinterpret_cast<char*>(&ac), sizeof(Account))) {
        ac.report();
    }
    fp.close();
}

void depositWithdraw(int n, int option) {
    double amt;
    bool found = false;
    fp.open("accounts.dat", ios::in | ios::out);
    while(!fp.eof() && !found) {
        int pos = fp.tellg();
        fp.read(reinterpret_cast<char*>(&ac), sizeof(Account));
        if(ac.getAccNo() == n) {
            ac.showAccount();
            if(option == 1) {
                cout << "\nEnter Amount to Deposit: ";
                cin >> amt;
                ac.deposit(amt);
            }
            else if(option == 2) {
                cout << "\nEnter Amount to Withdraw: ";
                cin >> amt;
                if(ac.getBalance() - amt < 0)
                    cout << "Insufficient Balance!";
                else
                    ac.withdraw(amt);
            }
            fp.seekp(pos);
            fp.write(reinterpret_cast<char*>(&ac), sizeof(Account));
            cout << "\nRecord Updated!";
            found = true;
        }
    }
    fp.close();
    if(!found)
        cout << "\nAccount Not Found!";
}
