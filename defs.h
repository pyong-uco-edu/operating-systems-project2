#include <string>

const int BUF_SIZE = 5;

struct stats {
    int no_deposits; // # of deposits into the account (not include if rejected)
    int no_withdrawals; // # of withdrawals from the account (not include if rejected)
        // a transfer is counted as one withdrawal and one deposit to the corresponding accounts
    int no_rejected; // # of rejected transactions
    int balance;     // account balance
};

struct parameters {
	int loop_count;
	std::string filename;
    int index;
};