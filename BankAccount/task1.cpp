#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <atomic>
#include<windows.h>
using namespace std;
mutex m;
class BankAccount {
	string owner;
	double balance;
public:
	BankAccount(string o, double b) : owner(o), balance(b) {}
	void deposit(double amount) {
		if(amount <= 0) {
			cout << "Invalid deposit amount." << endl;
			return;
		}
		m.lock();
		balance += amount;
		m.unlock();
	}
	void withdraw(double amount) {
		if (amount <= 0) {
			cout << "Invalid withdrawal amount." << endl;
			return;
		}
		m.lock();
		if(amount > balance) {
			cout << "Insufficient funds." << endl;
			m.unlock();
			return;
		}
		balance -= amount;
		m.unlock();
	}
	void printInfo() const {
		cout << "Owner: " << owner << ", Balance: " << balance << endl;
	}
	void addPersent() {
		m.lock();
		balance += balance * 0.01;
		m.unlock();
	}
};
void persent(BankAccount& acc, atomic<bool>& flag) {
	while (!flag.load()) {
		Sleep(2000);
		if (flag.load()) {
			break;
		}
		acc.addPersent();
		cout << " + 1% to balance" << endl;
	}
}
int main()
{
	string name;
	double startBalance;

	cout << "Enter name: ";
	cin >> name;
	cout << "Enter starting balance: ";
	cin >> startBalance;
	BankAccount account(name, startBalance);
	atomic<bool> flag(false);
	thread t(persent, ref(account), ref(flag));
	int choice;
	double amount;
	while (true) {
		cout << "Enter command (deposit, withdraw, balance, exit): ";
		cin >> choice;
		if (choice == 1) {
			cout << "Enter amount to deposit: ";
			cin >> amount;
			account.deposit(amount);
		} else if(choice == 2) {
			cout << "Enter amount to withdraw: ";
			cin >> amount;
			account.withdraw(amount);
		} else if(choice == 3) {
			account.printInfo();
		} else if(choice == 4) {
			flag = true;
			break;
		} 
		else {
			cout << "Invalid command." << endl;
		} 
	}
	t.join();
}