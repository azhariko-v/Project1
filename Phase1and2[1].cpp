#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <chrono>

using namespace std;

class Account {//account with a balance, capability to deposit or withdraw funds,
//and capability to preform a transfer to another account
private:
double balance;

public:
    Account(double balance) : balance(balance) {}

    void transaction(double amount) {    //facilitate withdrawl or deposit
        cout << "Processing transaction of amount " << amount << endl;
        balance += amount;
        cout << "New balance: " << balance << endl;
    }

    void transfer(Account &other, double amount1, double amount2) {//facilitate transaction between accounts

        transaction(amount1);
        other.transaction(amount2);
    }
};

void unprotectedTransaction(Account &account, double amount) {//unprotected transaction
    account.transaction(amount);
}

void protectedTransaction(Account &account, double amount) {//protected transaction
    mutex mut;
    lock_guard<mutex> lock(mut);  //lockguard mutex locks resource to nonrunning process
    account.transaction(amount);
}


int main() {
    Account account1(2000);
    Account account2(900);
    vector<thread> threads;
//P1 standard transaction
    for (int i = 0; i < 10; ++i) {// 10 threads
        double amount = i * 2 + 12;
        threads.push_back(thread(unprotectedTransaction, ref(account1), amount));
    }
//P2 protected transaction with mutex lock
cout << "\n\n\n";
    for (int i = 0; i < 10; ++i) {
        double amount = (i + 1) * 5;
        threads.push_back(thread(protectedTransaction, ref(account1), amount));
    }

    for (auto &t : threads) {//ensure all threads completed
        t.join();
    }

    return 0;
}