#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <string>
#include <iostream>

#include "defs.h"

stats checking_account;
stats savings_account;

stats th_checking[10];
stats th_savings[10];

pthread_mutex_t mutex;

extern void* operations(void *);

int main(int argc, char **argv) {
    if (argc != 2) {
		printf("Usage: %s loop_count\n", argv[0]);
		exit(1);
	}

	int loop_count = atoi(argv[1]);
	if (loop_count == 0) {
		printf("Usage: %s max_loop_count\n", argv[0]);
		exit(1);
	}

	checking_account.no_deposits = 0;
    checking_account.no_withdrawals = 0;
    checking_account.no_rejected = 0;
    checking_account.balance = 0;

	savings_account.no_deposits = 0;
    savings_account.no_withdrawals = 0;
    savings_account.no_rejected = 0;
    savings_account.balance = 0;

	for (int i = 0; i < 10; i++) {
		th_checking[i].no_deposits = 0;
		th_checking[i].no_withdrawals = 0;
		th_checking[i].no_rejected = 0;
		th_checking[i].balance = 0;

		th_savings[i].no_deposits = 0;
		th_savings[i].no_withdrawals = 0;
		th_savings[i].no_rejected = 0;
		th_savings[i].balance = 0;
	}

    pthread_mutex_init(&mutex, NULL);

    pthread_t threads[10];

	pthread_attr_t attr;
	pthread_attr_init(&attr);

    parameters* params[10];

	for (int i = 0; i < 10; i++) {
		params[i] = new parameters;
		params[i]->loop_count = loop_count;
		std::string filename = "thread_";
		filename += std::to_string(i);
		filename += ".txt";
		params[i]->filename = filename;
		params[i]->index = i;
		pthread_create(&threads[i], &attr, operations, params[i]);
	}

	for (int i = 0; i < 10; i++) {
		pthread_join(threads[i], NULL);
	}

	std::cout << "---FINAL STATS---" << std::endl;

    std::cout << "CHECKING ACCOUNT" << std::endl;
    std::cout << "- DEPOSITS:    " << checking_account.no_deposits << std::endl;
    std::cout << "- WITHDRAWALS: " << checking_account.no_withdrawals << std::endl;
    std::cout << "- REJECTED:    " << checking_account.no_rejected << std::endl;
    std::cout << "- BALANCE:     " << checking_account.balance << std::endl << std::endl;

    std::cout << "SAVINGS ACCOUNT" << std::endl;
    std::cout << "- DEPOSITS:    " << savings_account.no_deposits << std::endl;
    std::cout << "- WITHDRAWALS: " << savings_account.no_withdrawals << std::endl;
    std::cout << "- REJECTED:    " << savings_account.no_rejected << std::endl;
    std::cout << "- BALANCE:     " << savings_account.balance << std::endl << std::endl;

	int check_deposit_sum = 0;
	int check_withdraw_sum = 0;
	int check_reject_sum = 0;
	int save_deposit_sum = 0;
	int save_withdraw_sum = 0;
	int save_reject_sum = 0;
	
	for (int i = 0; i < 10; i++) {
		check_deposit_sum += th_checking[i].no_deposits;
		check_withdraw_sum += th_checking[i].no_withdrawals;
		check_reject_sum += th_checking[i].no_rejected;
		save_deposit_sum += th_savings[i].no_deposits;
		save_withdraw_sum += th_savings[i].no_withdrawals;
		save_reject_sum += th_savings[i].no_rejected;
	}

	if (check_deposit_sum != checking_account.no_deposits)
		std::cout << "Race Condition: Check Deposit " << check_deposit_sum << " - " << checking_account.no_deposits << std::endl;

	if (check_withdraw_sum != checking_account.no_withdrawals)
		std::cout << "Race Condition: Check Withdrawals " << check_withdraw_sum << " - " << checking_account.no_withdrawals << std::endl;

	if (check_reject_sum != checking_account.no_rejected)
		std::cout << "Race Condition: Check Rejected " << check_reject_sum << " - " << checking_account.no_rejected << std::endl;

	if (save_deposit_sum != savings_account.no_deposits)
		std::cout << "Race Condition: Save Deposit " << save_deposit_sum << " - " << savings_account.no_deposits << std::endl;

	if (save_withdraw_sum != savings_account.no_withdrawals)
		std::cout << "Race Condition: Save Withdrawals " << save_withdraw_sum << " - " << savings_account.no_withdrawals << std::endl;

	if (save_reject_sum != savings_account.no_rejected)
		std::cout << "Race Condition: Save Rejected " << save_reject_sum << " - " << savings_account.no_rejected << std::endl;
}