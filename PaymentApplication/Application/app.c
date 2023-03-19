#include "app.h"

int main(void)
{
	/********** Card Module Test Functions **********/
	//getCardHolderNameTest();
	//getCardExpiryDateTest();
	//getCardPANTest();

	/********** Terminal Module Test Functions **********/
	//getTransactionDateTest();
	//isCardExpriedTest();
	//getTransactionAmountTest();
	//isBelowMaxAmountTest();
	//setMaxAmountTest();
	//isValidCardPANTest();

	/********** Server Module Test Functions **********/
	//isValidAccountTest();
	//isBlockedAccountTest();
	//isAmountAvailableTest();
	//recieveTransactionDataTest();
	//saveTransactionTest();
	//listSavedTransactionsTest();

	//Applicaation Main Loop
	while(1)
	{	
		// Start the payment application.
		appStart();
	}
}

void appStart(void)
{
	/*
	This function will initiate the payment application and guide the user through various steps, such as
	entering cardholder name, card expiry date, card PAN, transaction date, and transaction amount. It will also
	check the validity of the entered data and ensure that the transaction amount is within the 
	maximum allowed amount before saving the transaction.
	*/

	// Declare and initialize a ST_cardData_t structure variable and a pointer to it which are needed for the payment application.
	ST_cardData_t card0;
	ST_cardData_t* ptocard0 = &card0;

	// Call the 'getCardHolderName' function to prompt the user to enter the cardholder's name and validate the entered data.
	uint8_t getCardHolderNameResult = 1;
	do { getCardHolderNameResult = getCardHolderName(ptocard0); } while (getCardHolderNameResult != 0);

	// Call the 'getCardExpiryDate' function to prompt the user to enter the card expiry date and validate the entered data.
	uint8_t getCardExpiryDateResult = 2;
	do { getCardExpiryDateResult = getCardExpiryDate(ptocard0); } while (getCardExpiryDateResult != 0);


	// Call the 'getCardPAN' and 'isValidCardPAN' functions to prompt the user to enter the card PAN and validate the entered data.
	uint8_t getCardPANResult = 3;
	uint8_t isValidCardPANResult = 3;
	do 
	{
		getCardPANResult = getCardPAN(ptocard0); if (getCardPANResult == 0) { isValidCardPANResult = isValidCardPAN(ptocard0); }
	} while (getCardPANResult != 0 || isValidCardPANResult != 0);

	// Declare and initialize a ST_terminalData_t structure variable and a pointer to it which are needed for the payment application.
	ST_terminalData_t terminal0;
	ST_terminalData_t* ptoterminal0 = &terminal0;

	// Call the 'getTransactionDate' function to prompt the user to enter the transaction date and validate the entered data.
	uint8_t getTransactionDateResult = 1;
	do { getTransactionDateResult = getTransactionDate(ptoterminal0); } while (getTransactionDateResult != 0);
	

	// Call the 'isCardExpired' function to check if the card has expired.
	uint8_t isCardExpiredResult = 2;
	isCardExpiredResult = isCardExpired(ptocard0, ptoterminal0);
	if (isCardExpiredResult == 2) { printf("Application Ended.\n\n"); }	// If the card is expired, end the application.
	else if (isCardExpiredResult == 0)	// If the card is not expired, continue with the payment application.
	{
		setMaxAmount(ptoterminal0, 2500.00);	// Set the maximum transaction amount to 2500.00.
		printf("Maximum Transaction Amount: %0.2f\n", ptoterminal0->maxTransAmount);

		// Call the 'getTransactionAmount' function to prompt the user to enter the transaction amount and validate the entered data.
		uint8_t getTransactionAmountResult = 4;
		do { getTransactionAmountResult = getTransactionAmount(ptoterminal0); } while (getTransactionAmountResult != 0);

		// Call the 'isBelowMaxAmount' function to check if the transaction amount is below the maximum
		uint8_t isBelowMaxAmountResult = 5;
		isBelowMaxAmountResult = isBelowMaxAmount(ptoterminal0);

		// If transaction amount is not below the maximum transaction amount allowed, end the application.
		if (isBelowMaxAmountResult == 5) { printf("Application Ended.\n\n"); }
		// Otherwise, if the transaction amount is below the maximum transaction amount allowed, proceed with the transaction.
		else if (isBelowMaxAmountResult == 0)
		{
			// Declare and initialize a ST_transaction_t structure variable and a pointer to it which are needed for the payment application.
			ST_transaction_t transaction0;
			ST_transaction_t* ptotransaction0 = &transaction0;

			// Initialize the transaction's 'cardHolderData' and 'terminalData' members.
			ptotransaction0->cardHolderData = card0;
			ptotransaction0->terminalData = terminal0;

			// Save the transaction using the 'saveTransaction' function.
			saveTransaction(ptotransaction0);

			// Print a new line character for formatting purposes.
			printf("\n");
		}
	}
}