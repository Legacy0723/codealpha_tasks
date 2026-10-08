#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <ctime>
#include <fstream>
#include <sstream>
#include <limits>
#include <cctype>
#include <cstdlib>
#include <conio.h>

using namespace std;

// Get current date and time as text
string getCurrentDateTime(){
    time_t now = time(nullptr);
    char buffer[30];
    strftime(buffer, sizeof(buffer),"%Y-%m-%d %H:%M", localtime(&now));
    return string(buffer);
}

// Hash a PIN or staff key (FNV-1a: fine for learning, not secure)
string hashText(const string &s){
    unsigned long long h = 1469598103934665603ULL;
    for(size_t i = 0; i < s.size(); i++){
        h ^= (unsigned char)s[i];
        h *= 1099511628211ULL;
    }
    return to_string(h);
}

// Exit cleanly if input is closed (Ctrl+C or end of input)
void inputClosed(){
    cout << "\nInput closed. Exiting.\n";
    exit(0);
}

// Read a PIN or key while showing * for each character
string getMaskedPassWord(){
    string password;

    while(true){
        int ch = _getch();

        if(ch == 0 || ch == 224){
            _getch();
            continue;
        }
        if(ch == 3){
            inputClosed();
        }

        if(ch == 13){
            cout << endl;
            break;
        }
        else if(ch == 8){
            if(!password.empty()){
                password.pop_back();
                cout << "\b \b";
            }
        }
        else if(ch >= 32 && ch < 127){
            password += (char)ch;
            cout << '*';
        }
    }

    return password;
}

// Read an integer, repeating until the input is valid
int readInt(const string &prompt){
    int v;
    cout << prompt;
    while(!(cin >> v)){
        if(cin.eof()) inputClosed();
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid number. Try again: ";
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return v;
}

// Read a decimal amount, repeating until the input is valid
double readDouble(const string &prompt){
    double v;
    cout << prompt;
    while(!(cin >> v)){
        if(cin.eof()) inputClosed();
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid amount. Try again: ";
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return v;
}

// Read a non-empty line; '|' is banned because it separates data file fields
string readLine(const string &prompt){
    string s;
    while(true){
        cout << prompt;
        if(!getline(cin, s)) inputClosed();
        if(s.empty()){
            cout << "This cannot be empty.\n";
            continue;
        }
        if(s.find('|') != string::npos){
            cout << "The '|' character is not allowed.\n";
            continue;
        }
        return s;
    }
}

// Read a phone number: exactly 10 digits
string readPhone(const string &prompt){
    while(true){
        string p = readLine(prompt);
        bool ok = (p.size() == 10);
        for(size_t i = 0; i < p.size() && ok; i++){
            if(!isdigit((unsigned char)p[i])) ok = false;
        }
        if(ok) return p;
        cout << "Phone must be exactly 10 digits.\n";
    }
}


// Read account type: 1 = Savings, 2 = Current
string readAccountType(){
    while(true){
        int t = readInt("Account Type (1. Savings  2. Current): ");
        if(t == 1) return "Savings";
        if(t == 2) return "Current";
        cout << "Please pick 1 or 2.\n";
    }
}

// Read a PIN: exactly 4 digits
string readPin(const string &prompt){
    while(true){
        string p;
        cout << prompt;
        p = getMaskedPassWord();
        bool ok = (p.size() == 4);
        for(size_t i = 0; i < p.size() && ok; i++){
            if(!isdigit((unsigned char)p[i])) ok = false;
        }
        if(ok) return p;
        cout << "PIN must be exactly 4 digits.\n";
    }
}

// Read a staff key: at least 6 characters
string readStaffKey(const string &prompt){
    while(true){
        string k;
        cout << prompt;
        k = getMaskedPassWord();
        if(k.size() >= 6) return k;
        cout << "Staff key must be at least 6 characters.\n";
    }
}

// Transaction Class
class Transaction{
private:
    string transactionType;
    double amount;
    string dateTime;
    string description;
    double balanceAfter;

public:
    Transaction(string type, double amt, string date, string desc, double balAfter = -1){
    transactionType = type;
    amount = amt;
    dateTime = date;
    description = desc;
    balanceAfter = balAfter;
    }

    void writeTo(ofstream &file) const {
        file << transactionType << "|" << fixed << setprecision(2)
             << amount << "|" << dateTime << "|" << description
             << "|" << balanceAfter << endl;
    }

    void displayTransaction() const {
        cout << "Type: " << transactionType << endl;
        cout << "Amount: GHc " << fixed << setprecision(2) << amount <<endl;
        cout << "Date: " << dateTime << endl;
        cout << "Description: " << description << endl;
        if(balanceAfter >= 0){
            cout << "Balance After: GHc " << fixed << setprecision(2) << balanceAfter << endl;
        }
    }
};

// Account Class
class Account{
private:
    string accountNumber;
    string accountType;
    double balance;
    string customerID;
    vector<Transaction> transactions;

public:
    Account(string accNumber, string accType, string CustID, double initialBalance = 0.0){
    accountNumber = accNumber;
    accountType = accType;
    customerID = CustID;
    balance = initialBalance;
    }

    string getAccountNumber() const {
        return accountNumber;
    }
    string getCustomerNumber() const{
        return customerID;
    }
    string getAccountType() const {
        return accountType;
    }
    size_t transactionCount() const {
        return transactions.size();
    }

    double getBalance() const {
        return balance;
    }
    void addTransaction(const Transaction &t){
        transactions.push_back(t);
    }

    bool deposit(double amount, string desc = "Cash Deposit"){
        if(amount <= 0){
            return false;
        }
        balance += amount;
        transactions.push_back(Transaction("Deposit",amount,getCurrentDateTime(),desc,balance));
        return true;
    }
    bool withdraw(double amount, string desc = "Cash Withdraw"){
        if(balance < amount){
            return false;
        }
        if(amount <= 0){
            return false;
        }
        balance -= amount;
        transactions.push_back(Transaction("Withdraw",amount,getCurrentDateTime(),desc,balance));
        return true;

    }

    bool transfer(Account &to, double amount){
        if(withdraw(amount, "Transfer to " + to.accountNumber)){
            to.deposit(amount, "Transfer from " + accountNumber);
            return true;
        }
        else {
            return false;
        }
    }

    void displayTransactions(size_t count) const{
        size_t start = 0;
        if(transactions.size() > count){
            start = transactions.size()-count;

        }
        for(size_t i = start; i < transactions.size(); i++){
            cout << "Transaction " << i + 1 << endl;
            transactions[i].displayTransaction();
            cout << "\n";
        }

    }

    void writeAccountTo(ofstream &file) const {
        file << accountNumber << "|" << accountType << "|" << customerID
             << "|" << fixed << setprecision(2) << balance <<endl;
    }

    void writeTransactionTo(ofstream &file) const {
        for(size_t i =0; i < transactions.size(); i++){
            file << accountNumber << "|";
            transactions[i].writeTo(file);
        }
    }

    void displayAccount() const{
        cout << "\nAccount Number: " << accountNumber << endl;
        cout << "Account Type: " << accountType << endl;
        cout << "Customer ID: " << customerID << endl;
        cout << "Balance: GHc " << fixed << setprecision(2) << balance << endl;
        cout << "\n\n";

    }

};

// Customer Class
class Customer{
private:
    string customerID;
    string name;
    string phoneNumber;
    string pinHash;

public:
    Customer(string id, string customerName, string phone, string pin = ""){
    customerID = id;
    name = customerName;
    phoneNumber = phone;
    pinHash = pin;
    }
    string getCustomerID() const{
        return customerID;
    }

    string getCustomerName() const {
        return name;
    }

    string getCustomerPhone() const {
        return phoneNumber;
    }
    string getPinHash() const {
        return pinHash;
    }
    bool hasPin() const {
        return !pinHash.empty();
    }
    void setPin(const string &plainPin){
        pinHash = hashText(customerID + ":" + plainPin);
    }
    bool verifyPin(const string &plainPin) const {
        return hasPin() && pinHash == hashText(customerID + ":" + plainPin);
    }
    void setName(const string &n){
        name = n;
    }
    void setPhone(const string &p){
        phoneNumber = p;
    }
    void displayCustomer() const {
        cout << "\nCustomer ID: " << customerID << endl;
        cout << "Name: " << name << endl;
        cout << "Phone Number: " << phoneNumber << endl;
        cout << "\n";
    }
};

// Bank Class
class Bank{
private:
    vector<Customer> customers;
    vector<Account> accounts;
    string staffKeyHash;

public:

    Account* findAccount(string accNumber){
        for(size_t i = 0; i < accounts.size(); i++){
            if(accounts[i].getAccountNumber() == accNumber){
                return &accounts[i];
            }
        }
        return nullptr;
    }

    Customer* findCustomer(string id){
            for(size_t i = 0; i < customers.size(); i++){
                if(customers[i].getCustomerID() == id){
                    return &customers[i];
            }
        }
        return nullptr;
    }

    bool addcustomer(const Customer &c){
        if(findCustomer(c.getCustomerID())){
            cout << "Customer ID already exists!\n";
            return false;
        }
        customers.push_back(c);
        return true;
    }

    bool addAccount(const Account &a){
        if(a.getBalance() < 0){
            cout << "Initial balance cannot be negative!\n";
            return false;
        }
        if(findAccount(a.getAccountNumber())){
            cout << "Account Number already exist!\n";
            return false;
        }
        if(!findCustomer(a.getCustomerNumber())){
            cout << "Customer not found!\n";
            return false;
        }
        accounts.push_back(a);
        return true;
    }

    bool updateCustomer(const string &id, const string &newName, const string &newPhone){
        Customer* c = findCustomer(id);
        if(c == nullptr) return false;
        c->setName(newName);
        c->setPhone(newPhone);
        return true;
    }

    bool closeAccount(const string &accNumber){
        for(size_t i = 0; i < accounts.size(); i++){
            if(accounts[i].getAccountNumber() == accNumber){
                if(accounts[i].getBalance() != 0){
                    cout << "Account can only be closed when the balance is zero.\n";
                    return false;
                }
                accounts.erase(accounts.begin() + i);
                return true;
            }
        }
        cout << "Account Not Found!\n";
        return false;
    }

    bool hasAccounts(const string &custID) const {
        for(size_t i = 0; i < accounts.size(); i++){
            if(accounts[i].getCustomerNumber() == custID) return true;
        }
        return false;
    }

    bool deleteCustomer(const string &id){
        for(size_t i = 0; i < customers.size(); i++){
            if(customers[i].getCustomerID() == id){
                if(hasAccounts(id)){
                    cout << "Cannot delete: this customer still has account(s). Close them first.\n";
                    return false;
                }
                customers.erase(customers.begin() + i);
                return true;
            }
        }
        cout << "Customer Not Found!\n";
        return false;
    }

    bool resetCustomerPin(const string &id, const string &newPin){
        Customer* c = findCustomer(id);
        if(c == nullptr) return false;
        c->setPin(newPin);
        return true;
    }

    void displayAccountsOf(const string &custID) const {
        bool found = false;
        for(size_t i = 0; i < accounts.size(); i++){
            if(accounts[i].getCustomerNumber() == custID){
                accounts[i].displayAccount();
                found = true;
            }
        }
        if(!found) cout << "\nNo accounts found for this customer.\n";
    }

    void displayAllAccounts() const {
        for(size_t i =0; i < accounts.size(); i++){
            accounts[i].displayAccount();
        }

    }

    void displayAllCustomers() const {
        for(size_t i = 0; i < customers.size(); i++){
            customers[i].displayCustomer();
        }
    }

    bool hasStaffKey() const {
        return !staffKeyHash.empty();
    }
    void setStaffKey(const string &plainKey){
        staffKeyHash = hashText("staff:" + plainKey);
    }
    bool checkStaffKey(){
        for(int attempt = 1; attempt <= 3; attempt++){
            string key;
            cout << "Enter Staff Key: ";
            key = getMaskedPassWord();
            if(hasStaffKey() && hashText("staff:" + key) == staffKeyHash){
                return true;
            }
            cout << "Wrong key. Attempts left: " << 3 - attempt << endl;
        }
        return false;
    }

    void saveAll() const{
        ofstream custfile("customers.txt");
        for(size_t i = 0; i < customers.size(); i++){
            custfile << customers[i].getCustomerID() << "|"
                 << customers[i].getCustomerName() << "|"
                 << customers[i].getCustomerPhone() << "|"
                 << customers[i].getPinHash() << endl;
        }
        ofstream accfile("accounts.txt");
        ofstream txtfile("transactions.txt");
        for(size_t i = 0; i < accounts.size(); i++){
            accounts[i].writeAccountTo(accfile);
            accounts[i].writeTransactionTo(txtfile);
        }
        ofstream keyfile("staff.key");
        keyfile << staffKeyHash << endl;
    }

    void loadAll() {
        string line;

        ifstream keyfile("staff.key");
        if(getline(keyfile, line)){
            staffKeyHash = line;
        }

        ifstream custfile("customers.txt");
        while(getline(custfile, line)){
            if(line.empty()) continue;
            stringstream ss(line);
            string id, name, phone, pin;
            getline(ss, id, '|');
            getline(ss, name, '|');
            getline(ss, phone, '|');
            getline(ss, pin, '|');
            customers.push_back(Customer(id, name, phone, pin));
        }

        ifstream accFile("accounts.txt");
        while(getline(accFile, line)){
            if(line.empty()) continue;
            stringstream ss(line);
            string accNum, type, custID, balanceText;
            getline(ss, accNum, '|');
            getline(ss, type, '|');
            getline(ss, custID, '|');
            getline(ss, balanceText, '|');
            try{
                accounts.push_back(Account(accNum,type,custID, stod(balanceText)));
            }
            catch(...){
                cout << "Skipped a corrupted line in accounts.txt\n";
            }
        }

        ifstream txtFile("transactions.txt");
        while(getline(txtFile, line)){
            if(line.empty()) continue;
            stringstream ss(line);
            string accNum, type, amountText, date, desc, balText;
            getline(ss, accNum, '|');
            getline(ss, type, '|');
            getline(ss, amountText, '|');
            getline(ss, date, '|');
            getline(ss, desc, '|');
            getline(ss, balText, '|');
            Account* acc = findAccount(accNum);
            if(acc != nullptr){
                try{
                    double balAfter = balText.empty() ? -1 : stod(balText);
                    acc->addTransaction(Transaction(type, stod(amountText), date, desc, balAfter));
                }
                catch(...){
                    cout << "Skipped a corrupted line in transactions.txt\n";
                }
            }
        }
    }

};

void customerSession(Bank &bank){
    string accNumber = readLine("Account Number: ");
    string custID = readLine("Customer ID: ");

    Account* acc = bank.findAccount(accNumber);
    Customer* cust = bank.findCustomer(custID);
    if(acc == nullptr || cust == nullptr || acc->getCustomerNumber() != custID){
        cout << "\nInvalid account number or customer ID!\n";
        return;
    }

    if(!cust->hasPin()){
        cout << "\nNo PIN is set on this profile yet. Please create one.\n";
        cust->setPin(readPin("Create 4-digit PIN: "));
        bank.saveAll();
    }
    else{
        bool ok = false;
        for(int attempt = 1; attempt <= 3 && !ok; attempt++){
            string pin;
            cout << "Enter PIN: ";
            pin = getMaskedPassWord();
            if(cust->verifyPin(pin)){
                ok = true;
            }
            else{
                cout << "Wrong PIN. Attempts left: " << 3 - attempt << endl;
            }
        }
        if(!ok){
            cout << "\nToo many wrong attempts. Returning to main menu.\n";
            return;
        }
    }

    cout << "\nWelcome, " << cust->getCustomerName() << "!\n";

// Customer Session
    int choice;
    do{
        acc = bank.findAccount(accNumber);
        cust = bank.findCustomer(custID);
        cout << "\n====== CUSTOMER MENU ======\n";
        cout << "1. Account Details\n";
        cout << "2. Transaction History\n";
        cout << "3. Deposit\n";
        cout << "4. Withdraw\n";
        cout << "5. Transfer Funds\n";
        cout << "6. Change PIN\n";
        cout << "7. Logout\n";
        choice = readInt("Enter Choice: ");

        switch(choice){
            case 1:
                acc->displayAccount();
                break;

            case 2:{
                if(acc->transactionCount() == 0){
                    cout << "\nNo transactions yet.\n";
                    break;
                }
                int n = readInt("How many recent transactions: ");
                if(n <= 0){
                    cout << "\nEnter a number greater than 0.\n";
                    break;
                }
                cout << "\n";
                acc->displayTransactions((size_t)n);
                break;
            }

            case 3:{
                double amount = readDouble("Amount: ");
                if(acc->deposit(amount)){
                    cout << "\nDeposit Successful! New balance: GHc "
                         << fixed << setprecision(2) << acc->getBalance() << endl;
                    bank.saveAll();
                }
                else{
                    cout << "\nDeposit Failed! Amount must be greater than 0.\n";
                }
                break;
            }

            case 4:{
                double amount = readDouble("Amount: ");
                if(acc->withdraw(amount)){
                    cout << "\nWithdrawal Successful! New balance: GHc "
                         << fixed << setprecision(2) << acc->getBalance() << endl;
                    bank.saveAll();
                }
                else{
                    cout << "\nWithdrawal Failed! Check the amount and your balance.\n";
                }
                break;
            }

            case 5:{
                string toNumber = readLine("Receiver Account Number: ");
                Account* to = bank.findAccount(toNumber);
                if(to == nullptr){
                    cout << "\nReceiver Account Not Found!\n";
                    break;
                }
                if(to == acc){
                    cout << "\nCannot Transfer To Same Account!\n";
                    break;
                }
                Customer* rc = bank.findCustomer(to->getCustomerNumber());
                cout << "Receiver: " << (rc ? rc->getCustomerName() : "Unknown") << endl;
                int confirm = readInt("Send to this person? (1. Yes  2. No): ");
                if(confirm != 1){
                    cout << "\nTransfer cancelled.\n";
                    break;
                }
                double amount = readDouble("Amount: ");
                if(acc->transfer(*to, amount)){
                    cout << "\nTransfer Successful!\n";
                    bank.saveAll();
                }
                else{
                    cout << "\nTransfer failed! Check the amount and your balance.\n";
                }
                break;
            }

            case 6:{
                string oldPin;
                cout << "Current PIN: ";
                oldPin = getMaskedPassWord();
                if(!cust->verifyPin(oldPin)){
                    cout << "\nWrong PIN!\n";
                    break;
                }
                cust->setPin(readPin("New 4-digit PIN: "));
                bank.saveAll();
                cout << "\nPIN changed!\n";
                break;
            }

            case 7:
                cout << "\nLogged out.\n";
                break;

            default:
                cout << "\nInvalid choice! Please pick 1 to 7\n";
                break;
        }
    }
    while(choice != 7);
}

// Staff Session
void staffSession(Bank &bank){
    int choice;
    do{
        cout << "\n======= STAFF MENU =======\n";
        cout << "1. Create Customer Profile\n";
        cout << "2. Open Account\n";
        cout << "3. Update Customer Details\n";
        cout << "4. Reset Customer PIN\n";
        cout << "5. Close Account\n";
        cout << "6. Delete Customer\n";
        cout << "7. View a Customer's Accounts\n";
        cout << "8. View All Customers\n";
        cout << "9. View All Accounts\n";
        cout << "10. Change Staff Key\n";
        cout << "11. Logout\n";
        choice = readInt("Enter Choice: ");

        switch(choice){
            case 1:{
                string id = readLine("Customer ID: ");
                string name = readLine("Name: ");
                string phone = readPhone("Phone (10 digits): ");
                string pin = readPin("Set 4-digit PIN for customer: ");
                Customer c(id, name, phone);
                c.setPin(pin);
                if(bank.addcustomer(c)){
                    cout << "\nCustomer created!\n";
                    bank.saveAll();
                }
                break;
            }

            case 2:{
                string accNumber = readLine("Account Number: ");
                string type = readAccountType();
                string custID = readLine("Customer ID: ");
                double initial = readDouble("Initial balance: ");
                if(bank.addAccount(Account(accNumber, type, custID, initial))){
                    cout << "\nAccount opened!\n";
                    bank.saveAll();
                }
                break;
            }

            case 3:{
                string id = readLine("Customer ID: ");
                Customer* c = bank.findCustomer(id);
                if(c == nullptr){
                    cout << "\nCustomer Not Found!\n";
                    break;
                }
                c->displayCustomer();
                string name = readLine("New Name: ");
                string phone = readPhone("New Phone (10 digits): ");
                bank.updateCustomer(id, name, phone);
                cout << "\nCustomer updated!\n";
                bank.saveAll();
                break;
            }

            case 4:{
                string id = readLine("Customer ID: ");
                if(bank.findCustomer(id) == nullptr){
                    cout << "\nCustomer Not Found!\n";
                    break;
                }
                string pin = readPin("New 4-digit PIN: ");
                bank.resetCustomerPin(id, pin);
                cout << "\nPIN reset!\n";
                bank.saveAll();
                break;
            }

            case 5:{
                string accNumber = readLine("Account Number to close: ");
                if(bank.closeAccount(accNumber)){
                    cout << "\nAccount closed.\n";
                    bank.saveAll();
                }
                break;
            }

            case 6:{
                string id = readLine("Customer ID to delete: ");
                int sure = readInt("Are you sure? (1. Yes  2. No): ");
                if(sure != 1){
                    cout << "\nCancelled.\n";
                    break;
                }
                if(bank.deleteCustomer(id)){
                    cout << "\nCustomer deleted.\n";
                    bank.saveAll();
                }
                break;
            }

            case 7:{
                string id = readLine("Customer ID: ");
                bank.displayAccountsOf(id);
                break;
            }

            case 8:
                bank.displayAllCustomers();
                break;

            case 9:
                bank.displayAllAccounts();
                break;

            case 10:{
                bank.setStaffKey(readStaffKey("New staff key (min 6 characters): "));
                bank.saveAll();
                cout << "\nStaff key changed!\n";
                break;
            }

            case 11:
                cout << "\nStaff logged out.\n";
                break;

            default:
                cout << "\nInvalid choice! Please pick 1 to 11\n";
                break;
        }
    }
    while(choice != 11);
}

// Program entry point: first-time setup, then the main menu
int main()
{
    Bank bank;
    bank.loadAll();

    if(!bank.hasStaffKey()){
        cout << "\n=== FIRST-TIME SETUP ===\n";
        cout << "No staff key found. Create one now.\n";
        bank.setStaffKey(readStaffKey("Create staff key (min 6 characters): "));
        bank.saveAll();
    }

    int choice;
    do{
        cout << "\n====================\n";
        cout << "   BANKING SYSTEM   \n";
        cout << "====================\n";
        cout << "1. Customer\n";
        cout << "2. Staff\n";
        cout << "3. Exit\n";
        choice = readInt("Enter Choice: ");

        switch(choice){
            case 1:
                customerSession(bank);
                break;
            case 2:
                if(bank.checkStaffKey()){
                    staffSession(bank);
                }
                else{
                    cout << "\nAccess denied!\n";
                }
                break;
            case 3:
                bank.saveAll();
                cout << "\n===Good Bye!====\n";
                break;
            default:
                cout << "\nInvalid choice! Please pick 1 to 3\n";
                break;
        }
    }
    while(choice != 3);

    return 0;
}

