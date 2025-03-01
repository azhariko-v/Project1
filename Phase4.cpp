#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

using namespace std;

class Account {//account with a balance, capability to deposit or withdraw funds,
    //and capability to preform a transfer to another account
private:
double balance;

public:
    Account(double balance) : balance(balance) {}

    void deposit(int amount) {
        balance += amount;//basic account operations
    }

    void withdraw(int amount) {
        balance -= amount;
    }

    int getBalance() {
        return balance;
    }

    mutex mtx;

    mutex& getMutex() {
        return mtx;//initalize mutex
    }
}

void transfer(Account &from, Account &to, int amount) {//deadlock resolution
    if (&from < &to) {//compare account address to determine if locked
        lock(from.getMutex(), to.getMutex());
        lock_guard<mutex> lock1(from.getMutex(), adopt_lock);
        lock_guard<mutex> lock2(to.getMutex(), adopt_lock);
    } else {
        lock(to.getMutex(), from.getMutex());
        lock_guard<mutex> lock1(to.getMutex(), adopt_lock);
        lock_guard<mutex> lock2(from.getMutex(), adopt_lock);
    }

    from.withdraw(amount);
    to.deposit(amount);
}

int main() {
    Account account1(5000);
    Account account2(90);

    thread t1(transfer, ref(account1), ref(account2), 100);
    thread t2(transfer, ref(account2), ref(account1), 200);//transfer is preformed without deadlock

    t1.join();
    t2.join();

    cout << "Account 1 balance: " << account1.getBalance() << endl;
    cout << "Account 2 balance: " << account2.getBalance() << endl;

    return 0;
}
