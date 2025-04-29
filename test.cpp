//#include <iostream>
//#include <string>
//#include <SFML/Graphics.hpp>
//#include <sstream>
//using namespace std;
//
//class BankAccount {
//private:
//    string accountHolder;
//    int accountCode;
//    double balance=0;
//
//public:
//    // Constructor to initialize a new account
//    BankAccount(string name, int code, double initialBalance) {
//        accountHolder = name;
//        accountCode = code;
//        balance = initialBalance;
//    }
//
//    // Deposit money into the account
//    void deposit(double amount) {
//        if (amount > 0) {
//            balance += amount;
//        }
//    }
//
//    // Withdraw money from the account
//    bool withdraw(double amount) {
//        if (amount > 0 && amount <= balance) {
//            balance -= amount;
//            return true;
//        }
//        return false;
//    }
//
//    // Display account details as a string
//    string getDetails() {
//        stringstream ss;
//        ss << "Account Holder: " << accountHolder << "\n";
//        ss << "Account Code: " << accountCode << "\n";
//        ss << "Balance: $" << balance;
//        return ss.str();
//    }
//
//    // Verify account code
//    bool verifyCode(int code) {
//        return accountCode == code;
//    }
//};
//
//int main() {
//    string name;
//    int code;
//    double initialBalance;
//
//    cout << "Welcome to the Banking System!" << endl;
//    cout << "Enter your name: ";
//    getline(cin, name);
//    cout << "Create a 4-digit account code: ";
//    cin >> code;
//    cout << "Enter initial balance: ";
//    cin >> initialBalance;
//
//    BankAccount account(name, code, initialBalance);
//
//    sf::RenderWindow window(sf::VideoMode(600, 400), "Banking System");
//    sf::Font font;
//
//    if (!font.loadFromFile("arial.ttf")) {
//        cout << "Error loading font!" << endl;
//        return -1;
//    }
//
//    sf::Text title("Banking System", font, 30);
//    title.setPosition(150, 20);
//
//    sf::Text menuOptions("1. Deposit\n2. Withdraw\n3. Display Account Details\n4. Exit", font, 20);
//    menuOptions.setPosition(150, 80);
//
//    sf::Text prompt("", font, 20);
//    prompt.setPosition(150, 200);
//
//    sf::Text output("", font, 20);
//    output.setPosition(150, 300);
//
//    int currentMenu = 0; // 0: Main Menu, 1: Deposit, 2: Withdraw, 3: Display Details
//    string input = "";
//    double amount = 0;
//
//    while (window.isOpen()) {
//        sf::Event event;
//        while (window.pollEvent(event)) {
//            if (event.type == sf::Event::Closed) {
//                window.close();
//            }
//
//            if (event.type == sf::Event::TextEntered) {
//                if (event.text.unicode < 128) {
//                    char entered = static_cast<char>(event.text.unicode);
//                    if (entered == '\n' || entered == 13) { // Enter key
//                        if (currentMenu == 0 && !input.empty()) {
//                            int choice = stoi(input);
//                            if (choice >= 1 && choice <= 4) {
//                                currentMenu = choice;
//                                input.clear();
//                                output.setString("");
//                            }
//                        }
//                        else if ((currentMenu == 1 || currentMenu == 2) && !input.empty()) {
//                            amount = stod(input);
//                            if (currentMenu == 1) {
//                                account.deposit(amount);
//                                output.setString("Deposit successful!");
//                            }
//                            else if (currentMenu == 2) {
//                                if (account.withdraw(amount)) {
//                                    output.setString("Withdrawal successful!");
//                                }
//                                else {
//                                    output.setString("Insufficient balance.");
//                                }
//                            }
//                            currentMenu = 0;
//                            input.clear();
//                        }
//                    }
//                    else if (entered == 8) { // Backspace key
//                        if (!input.empty())
//                            input.pop_back();
//                    }
//                    else if (isdigit(entered) || entered == '.') {
//                        input += entered;
//                    }
//                }
//            }
//        }
//
//        window.clear(sf::Color::Blue);
//        window.draw(title);
//         window.draw(menuOptions);
//
//        if (currentMenu == 0) {
//            prompt.setString("Enter your choice: " + input);
//        }
//        else if (currentMenu == 1) {
//            prompt.setString("Enter deposit amount: " + input);
//        }
//        else if (currentMenu == 2) {
//            prompt.setString("Enter withdrawal amount: " + input);
//        }
//        else if (currentMenu == 3) {
//            prompt.setString("Account Details:");
//            output.setString(account.getDetails());
//            currentMenu = 0;
//        }
//        else if (currentMenu == 4) {
//            prompt.setString("THANK YOU ");
//            prompt.setString("THANK YOU ");
//        }
//
//        window.draw(prompt);
//        window.draw(output);
//        window.display();
//    }
//
//    return 0;
//}
