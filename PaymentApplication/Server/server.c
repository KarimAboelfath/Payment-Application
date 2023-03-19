#include "server.h"

//Global Vaariables

//Accounts Database with 10 accounts for testing.
ST_accountsDB_t accountsDB[255] = { {2000.0, RUNNING, "8989374615436851"},\
{100000.0, BLOCKED, "5807007076043875"}, {2000.0, RUNNING,"4929804463622139"}, {25000.0,BLOCKED,"4024007139922671"},\
{1000.0,RUNNING,"4929722653797141"},{50000.0,BLOCKED,"4716229724788543"},{3000.0,RUNNING,"4556737586899855"},\
{75000.0,BLOCKED,"6011000990139424"},{1500.0,RUNNING,"4485078142230976"},{125000.0,BLOCKED,"6011111111111117"} };

//Create the transactions global array and initialize it with zeros. 
ST_transaction_t transactions[255];
int nofBytes = 255 * sizeof(ST_transaction_t);
uint8_t zeroVar = 0;
memset(transactions, zeroVar, nofBytes);

uint32_t transactionSequenceNumber_counter = 1000;	//Initial sequence number
ST_accountsDB_t* PTR; //Global pointer to keep the address of the accountReference after the function call ends.
ST_accountsDB_t* ptoAccountRef = 0;
uint8_t transactionStateResult;

EN_transState_t recieveTransactionData(ST_transaction_t* transData)
{
	/*
	This function receives all transaction data and validates it by checking the account details and amount availability.
	It contains all server logic to process the transaction. If the account does not exist, it returns FRAUD_CARD. 
	If the amount is not available, it returns DECLINED_INSUFFICIENT_FUND. If the account is blocked, it returns DECLINED_STOLEN_CARD.
	If a transaction can't be saved, it returns INTERNAL_SERVER_ERROR. Otherwise, it returns APPROVED and 
	updates the database with the new balance.
	*/

	// Check if the account is valid.
	uint8_t res1 = isValidAccount(&(transData->cardHolderData), ptoAccountRef);
	ptoAccountRef = PTR;

	// If the account is invalid, return FRAUD_CARD.
	if (res1 == 3) { printf("Declined! Invalid Account.\n\n"); return FRAUD_CARD; }
	// If the account is valid, check if it's blocked.
	else if (res1 == 0)
	{
		uint8_t res2 = isBlockedAccount(ptoAccountRef);
		// If the account is blocked, return DECLINED_STOLEN_CARD.
		if (res2 == 5) { printf("Declined! Invalid Account.\n\n"); return DECLINED_STOLEN_CARD; }
		// If the account is not blocked, check if the amount is available.
		else if (res2 == 0)
		{
			uint8_t res3 = isAmountAvailable(&(transData->terminalData), ptoAccountRef);
			// If the amount is not available, return DECLINED_INSUFFICIENT_FUND.
			if (res3 == 4) { printf("Declined! Insufficient Funds.\n\n"); return DECLINED_INSUFFECIENT_FUND; }
			// If the amount is available, update the balance and return APPROVED.
			else if (res3 == 0)
			{
				//Update account balance and return APPROVED for the transsaction being successful.
				PTR->balance -= transData->terminalData.transAmount;
				printf("Successful Transaction.\n\n");
				return APPROVED; //Assuming that Saving will always succeed since the terminal is always connected to the server.
			}
		}
	}
}
EN_serverError_t isValidAccount(ST_cardData_t* cardData, ST_accountsDB_t* accountRefrence)
{
	/*
	This function takes in card data and validates if the account related to this card exists or not.
	It checks if the Primary Account Number (PAN) exists or not in the server's database (searches for the card PAN in the DB).
	If the PAN doesn't exist, the function returns ACCOUNT_NOT_FOUND and the account reference will be NULL.
	Otherwise, the function returns SERVER_OK and returns a reference to this account in the DB.
	@param cardData Pointer to the card data structure.
	@param accountReference Pointer to the accounts database structure, which will hold the reference to the account if it exists.
	@return SERVER_OK if the PAN exists in the accounts database, and a reference to the account is 
	successfully stored in the 'accountReference' parameter.
	ACCOUNT_NOT_FOUND if the PAN doesn't exist in the accounts database and the 'accountReference' parameter is NULL.
	*/

	// Declare and initialize an array to hold the Primary Account Number (PAN) from the card data.
	uint8_t primaryAccountNumber_Var[50] = { "" };

	// Copy the PAN from the card data into the 'primaryAccountNumber_Var' array.
	strcpy_s(primaryAccountNumber_Var, sizeof(primaryAccountNumber_Var), cardData->primaryAccountNumber);

	// Declare a variable to hold the result of the search for the PAN in the accounts database.
	int result = 1;

	// Loop through the accounts database and search for the PAN.
	for (int i = 0; i < 10; i++)
	{
		result = strcmp(accountsDB[i].primaryAccountNumber, primaryAccountNumber_Var);
		if (result == 0)
		{
			// If the PAN is found in the accounts database, store a reference to the account in the 'accountReference' parameter.
			accountRefrence = &accountsDB[i];
			// Also store a global reference to the account in the 'PTR' variable.
			PTR = &accountsDB[i];
			// Return SERVER_OK to indicate that the PAN exists in the accounts database and a reference
			//to the account is stored in the 'accountReference' parameter.
			return SERVER_OK;
		}
	}

	// If the PAN is not found in the accounts database, set the 'accountReference' parameter to NULL and return ACCOUNT_NOT_FOUND.
	if (result != 0) { accountRefrence = 0; return ACCOUNT_NOT_FOUND; }
}
EN_serverError_t isBlockedAccount(ST_accountsDB_t* accountRefrence)
{
	/*
	This function takes a reference to an account in the database and checks if the account is blocked or not. 
	If the account is active, the function returns SERVER_OK, indicating that the account is running. 
	Otherwise, if the account is blocked, the function returns BLOCKED_ACCOUNT.
	@param accountReference A pointer to the account data structure.
	@return SERVER_OK if the account is active, BLOCKED_ACCOUNT if the account is blocked.
	*/

	// Check if the account reference is null.
	if (accountRefrence == 0) { return BLOCKED_ACCOUNT; }
	else
	{
		// Check if the account is blocked or not.
		if (accountRefrence->state == 1) { return BLOCKED_ACCOUNT; }
		else if (accountRefrence->state == 0) { return SERVER_OK; }
	}
}
EN_serverError_t isAmountAvailable(ST_terminalData_t* termData, ST_accountsDB_t* accountRefrence)
{
	/*
	This function takes terminal data and a reference to the account in the database and checks if 
	the account has a sufficient amount to withdraw or not.
	It checks if the transaction's amount is available or not.
	If the transaction amount is greater than the balance in the database, it will return LOW_BALANCE,
	otherwise, it will return SERVER_OK.
	@param termData Pointer to the terminal data structure.
	@param accountReference Pointer to the account in the database.
	@return LOW_BALANCE if the transaction amount is greater than the balance in the database, otherwise SERVER_OK.
	*/

	// Store the transaction amount and the account balance into separate variables for easier comparison.
	float transAmount_Var = termData->transAmount;
	float balance = accountRefrence->balance;

	// Check if the transaction amount is greater than the account balance in the database.
	// If it is, return LOW_BALANCE error code, otherwise return SERVER_OK.
	if (transAmount_Var > balance) { return LOW_BALANCE; }
	else { return SERVER_OK; }
}
EN_serverError_t saveTransaction(ST_transaction_t* transData)
{
	/*
	This function will store all transaction data in the transactions database.
	It gives a sequence number to a transaction, this number is incremented once a transaction is 
	processed into the server, you must check the last sequence number in the server to give the new 
	transaction a new sequence number.
	It saves any type of transaction, including APPROVED, DECLINED_INSUFFECIENT_FUND, DECLINED_STOLEN_CARD,
	FRAUD_CARD, INTERNAL_SERVER_ERROR.
	It will list all saved transactions using the listSavedTransactions function.
	Assuming that the connection between the terminal and server is always connected, then it will return SERVER_OK.
	@param transData Pointer to the transaction data structure.
	@return SERVER_OK to indicate that the transaction was successfully saved in the transactions database.
	*/

	// Receive transaction data and update the transaction state.
	transactionStateResult = recieveTransactionData(transData);
	transData->transState = transactionStateResult;

	// Give a sequence number to the transaction 
	transData->transactionSequenceNumber = transactionSequenceNumber_counter;

	// Store the transaction data in the transactions database.
	uint32_t transactionIndex = transactionSequenceNumber_counter - 1000;
	transactions[transactionIndex] = *transData;

	// Update the transaction sequence number counter.
	transactionSequenceNumber_counter++;

	// List all saved transactions using the listSavedTransactions function.
	listSavedTransactions();

	// Return SERVER_OK to indicate that the transaction was successfully saved in the transactions database.
	return SERVER_OK;
}
void listSavedTransactions(void)
{
	/*
	This function prints all transactions found in the transactions database.
	The function iterates over the 'transactions' array and prints the transaction details for each 
	transaction that has a sequence number greater than or equal to 1000.
	The function uses a string array to store the transaction state names and prints the corresponding 
	state name for each transaction.
	*/

	// Define an array of string literals to hold the transaction state names.
	const char* transStateNames[] = { "APPROVED", "DECLINED_INSUFFECIENT_FUND", "DECLINED_STOLEN_CARD", "FRAUD_CARD", "INTERNAL_SERVER_ERROR" };
	
	// Iterate over the 'transactions' array and print the details for each transaction with a sequence 
	//number greater than or equal to 1000.
	for (int i = 0; i < 255; i++)
	{
		if (transactions[i].transactionSequenceNumber < 1000) { break; }
		else
		{
			printf("#########################\n");
			printf("Transaction Sequence Number : %d\n",    transactions[i].transactionSequenceNumber);
			printf("Transaction Date            : %s\n",    transactions[i].terminalData.transactionDate);
			printf("Transaction Amount          : %0.2f\n", transactions[i].terminalData.transAmount);
			printf("Transaction State           : %s\n",    transStateNames[transactions[i].transState]);
			printf("Terminal Max Amount         : %0.2f\n", transactions[i].terminalData.maxTransAmount);
			printf("Cardholder Name             : %s\n",    transactions[i].cardHolderData.cardHolderName);
			printf("PAN                         : %s\n",    transactions[i].cardHolderData.primaryAccountNumber);
			printf("Card Expiration Date        : %s\n",    transactions[i].cardHolderData.cardExpirationDate);
			if (transactionStateResult == 0) { printf("Balance After Transaction   : %0.2f\n", PTR->balance); }
		}
	}

	// Print a separator after all transactions have been printed.
	printf("#########################\n");
}

void recieveTransactionDataTest(void)
{
	printf("Tester Name: Karim Nasr Aboelfath Abdelfattah\n");
	printf("Function Name : recieveTransactionData\n\n");
	
	ST_transaction_t test1;
	strcpy_s(test1.cardHolderData.primaryAccountNumber, sizeof(test1.cardHolderData.primaryAccountNumber), "1234567890123456");
	test1.terminalData.transAmount = 100.00;
	ST_transaction_t* ptotest1 = &test1;
	
	printf("Test Case 1: Invalid Card (Account Not in Database)\n");
	printf("Test Account PAN: 1234567890123456\n"); int res1 = recieveTransactionData(ptotest1);
	printf("Expected Result: 3 for FRAUD_CARD\n");
	printf("Actual Result: %d\n\n", res1);

	ST_transaction_t test2;
	strcpy_s(test2.cardHolderData.primaryAccountNumber, sizeof(test2.cardHolderData.primaryAccountNumber), "5807007076043875");
	test2.terminalData.transAmount = 300.00;
	ST_transaction_t* ptotest2 = &test2;

	printf("Test Case 2: BLOCKED Account\n");
	printf("Test Account PAN: 5807007076043875\n"); int res2 = recieveTransactionData(ptotest2);
	printf("Expected Result: 2 for DECLINED_STOLEN_CARD\n");
	printf("Actual Result: %d\n\n", res2);

	ST_transaction_t test3;
	strcpy_s(test3.cardHolderData.primaryAccountNumber, sizeof(test3.cardHolderData.primaryAccountNumber), "4532811690128502");
	test3.terminalData.transAmount = 1400.00;
	ST_transaction_t* ptotest3 = &test3;

	printf("Test Case 3: Not Enough Balance in Account to Process Transaction\n");
	printf("Test Account PAN: 4532811690128502\n"); int res3 = recieveTransactionData(ptotest3);
	printf("Expected Result: 1 for DECLINED_INSUFFECIENT_FUND\n");
	printf("Actual Result: %d\n\n", res3);

	ST_transaction_t test4;
	strcpy_s(test4.cardHolderData.primaryAccountNumber, sizeof(test4.cardHolderData.primaryAccountNumber), "4929804463622139");
	test4.terminalData.transAmount = 200.00;
	ST_transaction_t* ptotest4 = &test4;

	printf("Test Case 4: Successful Transaction\n");
	printf("Test Account PAN: 4929804463622139\n"); int res4 = recieveTransactionData(ptotest4);
	printf("Expected Result: 0 for APPROVED\n");
	printf("Actual Result: %d\n\n", res4);
}
void isValidAccountTest(void)
{
	printf("Tester Name: Karim Nasr Aboelfath Abdelfattah\n");
	printf("Function Name : isValidAccount\n\n");

	ST_cardData_t card1;
	strcpy_s(card1.primaryAccountNumber, sizeof(card1.primaryAccountNumber), "1234567890123456");
	ST_cardData_t* ptocard1 = &card1;
	ST_accountsDB_t* ptoAccountRefTest1 = 0;

	printf("Test Case 1: Account not in Database\n");
	printf("Test Account PAN: 1234567890123456\n"); int res1 = isValidAccount(ptocard1, ptoAccountRefTest1);
	ptoAccountRefTest1 = PTR;
	printf("Expected Result: 3 for ACCOUNT_NOT_FOUND\n");
	printf("Actual Result: %d\n", res1);
	printf("Expected Account Reference: 0\n");
	printf("Actual Account Reference: %p\n\n", ptoAccountRefTest1);

	ST_cardData_t card2;
	strcpy_s(card2.primaryAccountNumber, sizeof(card2.primaryAccountNumber), "4485078142230976");
	ST_cardData_t* ptocard2 = &card2;
	ST_accountsDB_t* ptoAccountRefTest2 = 0;

	printf("Test Case 2: Account in Database\n");
	printf("Test Account PAN: 4485078142230976\n"); int res2 = isValidAccount(ptocard2, ptoAccountRefTest2);
	ptoAccountRefTest2 = PTR;
	printf("Expected Result: 0 for SERVER_OK\n");
	printf("Actual Result: %d\n", res2);
	printf("Expected Account Reference: Non-Zero\n");
	printf("Actual Account Reference: %p\n\n", ptoAccountRefTest2);
}
void isBlockedAccountTest(void)
{
	printf("Tester Name: Karim Nasr Aboelfath Abdelfattah\n");
	printf("Function Name : isBlockedAccount\n\n");

	ST_cardData_t card1;
	strcpy_s(card1.primaryAccountNumber, sizeof(card1.primaryAccountNumber), "5807007076043875");
	ST_cardData_t* ptocard1 = &card1;
	ST_accountsDB_t* ptoAccountRefTest1 = 0;

	printf("Test Case 1: BLOCKED Account\n");
	printf("Test Account PAN: 5807007076043875\n"); int res1 = isValidAccount(ptocard1, ptoAccountRefTest1);
	ptoAccountRefTest1 = PTR;
	int res11 = isBlockedAccount(ptoAccountRefTest1);
	printf("Expected Result: 5 for BLOCKED_ACCOUNT\n");
	printf("Actual Result: %d\n", res11);
	printf("Expected Account Reference: Non-Zero\n");
	printf("Actual Account Reference: %p\n\n", ptoAccountRefTest1);

	ST_cardData_t card2;
	strcpy_s(card2.primaryAccountNumber, sizeof(card2.primaryAccountNumber), "4532811690128502");
	ST_cardData_t* ptocard2 = &card2;
	ST_accountsDB_t* ptoAccountRefTest2 = 0;

	printf("Test Case 2: RUNNING Account\n");
	printf("Test Account PAN: 4532811690128502\n"); int res2 = isValidAccount(ptocard2, ptoAccountRefTest2);
	ptoAccountRefTest2 = PTR;
	int res22 = isBlockedAccount(ptoAccountRefTest2);
	printf("Expected Result: 0 for SERVER_OK\n");
	printf("Actual Result: %d\n", res22);
	printf("Expected Account Reference: Non-Zero\n");
	printf("Actual Account Reference: %p\n\n", ptoAccountRefTest2);
}
void isAmountAvailableTest(void)
{
	printf("Tester Name: Karim Nasr Aboelfath Abdelfattah\n");
	printf("Function Name : isAmountAvailable\n\n");

	ST_cardData_t card1;
	strcpy_s(card1.primaryAccountNumber, sizeof(card1.primaryAccountNumber), "4532811690128502");
	ST_cardData_t* ptocard1 = &card1;
	ST_accountsDB_t* ptoAccountRefTest1 = 0;

	ST_terminalData_t terminal1;
	terminal1.transAmount = 1800.0;
	ST_terminalData_t* ptoterminal1 = &terminal1;

	printf("Test Case 1: Transaction Amount Higher Than Balance (Insufficient Funds)\n");
	printf("Test Account PAN: 4532811690128502\n"); int res1 = isValidAccount(ptocard1, ptoAccountRefTest1);
	ptoAccountRefTest1 = PTR;
	int res11 = isAmountAvailable(ptoterminal1, ptoAccountRefTest1);
	printf("Balance: %0.2f\nTransaction Amount: %0.2f\n", 1000.00, terminal1.transAmount);
	printf("Expected Result: 4 for LOW_BALANCE\n");
	printf("Actual Result: %d\n", res11);
	printf("Expected Account Reference: Non-Zero\n");
	printf("Actual Account Reference: %p\n\n", ptoAccountRefTest1);

	ST_cardData_t card2;
	strcpy_s(card2.primaryAccountNumber, sizeof(card2.primaryAccountNumber), "4556737586899855");
	ST_cardData_t* ptocard2 = &card2;
	ST_accountsDB_t* ptoAccountRefTest2 = 0;

	ST_terminalData_t terminal2;
	terminal2.transAmount = 2500.0;
	ST_terminalData_t* ptoterminal2 = &terminal2;

	printf("Test Case 2: Transaction Amount Lower Than Balance (Enough Balance)\n");
	printf("Test Account PAN: 4556737586899855\n"); int res2 = isValidAccount(ptocard2, ptoAccountRefTest2);
	ptoAccountRefTest2 = PTR;
	int res22 = isAmountAvailable(ptoterminal2, ptoAccountRefTest2);
	printf("Balance: %0.2f\nTransaction Amount: %0.2f\n", 3000.00, terminal2.transAmount);
	printf("Expected Result: 0 for SERVER_OK\n");
	printf("Actual Result: %d\n", res22);
	printf("Expected Account Reference: Non-Zero\n");
	printf("Actual Account Reference: %p\n\n", ptoAccountRefTest2);
}
void saveTransactionTest(void)
{
	printf("Tester Name: Karim Nasr Aboelfath Abdelfattah\n");
	printf("Function Name : saveTransaction and listSavedTransactions\n\n");

	// Create a variable of ST_transaction_t type
	ST_transaction_t test1;
	// Fill in the cardHolderData struct
	strcpy_s(test1.cardHolderData.cardHolderName, sizeof(test1.cardHolderData.cardHolderName), "John Smith");
	strcpy_s(test1.cardHolderData.primaryAccountNumber, sizeof(test1.cardHolderData.primaryAccountNumber), "1234567890123456");
	strcpy_s(test1.cardHolderData.cardExpirationDate, sizeof(test1.cardHolderData.cardExpirationDate), "12/25");
	// Fill in the terminalData struct
	test1.terminalData.transAmount = 100.00;
	test1.terminalData.maxTransAmount = 500.00;
	strcpy_s(test1.terminalData.transactionDate, sizeof(test1.terminalData.transactionDate), "02/28/2023");
	ST_transaction_t* ptotest1 = &test1;

	printf("Test Case 1: \n");
	int res1 = saveTransaction(ptotest1);
	printf("Expected Result: 0 for SERVER_OK for Saving Transaction Successfully\n");
	printf("Actual Result: %d\n\n", res1);

	printf("Tester Name: Karim Nasr Aboelfath Abdelfattah\n");
	printf("Function Name : saveTransaction and listSavedTransactions\n\n");
	
	// Create another variable of ST_transaction_t type
	ST_transaction_t test2;
	// Fill in the cardHolderData struct
	strcpy_s(test2.cardHolderData.cardHolderName, sizeof(test2.cardHolderData.cardHolderName), "Bob Jones");
	strcpy_s(test2.cardHolderData.primaryAccountNumber, sizeof(test2.cardHolderData.primaryAccountNumber), "5807007076043875");
	strcpy_s(test2.cardHolderData.cardExpirationDate, sizeof(test2.cardHolderData.cardExpirationDate), "09/23");
	// Fill in the terminalData struct
	test2.terminalData.transAmount = 300.00;
	test2.terminalData.maxTransAmount = 600.00;
	strcpy_s(test2.terminalData.transactionDate, sizeof(test2.terminalData.transactionDate), "02/28/2023");
	ST_transaction_t* ptotest2 = &test2;

	printf("Test Case 2: \n");
	int res2 = saveTransaction(ptotest2);
	printf("Expected Result: 0 for SERVER_OK for Saving Transaction Successfully\n");
	printf("Actual Result: %d\n\n", res2);

	printf("Tester Name: Karim Nasr Aboelfath Abdelfattah\n");
	printf("Function Name : saveTransaction and listSavedTransactions\n\n");

	// Create another variable of ST_transaction_t type
	ST_transaction_t test3;
	// Fill in the cardHolderData struct
	strcpy_s(test3.cardHolderData.cardHolderName, sizeof(test3.cardHolderData.cardHolderName), "Mary Trevor");
	strcpy_s(test3.cardHolderData.primaryAccountNumber, sizeof(test3.cardHolderData.primaryAccountNumber), "4532811690128502");
	strcpy_s(test3.cardHolderData.cardExpirationDate, sizeof(test3.cardHolderData.cardExpirationDate), "03/22");
	// Fill in the terminalData struct
	test3.terminalData.transAmount = 1400.00;
	test3.terminalData.maxTransAmount = 1700.00;
	strcpy_s(test3.terminalData.transactionDate, sizeof(test3.terminalData.transactionDate), "02/28/2023");
	ST_transaction_t* ptotest3 = &test3;

	printf("Test Case 3: \n");
	int res3 = saveTransaction(ptotest3);
	printf("Expected Result: 0 for SERVER_OK for Saving Transaction Successfully\n");
	printf("Actual Result: %d\n\n", res3);

	printf("Tester Name: Karim Nasr Aboelfath Abdelfattah\n");
	printf("Function Name : saveTransaction and listSavedTransactions\n\n");

	// Create another variable of ST_transaction_t type
	ST_transaction_t test4;
	// Fill in the cardHolderData struct
	strcpy_s(test4.cardHolderData.cardHolderName, sizeof(test4.cardHolderData.cardHolderName), "Alice Lee");
	strcpy_s(test4.cardHolderData.primaryAccountNumber, sizeof(test4.cardHolderData.primaryAccountNumber), "4929804463622139");
	strcpy_s(test4.cardHolderData.cardExpirationDate, sizeof(test4.cardHolderData.cardExpirationDate), "06/24");
	// Fill in the terminalData struct
	test4.terminalData.transAmount = 200.00;
	test4.terminalData.maxTransAmount = 400.00;
	strcpy_s(test4.terminalData.transactionDate, sizeof(test4.terminalData.transactionDate), "02/28/2023");
	ST_transaction_t* ptotest4 = &test4;

	printf("Test Case 4: \n");
	int res4 = saveTransaction(ptotest4);
	printf("Expected Result: 0 for SERVER_OK for Saving Transaction Successfully\n");
	printf("Actual Result: %d\n\n", res4);
}
void listSavedTransactionsTest(void)
{
	//listSavedTransactions is called and tested within saveTransaction function
	saveTransactionTest();
}