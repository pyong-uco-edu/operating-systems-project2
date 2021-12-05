#include <iostream>
#include <unistd.h>
#include <random>
#include <fstream>
#include <semaphore.h>

#include "defs.h"

extern stats checking_account;
extern stats savings_account;

extern stats th_checking[10];
extern stats th_savings[10];

extern int buffer[BUF_SIZE];

extern pthread_mutex_t mutex;
extern sem_t full;
extern sem_t empty;

int randomRange(int start, int end) {
    std::random_device rand_dev;
    std::mt19937 generator(rand_dev());
    std::uniform_int_distribution<int> distr(start, end);

    return distr(generator);
}

void* operations(void* p) {
    parameters* params = (parameters*) p;
    int loop = params->loop_count;

    std::ofstream file;
	file.open(params->filename);

    int amount;

    for (int i = 0; i < loop; i++) {
        switch(randomRange(1, 6)) {
            case 1: // deposit into checking account
                amount = randomRange(50, 100);
                checking_account.no_deposits++;
                checking_account.balance += amount;
                th_checking[params->index].no_deposits++;
                th_checking[params->index].balance += amount;
                file << "DEPOSIT CHECKING " << amount << std::endl;
                std::cout << "DEPOSIT CHECKING " << amount << std::endl;
                break;
            case 2: // withdraw from checking account
                amount = randomRange(50, 100);
                if (checking_account.balance >= amount) {
                    checking_account.no_withdrawals++;
                    checking_account.balance -= amount;
                    file << "WITHDRAWAL CHECKING " << amount << std::endl;
                    std::cout << "WITHDRAWAL CHECKING " << amount << std::endl;
                } else {
                    checking_account.no_rejected++;
                    file << "WITHDRAWAL CHECKING " << amount << " (REJECTED)" << std::endl;
                    std::cout << "WITHDRAWAL CHECKING " << amount << " (REJECTED)" << std::endl;
                }
                break;
            case 3: // deposited into savings account
                amount = randomRange(100, 150);
                savings_account.no_deposits++;
                savings_account.balance += amount;
                th_savings[params->index].no_deposits++;
                th_savings[params->index].balance += amount;
                file << "DEPOSIT SAVINGS " << amount << std::endl;
                std::cout << "DEPOSIT SAVINGS " << amount << std::endl;
                break;
            case 4: // withdraw from savings account
                amount = randomRange(100, 150);
                if (savings_account.balance >= amount) {
                    savings_account.no_withdrawals++;
                    savings_account.balance -= amount;
                    file << "WITHDRAWAL SAVINGS " << amount << std::endl;
                    std::cout << "WITHDRAWAL SAVINGS " << amount << std::endl;
                } else {
                    savings_account.no_rejected++;
                    file << "WITHDRAWAL SAVINGS " << amount << " (REJECTED)" << std::endl;
                    std::cout << "WITHDRAWAL SAVINGS " << amount << " (REJECTED)" << std::endl;
                }
                break;
            case 5: // transfer from checking to savings account
                amount = randomRange(100, 200);
                if (checking_account.balance >= amount) {
                    checking_account.no_withdrawals++;
                    checking_account.balance -= amount;
                    savings_account.no_deposits++;
                    savings_account.balance += amount;
                    file << "TRANSFER CHECKING TO SAVINGS " << amount << std::endl;
                    std::cout << "TRANSFER CHECKING TO SAVINGS " << amount << std::endl;
                } else {
                    checking_account.no_rejected++;
                    file << "TRANSFER CHECKING TO SAVINGS " << amount << " (REJECTED)" << std::endl;
                    std::cout << "TRANSFER CHECKING TO SAVINGS " << amount << " (REJECTED)" << std::endl;
                }
                break;
            case 6: // transfer from savings to checking account
                amount = randomRange(100, 200);
                if (savings_account.balance >= amount) {
                    savings_account.no_withdrawals++;
                    savings_account.balance -= amount;
                    checking_account.no_deposits++;
                    checking_account.balance += amount;
                    file << "TRANSFER SAVINGS TO CHECKING " << amount << std::endl;
                    std::cout << "TRANSFER SAVINGS TO CHECKING " << amount << std::endl;
                } else {
                    savings_account.no_rejected++;
                    file << "TRANSFER SAVINGS TO CHECKING " << amount << " (REJECTED)" << std::endl;
                    std::cout << "TRANSFER SAVINGS TO CHECKING " << amount << " (REJECTED)" << std::endl;
                }
                break;
            default:
                break;
        }
    }
    std::cout << std::endl << "---THREAD [" << params->index << "] STATS---" << std::endl;
    std::cout << "CHECKING ACCOUNT" << std::endl;
    std::cout << "- DEPOSITS:    " << th_checking[params->index].no_deposits << std::endl;
    std::cout << "- WITHDRAWALS: " << th_checking[params->index].no_withdrawals << std::endl;
    std::cout << "- REJECTED:    " << th_checking[params->index].no_rejected << std::endl;
    std::cout << "- BALANCE:     " << th_checking[params->index].balance << std::endl << std::endl;

    file << std::endl << "---THREAD [" << params->index << "] STATS---" << std::endl;
    file << "CHECKING ACCOUNT" << std::endl;
    file << "- DEPOSITS:    " << th_checking[params->index].no_deposits << std::endl;
    file << "- WITHDRAWALS: " << th_checking[params->index].no_withdrawals << std::endl;
    file << "- REJECTED:    " << th_checking[params->index].no_rejected << std::endl;
    file << "- BALANCE:     " << th_checking[params->index].balance << std::endl << std::endl;

    std::cout << "SAVINGS ACCOUNT" << std::endl;
    std::cout << "- DEPOSITS:    " << th_savings[params->index].no_deposits << std::endl;
    std::cout << "- WITHDRAWALS: " << th_savings[params->index].no_withdrawals << std::endl;
    std::cout << "- REJECTED:    " << th_savings[params->index].no_rejected << std::endl;
    std::cout << "- BALANCE:     " << th_savings[params->index].balance << std::endl << std::endl;

    file << "SAVINGS ACCOUNT" << std::endl;
    file << "- DEPOSITS:    " << th_savings[params->index].no_deposits << std::endl;
    file << "- WITHDRAWALS: " << th_savings[params->index].no_withdrawals << std::endl;
    file << "- REJECTED:    " << th_savings[params->index].no_rejected << std::endl;
    file << "- BALANCE:     " << th_savings[params->index].balance << std::endl << std::endl;

    file.close();
    pthread_exit(0);
}