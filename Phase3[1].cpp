#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <chrono>

using namespace std;

class Account {
private:
double balance;

public:
    Account(double balance) : balance(balance) {}

    mutex mtx;  //intiialize mutex

    void transaction(double amount) {
        lock_guard<mutex> lock(mtx);  //mutex
        cout << "Processing transaction of amount " << amount << endl;
        balance += amount;
        cout << "New balance: " << balance << endl;
    }

    void transfer(Account &other, double amount1, double amount2) {
        lock(mtx, other.mtx);  //lock accounts depending on other's status
        lock_guard<mutex> lock1(mtx, adopt_lock);
        lock_guard<mutex> lock2(other.mtx, adopt_lock);

        transaction(amount1);
        other.transaction(amount2);
    }
};

void transferMoney(Account &from, Account &to, double amount1, double amount2) {
    from.transfer(to, amount1, amount2);  //transfer between two accounts
}

int main() {
    Account account1(2000);
    Account account2(900);
    vector<thread> threads;

    threads.push_back(thread(transferMoney, ref(account1), ref(account2), 100, 50));
    threads.push_back(thread(transferMoney, ref(account2), ref(account1), 150, 75));//deadlocked threads

    for (auto &t : threads) {
        t.join();  //will not allow procedure before all threads exectute
    }

    cout<<"Message cannot be displayed due to deadlock";

    return 0;
}