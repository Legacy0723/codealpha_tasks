# include <iostream>
# include <fstream>
# include <algorithm>
# include <cctype>
# include <conio.h>
 using namespace std;

 // Convert a string to lowercase (usernames are case-insensitive)
 string tolowerstring(string str){
    transform(str.begin(),str.end(),str.begin(),[](unsigned char c){
        return static_cast<char> (tolower(c));
    });
    return str;
 }

 // Hash a password with djb2 (unsalted: fine for learning, not secure)
 string hashPassword(const string& password){
    unsigned long hash = 5381;
    for (char c : password){
        hash = ((hash << 5)+ hash)+ c;
    }
    return to_string(hash);
 }

 // Read a password while showing * for each character
 string getMaskedPassWord(){
    string password;
    char ch;

    while(true){
        ch = _getch();

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
        else {
            password += ch;
            cout << '*';
        }
    }
    return password;
 }
// Check the rules: username 3+ characters with no spaces, password 8+ characters
bool isvalidinput(const string& username, const string& password){
    if(username.length() < 3){
        cout << "Error : Username must be at least 3 characters long.\n";
        return false;
    }
    if(password.length() < 8){
        cout << "Error: Password must be at least 8 characters long.\n";
        return false;
    }
    if(username.find(' ') != string::npos){
        cout << "Error: Username cannot contain spaces!\n";
        return false;
    }
    return true;
}
// Check whether a username already exists (case-insensitive)
bool isusernameTaken(const string& username){
    ifstream file("database.txt");
    if(!file.is_open()){
        return false;
    }
    string fileUser, filePass;
    string targetUser = tolowerstring(username);

    while(getline(file,fileUser) && getline(file,filePass)){
        if(tolowerstring(fileUser)== targetUser){
            file.close();
            return true;
        }
    }
    file.close();
    return false;
}

// Register a new user: validate, reject duplicates, save the hashed password
bool registerUser(const string& username,const string& password){
    if(!isvalidinput(username,password)){
        return false;
    }
    if(isusernameTaken(username)){
        cout << "Error : Username '" << username << "' is already taken!\n";
        return false;
    }
    ofstream file("database.txt", ios::app);
    if(!file.is_open()){
        cout << "Error: Could not open database file for writing.\n";
        return false;
    }
    file << username <<"\n" <<hashPassword(password) << "\n";
    file.close();

    cout << "Success: User '" << username << "' registered successfully!\n";

    return true;
}
// Log in: match the username (case-insensitive) and the hashed password
bool loginUser(const string& username, const string& password){
    ifstream file("database.txt");
    if(!file.is_open()){
        cout << "Error: No registered users found in the system.\n";
        return false;
     }
     string fileUser, filepass;
     while(getline(file, fileUser )&& getline(file, filepass)){
        if(tolowerstring(fileUser)== tolowerstring(username) && filepass == hashPassword(password)){
            file.close();
            cout << "Success: Login successful! Welcome, " << username << ".\n";
            return true;
        }
     }
     file.close();
     cout << "Error: Invalid username or password.\n";
     return false;
}

// Program entry point: menu loop for register, login and exit
int main(){
    int choice = 0;
    while(true){
        cout << "____________________________\n";
        cout << "\n";
        cout << "LOGIN AND REGISTRATION SYSTEM \n";
        cout << "____________________________\n";
        cout << "1.Register\n";
        cout << "2.Login\n";
        cout << "3.Exit\n";
        cout << "\n\n";
        cout << "Enter choice: ";


    if(!(cin >> choice)){
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Invalid selection. Try again.\n";
        continue;
    }
    if(choice == 1){
        string Username,PassWord;
        cout <<"\n--- Registration ---\n";
        cout << "Enter Username: ";
        getline(cin >> ws,Username);
        cout << "\n";
        cout << "Enter Password: ";
        PassWord = getMaskedPassWord();

        registerUser(Username,PassWord);
    }
    else if(choice == 2){
        string Username,PassWord;
        cout <<"\n--- Login---\n";
        cout << "Enter Username: ";
        getline(cin >> ws ,Username);
        cout << "\n";
        cout << "Enter Password: ";
        PassWord = getMaskedPassWord();
        loginUser(Username,PassWord);
    }

    else if(choice == 3){
        cout << "Exiting program.\n";
        break;
    }
    else {
        cout << "Invalid choice selected. Please enter 1,2 or 3 to continue.\n";
    }
    }
    return 0;
}
