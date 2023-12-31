/*
* ************ Author	: Ibrahim Saber		****************
* ************ Date		: July 2022			****************
* ************ SWC		: app.h				****************
*/
#ifndef APP_H_
#define APP_H_

typedef enum
{
	ERROR, INIT, START, CARD_HOLDER_NAME, CARD_EXPIRATION, CARD_PAN, GET_TRANSACTION_DATE,
	IS_CARD_EXPIRED, IS_CARD_VALID, GET_TRANSACTION_AMOUNT, IS_BELOW_MAX, SNED_DATA_SERVER
}App_Steps_t;
void StartApp(void);

#endif // !APP_H_
