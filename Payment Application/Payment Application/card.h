/*
* ************ Author	: Ibrahim Saber		****************
* ************ Date		: July 2022			****************
* ************ SWC		: Card.h			****************
*/
// Header Guard
#ifndef CARD_H_
#define CARD_H_

/* Enum for card Errors data type  */
typedef enum EN_cardError_t
{
	OK, WRONG_NAME, WRONG_EXP_DATE, WRONG_PAN
}EN_cardError_t;


/* Structure contains the Data needed  by the Card SWC */
typedef struct ST_cardData_t
{
	uint8_t cardHolderName[25];
	uint8_t primaryAccountNumber[20];
	uint8_t cardExpirationDate[6];
}ST_cardData_t;

EN_cardError_t getCardHolderName(ST_cardData_t* cardData);
EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData);
EN_cardError_t getCardPAN(ST_cardData_t* cardData);

#endif // !CARD_H_