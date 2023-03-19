#include <stdio.h>
#include <string.h>
#include "../Card/card.h"

EN_cardError_t getCardHolderName(ST_cardData_t *cardData)
{
	/*
	This function prompts the user to enter the cardholder's name and stores it into the
	'cardHolderName' member of the 'cardData' structure.
	The function checks if the entered name is valid (i.e., between 20 and 24 alphabetic
	characters) and returns an appropriate error message if not.
	@param cardData Pointer to the card data structure.
	@return CARD_OK if the cardholder name is valid and successfully stored in the
	'cardHolderName' member of the 'cardData' structure.
	*/

	// Declare and initialize an array to hold the user's input for the cardholder's name.
	uint8_t cardHolderName_Var[50] = {""};

	// Prompt the user to enter the cardholder's name and read the input into the 'cardHolderName_Var' array.
	printf("Insert Card Holder Name: ");
	gets(cardHolderName_Var);
	
	// Check if the entered name is valid, i.e., not NULL, between 20 and 24 characters and contains only alphabetic
	//characters and spaces.
	int isAllSpaceFlag = 1;
	if (strlen(cardHolderName_Var) == 0)	{ printf("Invalid Name! Please try again.\n\n"); return WRONG_NAME; }
	if (strlen(cardHolderName_Var) < 20 || strlen(cardHolderName_Var) > 24) { printf("Invalid Name! Please try again.\n\n"); return WRONG_NAME; }

	for (int i = 0; i < strlen(cardHolderName_Var); i++)
	{
		int isA = isalpha(cardHolderName_Var[i]);
		if (isA) { isAllSpaceFlag = 0; }
		int isS = isspace(cardHolderName_Var[i]);
		
		if (!(isA || isS) || isAllSpaceFlag) { printf("Invalid Name! Please try again.\n\n"); return WRONG_NAME; }
	}
	// If the entered name is valid, copy it into the 'cardHolderName' member of the 'cardData' structure.
	strcpy_s(cardData->cardHolderName, sizeof(cardData->cardHolderName), cardHolderName_Var);

	// Return CARD_OK to indicate that the cardholder name was successfully stored in the 'cardHolderName' member of
	//the 'cardData' structure.
	return CARD_OK;
}
EN_cardError_t getCardExpiryDate(ST_cardData_t *cardData)
{
	/*
	This function prompts the user to enter the card expiry date and stores it into the 
	'cardExpirationDate' member of the 'cardData' structure. The function checks if the 
	entered expiry date is valid (i.e., 5 characters long and in the format "MM/YY") and 
	returns an appropriate error message if not.
	@param cardData Pointer to the card data structure.
	@return CARD_OK if the card expiry date is valid and successfully stored in the 
	'cardExpirationDate' member of the 'cardData' structure.
	*/

	// Declare and initialize an array to hold the user's input for the card expiry date.
	uint8_t cardExpirationDate_Var[50] = { "" };

	// Prompt the user to enter the card expiry date and read the input into the 'cardExpirationDate_Var' array.
	printf("Insert Card Expiry Date: ");
	gets(cardExpirationDate_Var);

	// Check if the entered date is valid, i.e., not NULL, in the format MM/YY and contains only digits and '/'.
	if (strlen(cardExpirationDate_Var) == 0 || strlen(cardExpirationDate_Var) != 5) { printf("Invalid Expiry Date! Please try again.\n\n"); return WRONG_EXP_DATE; }
	for (int i = 0; i < strlen(cardExpirationDate_Var); i++)
	{
		if (i == 0 && (cardExpirationDate_Var[i] != '0' && cardExpirationDate_Var[i] != '1')) { printf("Invalid Expiry Date! Please try again.\n\n"); return WRONG_EXP_DATE; }
		if (i == 1)
		{
			uint8_t x1 = cardExpirationDate_Var[1];
			int isnD1 = !(isdigit(x1));
			if (isnD1) { printf("Invalid Expiry Date! Please try again.\n\n"); return WRONG_EXP_DATE; }
			uint8_t x0 = cardExpirationDate_Var[0];
			if (x0 == '1') { if (x1 != '0' && x1 != '1' && x1 != '2') { printf("Invalid Expiry Date! Please try again.\n\n"); return WRONG_EXP_DATE; } }
			if (x0 == '0' && x1 == '0') { printf("Invalid Expiry Date! Please try again.\n\n"); return WRONG_EXP_DATE; }
		}
		if (i == 2 && (cardExpirationDate_Var[i] != '/')) { printf("Invalid Expiry Date! Please try again.\n\n"); return WRONG_EXP_DATE; }
		int isnD3 = !(isdigit(cardExpirationDate_Var[i]));
		if (i == 3 && isnD3) { printf("Invalid Expiry Date! Please try again.\n\n"); return WRONG_EXP_DATE; }
		int isnD4 = !(isdigit(cardExpirationDate_Var[i]));
		if (i == 4 && isnD4) { printf("Invalid Expiry Date! Please try again.\n\n"); return WRONG_EXP_DATE; }
	}
	// If the entered date is valid, copy it into the 'cardExpirationDate' member of the 'cardData' structure.
	strcpy_s(cardData->cardExpirationDate, sizeof(cardData->cardExpirationDate), cardExpirationDate_Var);

	// Return CARD_OK to indicate that the expiry date was successfully stored in the 'cardExpirationDate' member of
	//the 'cardData' structure.
	return CARD_OK;
}
EN_cardError_t getCardPAN(ST_cardData_t *cardData)
{
	/*
	This function prompts the user to enter the card's Primary Account Number and stores
	it in the 'primaryAccountNumber' member of the 'cardData' structure. The function checks
	if the entered PAN is valid (i.e., between 16 and 19 numeric characters) and returns an 
	appropriate error message if not.
	@param cardData Pointer to the card data structure.
	@return CARD_OK if the Primary Account Number is valid and successfully stored in the 
	'primaryAccountNumber' member of the 'cardData' structure.
	*/

	// Declare and initialize an array to hold the user's input for the Primary Account Number.
	uint8_t primaryAccountNumber_Var[50] = { "" };

	// Prompt the user to enter the Primary Account Number and read the input into the 'primaryAccountNumber_Var' array.
	printf("Insert Primary Account Number: ");
	gets(primaryAccountNumber_Var);


	// Check if the entered PAN is valid, i.e., not NULL, between 16 and 19 characters and contains only numeric 
	//characters and no spaces.
	int isAllDigitFlag = 1;
	int isAllSpaceFlag = 1;
	if (strlen(primaryAccountNumber_Var) == 0) { printf("Invalid Primary Account Number! Please try again.\n\n"); return WRONG_PAN; }
	if (strlen(primaryAccountNumber_Var) < 16 || strlen(primaryAccountNumber_Var) > 19) { printf("Invalid Primary Account Number! Please try again.\n\n"); return WRONG_PAN; }
	for (int i = 0; i < strlen(primaryAccountNumber_Var); i++)
	{
		int isS = isspace(primaryAccountNumber_Var[i]);
		if (!isS) { isAllSpaceFlag = 0; }
		if (isAllSpaceFlag) { printf("Invalid Primary Account Number! Please try again.\n\n"); return WRONG_PAN; }
	}
	for (int i = 0; i < strlen(primaryAccountNumber_Var); i++)
	{
		int isD = isdigit(primaryAccountNumber_Var[i]);
		if (!isD) { isAllDigitFlag = 0; }
		if (!isAllDigitFlag) { printf("Invalid Primary Account Number! Please try again.\n\n"); return WRONG_PAN; }
	}

	// If the entered PAN is valid, copy it into the 'primaryAccountNumber' member of the 'cardData' structure.
	strcpy_s(cardData->primaryAccountNumber, sizeof(cardData->primaryAccountNumber), primaryAccountNumber_Var);

	// Return CARD_OK to indicate that the Primary Account Number was successfully stored in the 'primaryAccountNumber'
	//member of the 'cardData' structure.
	return CARD_OK;
}

void getCardHolderNameTest(void)
{
	printf("Tester Name: Karim Nasr Aboelfath Abdelfattah\n");
	printf("Function Name : getCardHolderName\n\n");

	ST_cardData_t Card1;
	ST_cardData_t *ptoCardData = &Card1;

	printf("Test Case 1: Name Too Short\n");
	printf("Input Data: "); int res1 = getCardHolderName(ptoCardData);
	printf("Expected Result: 1 for WRONG_NAME\n");
	printf("Actual Result: %d\n\n", res1);

	printf("Test Case 2: Name Too Long\n");
	printf("Input Data: "); int res2 = getCardHolderName(ptoCardData);
	printf("Expected Result: 1 for WRONG_NAME\n");
	printf("Actual Result: %d\n\n", res2);

	printf("Test Case 3: Name All Spaces\n");
	printf("Input Data: "); int res3 = getCardHolderName(ptoCardData);
	printf("Expected Result: 1 for WRONG_NAME\n");
	printf("Actual Result: %d\n\n", res3);

	printf("Test Case 4: Empty Name\n");
	printf("Input Data: "); int res4 = getCardHolderName(ptoCardData);
	printf("Expected Result: 1 for WRONG_NAME\n");
	printf("Actual Result: %d\n\n", res4);

	printf("Test Case 5: Special Character\n");
	printf("Input Data: "); int res5 = getCardHolderName(ptoCardData);
	printf("Expected Result: 1 for WRONG_NAME\n");
	printf("Actual Result: %d\n\n", res5);

	printf("Test Case 6: Valid Name\n");
	printf("Input Data: "); int res6 = getCardHolderName(ptoCardData);
	printf("Expected Result: 0 for CARD_OK\n");
	printf("Actual Result: %d\n\n", res6);
}
void getCardExpiryDateTest(void)
{
	printf("Tester Name: Karim Nasr Aboelfath Abdelfattah\n");
	printf("Function Name : getCardExpiryDate \n\n");

	ST_cardData_t Card1;
	ST_cardData_t *ptoCardData = &Card1;

	printf("Test Case 1: Date Too Short\n");
	printf("Input Data: "); int res1 = getCardExpiryDate(ptoCardData);
	printf("Expected Result: 2 for WRONG_EXP_DATE\n");
	printf("Actual Result: %d\n\n", res1);

	printf("Test Case 2: Date Too Long\n");
	printf("Input Data: "); int res2 = getCardExpiryDate(ptoCardData);
	printf("Expected Result: 2 for WRONG_EXP_DATE\n");
	printf("Actual Result: %d\n\n", res2);

	printf("Test Case 3: Date All Spaces\n");
	printf("Input Data: "); int res3 = getCardExpiryDate(ptoCardData);
	printf("Expected Result: 2 for WRONG_EXP_DATE\n");
	printf("Actual Result: %d\n\n", res3);

	printf("Test Case 4: Empty Date\n");
	printf("Input Data: "); int res4 = getCardExpiryDate(ptoCardData);
	printf("Expected Result: 2 for WRONG_EXP_DATE\n");
	printf("Actual Result: %d\n\n", res4);

	printf("Test Case 5: Invalid Month 1\n");
	printf("Input Data: "); int res5 = getCardExpiryDate(ptoCardData);
	printf("Expected Result: 2 for WRONG_EXP_DATE\n");
	printf("Actual Result: %d\n\n", res5);

	printf("Test Case 6: Invalid Month 2\n");
	printf("Input Data: "); int res6 = getCardExpiryDate(ptoCardData);
	printf("Expected Result: 2 for WRONG_EXP_DATE\n");
	printf("Actual Result: %d\n\n", res6);

	printf("Test Case 7: Invalid Month 3\n");
	printf("Input Data: "); int res7 = getCardExpiryDate(ptoCardData);
	printf("Expected Result: 2 for WRONG_EXP_DATE\n");
	printf("Actual Result: %d\n\n", res7);

	printf("Test Case 8: Invalid Date Separator\n");
	printf("Input Data: "); int res8 = getCardExpiryDate(ptoCardData);
	printf("Expected Result: 2 for WRONG_EXP_DATE\n");
	printf("Actual Result: %d\n\n", res8);

	printf("Test Case 9: Invalid Year\n");
	printf("Input Data: "); int res9 = getCardExpiryDate(ptoCardData);
	printf("Expected Result: 2 for WRONG_EXP_DATE\n");
	printf("Actual Result: %d\n\n", res9);

	printf("Test Case 10: Valid Date Format\n");
	printf("Input Data: "); int res10 = getCardExpiryDate(ptoCardData);
	printf("Expected Result: 0 for CARD_OK\n");
	printf("Actual Result: %d\n\n", res10);
}
void getCardPANTest(void)
{
	printf("Tester Name: Karim Nasr Aboelfath Abdelfattah\n");
	printf("Function Name : getCardPAN\n\n");

	ST_cardData_t Card1;
	ST_cardData_t *ptoCardData = &Card1;

	printf("Test Case 1: PAN Too Short\n");
	printf("Input Data: "); int res1 = getCardPAN(ptoCardData);
	printf("Expected Result: 3 for WRONG_PAN\n");
	printf("Actual Result: %d\n\n", res1);

	printf("Test Case 2: PAN Too Long\n");
	printf("Input Data: "); int res2 = getCardPAN(ptoCardData);
	printf("Expected Result: 3 for WRONG_PAN\n");
	printf("Actual Result: %d\n\n", res2);

	printf("Test Case 3: PAN All Spaces\n");
	printf("Input Data: "); int res3 = getCardPAN(ptoCardData);
	printf("Expected Result: 3 for WRONG_PAN\n");
	printf("Actual Result: %d\n\n", res3);

	printf("Test Case 4: Empty PAN\n");
	printf("Input Data: "); int res4 = getCardPAN(ptoCardData);
	printf("Expected Result: 3 for WRONG_PAN\n");
	printf("Actual Result: %d\n\n", res4);

	printf("Test Case 5: Nonnumerical Characters\n");
	printf("Input Data: "); int res5 = getCardPAN(ptoCardData);
	printf("Expected Result: 3 for WRONG_PAN\n");
	printf("Actual Result: %d\n\n", res5);

	printf("Test Case 6: Valid PAN\n");
	printf("Input Data: "); int res6 = getCardPAN(ptoCardData);
	printf("Expected Result: 0 for CARD_OK\n");
	printf("Actual Result: %d\n\n", res6);
}