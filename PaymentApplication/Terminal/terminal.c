#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../Terminal/terminal.h"

EN_terminalError_t getTransactionDate(ST_terminalData_t *termData)
{
	/*
	This function prompts the user to enter the transaction date and stores it into the 'transactionDate'
	member of the 'termData' structure.
	The function checks if the entered date is valid (i.e., 10 characters in the format DD/MM/YYYY) and returns
	an appropriate error message if not.
	@param termData Pointer to the terminal data structure.
	@return TERMINAL_OK if the transaction date is valid and successfully stored in the 'transactionDate' member
	of the 'termData' structure.
	*/

	// Declare and initialize an array to hold the user's input for the transaction date.
	uint8_t transactionDate_Var[50] = { "" };

	// Prompt the user to enter the transaction date and read the input into the 'transactionDate_Var' array.
	printf("Insert Transaction Date: ");
	gets(transactionDate_Var);

	// Check if the entered date is valid, i.e., not NULL, exactly 10 characters long and in the format DD/MM/YYYY.
	if (strlen(transactionDate_Var) == 0 || strlen(transactionDate_Var) != 10) { printf("Invalid Transaction Date! Please try again.\n\n"); return WRONG_DATE; }
	for (int i = 0; i < strlen(transactionDate_Var); i++)
	{
		if (i == 0)
		{
			uint8_t e0 = transactionDate_Var[0];
			if (e0 != '0' && e0 != '1' && e0 != '2' && e0 != '3') { printf("Invalid Transaction Date! Please try again.\n\n"); return WRONG_DATE; }
		}
		if (i == 1)
		{
			int isD1 = isdigit(transactionDate_Var[1]);
			if (!isD1) { printf("Invalid Transaction Date! Please try again.\n\n"); return WRONG_DATE; }
			if (transactionDate_Var[0] == '0' && transactionDate_Var[1] == '0') { printf("Invalid Transaction Date! Please try again.\n\n"); return WRONG_DATE; }
			uint8_t e1 = transactionDate_Var[1];
			int ie1 = e1 - '0';
			if (transactionDate_Var[0] == '3' && ie1 > 1) { printf("Invalid Transaction Date! Please try again.\n\n"); return WRONG_DATE; }
		}
		if (i == 2 && (transactionDate_Var[2] != '/')) { printf("Invalid Transaction Date! Please try again.\n\n"); return WRONG_DATE; }
		if (i == 3 && (transactionDate_Var[3] != '0' && transactionDate_Var[3] != '1')) { printf("Invalid Transaction Date! Please try again.\n\n"); return WRONG_DATE; }
		if (i == 4)
		{
			uint8_t x4 = transactionDate_Var[4];
			int isnD1 = !(isdigit(x4));
			if (isnD1) { printf("Invalid Transaction Date! Please try again.\n\n"); return WRONG_DATE; }
			uint8_t x3 = transactionDate_Var[3];
			if (x3 == '1') { if (x4 != '0' && x4 != '1' && x4 != '2') { printf("Invalid Transaction Date! Please try again.\n\n"); return WRONG_DATE; } }
			if (x3 == '0' && x4 == '0') { printf("Invalid Transaction Date! Please try again.\n\n"); return WRONG_DATE; }
		}
		if (i == 5 && (transactionDate_Var[5] != '/')) { printf("Invalid Transaction Date! Please try again.\n\n"); return WRONG_DATE; }
		int isnD2 = !(isdigit(transactionDate_Var[i]));
		if (i == 6 && isnD2) { printf("Invalid Transaction Date! Please try again.\n\n"); return WRONG_DATE; }
		int isnD3 = !(isdigit(transactionDate_Var[i]));
		if (i == 7 && isnD3) { printf("Invalid Transaction Date! Please try again.\n\n"); return WRONG_DATE; }
		int isnD4 = !(isdigit(transactionDate_Var[i]));
		if (i == 8 && isnD4) { printf("Invalid Transaction Date! Please try again.\n\n"); return WRONG_DATE; }
		int isnD5 = !(isdigit(transactionDate_Var[i]));
		if (i == 9 && isnD5) { printf("Invalid Transaction Date! Please try again.\n\n"); return WRONG_DATE; }
	}

	// If the transaction date is valid, copy it into the 'transactionDate' member of the 'termData' structure.
	strcpy_s(termData->transactionDate, sizeof(termData->transactionDate), transactionDate_Var);

	// Return TERMINAL_OK to indicate that the transaction date was successfully stored in the 'transactionDate'
	//member of the 'termData' structure.
	return TERMINAL_OK;
}
EN_terminalError_t isCardExpired(ST_cardData_t *cardData, ST_terminalData_t *termData)
{
	/*
	This function compares the card expiry date with the transaction date. If the card expiration date
	is before the transaction date, it returns EXPIRED_CARD; otherwise, it returns TERMINAL_OK.
	@param cardData Pointer to the card data structure.
	@param termData Pointer to the terminal data structure.
	@return EXPIRED_CARD if the card is expired or TERMINAL_OK if it's not expired.
	*/

	// Copy the card's expiry date and transaction date into new arrays.
	uint8_t expDate[6], trnDate[11];
	strcpy_s(expDate, sizeof(expDate), cardData->cardExpirationDate);
	strcpy_s(trnDate, sizeof(trnDate), termData->transactionDate);

	// Extract the year from the expiry date and transaction date.
	uint8_t expYear[50] = { "20" };
	for (int i = 3; i <= 4; i++)
	{
		strncat_s(expYear, sizeof(expYear), &expDate[i], sizeof(expDate[i]));
	}
	uint8_t trnYear[50] = { "" };
	for (int i = 6; i <= 9; i++)
	{
		strncat_s(trnYear, sizeof(trnYear), &trnDate[i], sizeof(trnDate[i]));
	}

	// Compare the years. If the transaction year is greater than the expiry year, the card is expired.
	if (atoi(trnYear) > atoi(expYear)) { printf("Declined! Expired Card.\n\n"); return EXPIRED_CARD; }
	// If the transaction year is the same as the expiry year, extract the month from both dates and compare them.
	else if (atoi(trnYear) == atoi(expYear))
	{
		uint8_t expMonth[50] = { "" };
		uint8_t trnMonth[50] = { "" };
		for (int i = 0; i <= 1; i++)
		{
			strncat_s(expMonth, sizeof(expMonth), &expDate[i], sizeof(expDate[i]));
		}
		for (int i = 3; i <= 4; i++)
		{
			strncat_s(trnMonth, sizeof(trnMonth), &trnDate[i], sizeof(trnDate[i]));
		}
		// If the transaction month is greater than the expiry month, the card is expired.
		if (atoi(trnMonth) > atoi(expMonth)) { printf("Declined! Expired Card.\n\n"); return EXPIRED_CARD; }
		// Otherwise, the card is still valid.
		else { return TERMINAL_OK; }
	}
	// If the transaction year is less than the expiry year, the card is still valid.
	else { return TERMINAL_OK; }
}
EN_terminalError_t getTransactionAmount(ST_terminalData_t *termData)
{
	/*
	This function prompts the user to enter the transaction amount and stores it into the 'transAmount'
	member of the 'termData' structure.
	The function checks if the entered amount is valid (i.e., greater than 0) and returns an appropriate
	error message if not.
	@param termData Pointer to the terminal data structure.
	@return TERMINAL_OK if the transaction amount is valid and successfully stored in the 'transAmount'
	member of the 'termData' structure.
	*/

	// Declare and initialize a variable to hold the user's input for the transaction amount.
	float transAmount_var = 0.0;

	// Prompt the user to enter the transaction amount and read the input into the 'transAmount_var' variable.
	printf("Insert Transaction Amount: ");
	int result = scanf_s("%f", &transAmount_var);

	// Check if the entered amount is valid, i.e., greater than 0.
	if (result == 0) 
	{
		// If the input is invalid, clear the input buffer and return an appropriate error message.
		int ch = getchar();
		while (ch != '\n' && ch != EOF) { ch = getchar(); }
		printf("Invalid Transaction Amount! Please try again.\n\n");
		return INVALID_AMOUNT;
	}
	else if (transAmount_var <= 0) { getchar(); printf("Invalid Transaction Amount! Please try again.\n\n"); return INVALID_AMOUNT; }
	else
	{ 
		// If the input is valid, store the transaction amount in the 'transAmount' member of the 'termData' structure.
		getchar();
		termData->transAmount = transAmount_var;

		// Return TERMINAL_OK to indicate that the transaction amount was successfully stored in the 'transAmount' 
		//member of the 'termData' structure.
		return TERMINAL_OK;
	}
}
EN_terminalError_t isBelowMaxAmount(ST_terminalData_t *termData)
{
	/*
	This function compares the transaction amount with the terminal max allowed amount.
	If the transaction amount is larger than the terminal max allowed amount, the function returns EXCEED_MAX_AMOUNT.
	Otherwise, it returns TERMINAL_OK.
	@param termData Pointer to the terminal data structure.
	@return EXCEED_MAX_AMOUNT if the transaction amount is larger than the terminal max allowed amount.
	Otherwise, it returns TERMINAL_OK.
	*/

	// Compare the transaction amount with the terminal max allowed amount.
	// If the transaction amount is larger than the terminal max allowed amount, return EXCEED_MAX_AMOUNT.
	if (termData->transAmount > termData->maxTransAmount){ printf("Declined! Amount Exceeding Limit.\n\n"); return EXCEED_MAX_AMOUNT; }
	// Otherwise, return TERMINAL_OK.
	else{ return TERMINAL_OK; }
}
EN_terminalError_t setMaxAmount(ST_terminalData_t *termData, float maxAmount)
{
	/*
	This function takes the maximum allowed amount and stores it into terminal data.
	Transaction max amount is a float number.
	If the transaction max amount is less than or equal to 0, the function returns the INVALID_MAX_AMOUNT error.
	If the transaction max amount is valid, it is stored in the 'maxTransAmount' member of the 'termData' 
	structure, and the function returns TERMINAL_OK.
	@param termData Pointer to the terminal data structure.
	@param maxAmount The maximum transaction amount to be stored in the 'maxTransAmount' member of the 'termData' structure.
	@return INVALID_MAX_AMOUNT if the maximum transaction amount is less than or equal to 0.
	TERMINAL_OK if the maximum transaction amount is valid and successfully stored in the 'maxTransAmount'
	member of the 'termData' structure.
	*/

	// Check if the transaction max amount is less than or equal to 0.
	// If the transaction max amount is less than or equal to 0, return the INVALID_MAX_AMOUNT error.
	if (maxAmount <= 0) { return INVALID_MAX_AMOUNT; }
	else
	{
		// If the transaction max amount is valid, store it in the 'maxTransAmount' member of the 'termData' structure.
		termData->maxTransAmount = maxAmount;

		// Return TERMINAL_OK to indicate that the maximum transaction amount was successfully stored in the 'maxTransAmount'
		//member of the 'termData' structure.
		return TERMINAL_OK;
	}
}
EN_terminalError_t isValidCardPAN(ST_cardData_t *cardData) // Optional
{
	/*
	This function checks whether the Primary Account Number (PAN) provided is a Luhn number or not.
	If the PAN is not a Luhn number, it returns INVALID_CARD, else it returns TERMINAL_OK.
	*/

	// Declare and initialize an array to hold the Primary Account Number (PAN) value and a pointer to it.
	uint8_t primaryAccountNumber_Var[20];
	uint8_t* ptoprimaryAccountNumber_Var = primaryAccountNumber_Var;

	// Copy the PAN value from the cardData structure to the primaryAccountNumber_Var array.
	strcpy_s(primaryAccountNumber_Var, sizeof(primaryAccountNumber_Var), cardData->primaryAccountNumber);

	// Determine the number of digits in the PAN value.
	int count = 0;
	unsigned long long intofprimaryAccountNumber_Var = strtoull(primaryAccountNumber_Var, NULL, 10); // Convert the string to an unsigned long long
	long long int temp_num = intofprimaryAccountNumber_Var;
	while (temp_num > 0) 
	{
		count++;
		temp_num /= 10LL;
	}

	// Applying the Luhn algorithm to calculate the sum of digits
	int sum = 0;
	temp_num = intofprimaryAccountNumber_Var;
	for (int i = 1; i <= count; i++) 
	{
		int digit = temp_num % 10LL; // Getting the rightmost digit
		if (i % 2 == 0) // If it is an even position from right, Multiply it by 2
		{ 
			digit *= 2;
			if (digit > 9){ digit -= 9; } // If it is a two-digit number, Subtract 9 from it
		}
		sum += digit; // Add it to the sum
		temp_num /= 10LL; // Remove the rightmost digit
	}

	// Checking the validity of the number according to Luhn's algorithm
	if (sum % 10 != 0) { printf("Invalid Primary Account Number (Not a LUHN Number)! Please try again.\n\n"); return INVALID_CARD; } // If the sum is not divisible by 10 

	// Return TERMINAL_OK to indicate that the PAN is a valid Luhn number.
	return TERMINAL_OK;
}

void getTransactionDateTest(void)
{
	printf("Tester Name: Karim Nasr Aboelfath Abdelfattah\n");
	printf("Function Name : getTransactionDate\n\n");

	ST_terminalData_t Terminal1;
	ST_terminalData_t *ptoTerminalData = &Terminal1;

	printf("Test Case 1: Date Too Short\n");
	printf("Input Data: "); int res1 = getTransactionDate(ptoTerminalData);
	printf("Expected Result: 1 for WRONG_DATE\n");
	printf("Actual Result: %d\n\n", res1);

	printf("Test Case 2: Date Too Long\n");
	printf("Input Data: "); int res2 = getTransactionDate(ptoTerminalData);
	printf("Expected Result: 1 for WRONG_DATE\n");
	printf("Actual Result: %d\n\n", res2);

	printf("Test Case 3: Date All Spaces\n");
	printf("Input Data: "); int res3 = getTransactionDate(ptoTerminalData);
	printf("Expected Result: 1 for WRONG_DATE\n");
	printf("Actual Result: %d\n\n", res3);

	printf("Test Case 4: Empty Date\n");
	printf("Input Data: "); int res4 = getTransactionDate(ptoTerminalData);
	printf("Expected Result: 1 for WRONG_DATE\n");
	printf("Actual Result: %d\n\n", res4);

	printf("Test Case 5: Invalid Month 1\n");
	printf("Input Data: "); int res5 = getTransactionDate(ptoTerminalData);
	printf("Expected Result: 1 for WRONG_DATE\n");
	printf("Actual Result: %d\n\n", res5);

	printf("Test Case 6: Invalid Month 2\n");
	printf("Input Data: "); int res6 = getTransactionDate(ptoTerminalData);
	printf("Expected Result: 1 for WRONG_DATE\n");
	printf("Actual Result: %d\n\n", res6);

	printf("Test Case 7: Invalid Month 3\n");
	printf("Input Data: "); int res7 = getTransactionDate(ptoTerminalData);
	printf("Expected Result: 1 for WRONG_DATE\n");
	printf("Actual Result: %d\n\n", res7);

	printf("Test Case 8: Invalid Date Separator\n");
	printf("Input Data: "); int res8 = getTransactionDate(ptoTerminalData);
	printf("Expected Result: 1 for WRONG_DATE\n");
	printf("Actual Result: %d\n\n", res8);

	printf("Test Case 9: Invalid Year\n");
	printf("Input Data: "); int res9 = getTransactionDate(ptoTerminalData);
	printf("Expected Result: 1 for WRONG_DATE\n");
	printf("Actual Result: %d\n\n", res9);

	printf("Test Case 10: Valid Date Format\n");
	printf("Input Data: "); int res10 = getTransactionDate(ptoTerminalData);
	printf("Expected Result: 0 for TERMINAL_OK\n");
	printf("Actual Result: %d\n\n", res10);
}
void isCardExpriedTest(void)
{
	printf("Tester Name: Karim Nasr Aboelfath Abdelfattah\n");
	printf("Function Name : isCardExpired\n\n");

	printf("Test Case 1: Transaction Year After Expiry Year\n");
	ST_cardData_t Card1;
	ST_cardData_t *ptoCardData1 = &Card1;
	printf("Expiry Date: ");  getCardExpiryDate(ptoCardData1);
	ST_terminalData_t Terminal1;
	ST_terminalData_t *ptoTerminalData1 = &Terminal1;
	printf("Transaction Date: ");  getTransactionDate(ptoTerminalData1);
	int res1 = isCardExpired(ptoCardData1, ptoTerminalData1);
	printf("Expected Result: 2 for EXPIRED_CARD\n");
	printf("Actual Result: %d\n\n", res1);


	printf("Test Case 2: Same Year, Transaction Month After Expiry Month\n");
	ST_cardData_t Card2;
	ST_cardData_t *ptoCardData2 = &Card2;
	printf("Expiry Date: ");  getCardExpiryDate(ptoCardData2);
	ST_terminalData_t Terminal2;
	ST_terminalData_t *ptoTerminalData2 = &Terminal2;
	printf("Transaction Date: "); getTransactionDate(ptoTerminalData2);
	int res2 = isCardExpired(ptoCardData2, ptoTerminalData2);
	printf("Expected Result: 2 for EXPIRED_CARD\n");
	printf("Actual Result: %d\n\n", res2);


	printf("Test Case 3: Same Year, Same Month\n");
	ST_cardData_t Card3;
	ST_cardData_t *ptoCardData3 = &Card3;
	printf("Expiry Date: ");  getCardExpiryDate(ptoCardData3);
	ST_terminalData_t Terminal3;
	ST_terminalData_t *ptoTerminalData3 = &Terminal3;
	printf("Transaction Date: "); getTransactionDate(ptoTerminalData3);
	int res3 = isCardExpired(ptoCardData3, ptoTerminalData3);
	printf("Expected Result: 0 for TERMINAL_OK\n");
	printf("Actual Result: %d\n\n", res3);


	printf("Test Case 4: Same Year, Transaction Month before Expiry Month\n");
	ST_cardData_t Card4;
	ST_cardData_t *ptoCardData4 = &Card4;
	printf("Expiry Date: ");  getCardExpiryDate(ptoCardData4);
	ST_terminalData_t Terminal4;
	ST_terminalData_t *ptoTerminalData4 = &Terminal4;
	printf("Transaction Date: "); getTransactionDate(ptoTerminalData4);
	int res4 = isCardExpired(ptoCardData4, ptoTerminalData4);
	printf("Expected Result: 0 for TERMINAL_OK\n");
	printf("Actual Result: %d\n\n", res4);


	printf("Test Case 5: Transaction Year Before Expiry Year\n");
	ST_cardData_t Card5;
	ST_cardData_t *ptoCardData5 = &Card5;
	printf("Expiry Date: ");  getCardExpiryDate(ptoCardData5);
	ST_terminalData_t Terminal5;
	ST_terminalData_t *ptoTerminalData5 = &Terminal5;
	printf("Transaction Date: "); getTransactionDate(ptoTerminalData5);
	printf("Test Case 5: Transaction Year Before Expiry Year\n");
	int res5 = isCardExpired(ptoCardData5, ptoTerminalData5);
	printf("Expected Result: 0 for TERMINAL_OK\n");
	printf("Actual Result: %d\n\n", res5);
}
void getTransactionAmountTest(void)
{
	printf("Tester Name: Karim Nasr Aboelfath Abdelfattah\n");
	printf("Function Name : getTransactionAmount\n\n");

	ST_terminalData_t Terminal1;
	ST_terminalData_t * ptoTerminalData = &Terminal1;

	printf("Test Case 1: Negative Amount\n");
	printf("Input Data: "); int res1 = getTransactionAmount(ptoTerminalData);
	printf("Expected Result: 4 for INVALID_AMOUNT\n");
	printf("Actual Result: %d\n\n", res1);

	printf("Test Case 2: Zero Amount\n");
	printf("Input Data: "); int res2 = getTransactionAmount(ptoTerminalData);
	printf("Expected Result: 4 for INVALID_AMOUNT\n");
	printf("Actual Result: %d\n\n", res2);

	printf("Test Case 3: Positive Amount\n");
	printf("Input Data: "); int res3 = getTransactionAmount(ptoTerminalData);
	printf("Expected Result: 0 for TERMINAL_OK\n");
	printf("Actual Result: %d\n\n", res3);
}
void isBelowMaxAmountTest(void)
{
	printf("Tester Name: Karim Nasr Aboelfath Abdelfattah\n");
	printf("Function Name : isBelowMaxAmount\n\n");

	printf("Test Case 1: Above Maximum Amount\n");
	ST_terminalData_t Terminal1;
	ST_terminalData_t * ptoTerminalData = &Terminal1;
	float maxTransAmount_Var = 0.0;
	float transAmount_Var = 0.0;
	printf("Maximum Amount: "); scanf_s("%f", &maxTransAmount_Var);
	printf("Transaction Amount: "); scanf_s("%f", &transAmount_Var);
	ptoTerminalData->maxTransAmount = maxTransAmount_Var;
	ptoTerminalData->transAmount = transAmount_Var;
	int res1 = isBelowMaxAmount(ptoTerminalData);
	printf("Expected Result: 5 for EXCEED_MAX_AMOUNT\n");
	printf("Actual Result: %d\n\n", res1);

	printf("Test Case 2: Equals Maximum Amount\n");
	printf("Maximum Amount: "); scanf_s("%f", &maxTransAmount_Var);
	printf("Transaction Amount: "); scanf_s("%f", &transAmount_Var);
	ptoTerminalData->maxTransAmount = maxTransAmount_Var;
	ptoTerminalData->transAmount = transAmount_Var;
	int res2 = isBelowMaxAmount(ptoTerminalData);
	printf("Expected Result: 0 for TERMINAL_OK\n");
	printf("Actual Result: %d\n\n", res2);

	printf("Test Case 3: Below Maximum Amount\n");
	printf("Maximum Amount: "); scanf_s("%f", &maxTransAmount_Var);
	printf("Transaction Amount: "); scanf_s("%f", &transAmount_Var);
	ptoTerminalData->maxTransAmount = maxTransAmount_Var;
	ptoTerminalData->transAmount = transAmount_Var;
	int res3 = isBelowMaxAmount(ptoTerminalData);
	printf("Expected Result: 0 for TERMINAL_OK\n");
	printf("Actual Result: %d\n\n", res3);
}
void setMaxAmountTest(void)
{
	printf("Tester Name: Karim Nasr Aboelfath Abdelfattah\n");
	printf("Function Name : setMaxAmount\n\n");

	printf("Test Case 1: Negative Maximum Amount\n");
	ST_terminalData_t Terminal1;
	ST_terminalData_t* ptoTerminalData = &Terminal1;
	float maxTransAmount_Var = 0.0;
	printf("Maximum Amount: "); scanf_s("%f", &maxTransAmount_Var);
	int res1 = setMaxAmount(ptoTerminalData, maxTransAmount_Var);
	printf("Expected Result: 6 for INVALID_MAX_AMOUNT\n");
	printf("Actual Result: %d\n\n", res1);

	printf("Test Case 1: Zero Maximum Amount\n");
	printf("Maximum Amount: "); scanf_s("%f", &maxTransAmount_Var);
	int res2 = setMaxAmount(ptoTerminalData, maxTransAmount_Var);
	printf("Expected Result: 6 for INVALID_MAX_AMOUNT\n");
	printf("Actual Result: %d\n\n", res2);

	printf("Test Case 1: Positive Maximum Amount\n");
	printf("Maximum Amount: "); scanf_s("%f", &maxTransAmount_Var);
	int res3 = setMaxAmount(ptoTerminalData, maxTransAmount_Var);
	printf("Expected Result: 0 for TERMINAL_OK\n");
	printf("Actual Result: %d\n\n", res3);
}
void isValidCardPANTest(void)
{
	printf("Tester Name: Karim Nasr Aboelfath Abdelfattah\n");
	printf("Function Name : isValidCardPAN\n\n");
	ST_cardData_t Card1;
	ST_cardData_t* ptoCardData1 = &Card1;

	printf("Test Case 1: Invalid PAN1\n");
	printf("Card PAN: ");
	int resx1 = getCardPAN(ptoCardData1);
	int res1 = isValidCardPAN(ptoCardData1);
	printf("Expected Result: 3 for INVALID_CARD\n");
	printf("Actual Result: %d\n\n", res1);

	ST_cardData_t Card2;
	ST_cardData_t* ptoCardData2 = &Card2;

	printf("Test Case 2: Invalid PAN2\n");
	printf("Card PAN: ");
	int resx2 = getCardPAN(ptoCardData2);
	int res2 = isValidCardPAN(ptoCardData2);
	printf("Expected Result: 3 for INVALID_CARD\n");
	printf("Actual Result: %d\n\n", res2);

	ST_cardData_t Card3;
	ST_cardData_t* ptoCardData3 = &Card3;

	printf("Test Case 3: Valid PAN1\n");
	printf("Card PAN: ");
	int resx3 = getCardPAN(ptoCardData3);
	int res3 = isValidCardPAN(ptoCardData3);
	printf("Expected Result: 0 for TERMINAL_OK\n");
	printf("Actual Result: %d\n\n", res3);

	ST_cardData_t Card4;
	ST_cardData_t* ptoCardData4 = &Card4;

	printf("Test Case 4: Valid PAN1\n");
	printf("Card PAN: ");
	int resx4 = getCardPAN(ptoCardData4);
	int res4 = isValidCardPAN(ptoCardData4);
	printf("Expected Result: 0 for TERMINAL_OK\n");
	printf("Actual Result: %d\n\n", res4);
}