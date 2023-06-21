/*
* ************ Author	: Ibrahim Saber		****************
* ************ Date		: July 2022			****************
* ************ SWC		: app.c				****************
*/
#include <stdio.h>
#include <string.h>
#include "STD_TYPES.h"
#include "app.h"
#include "card.h"
#include "terminal.h"
#include "server.h"


void StartApp(void)
{
	ST_transaction_t ClientData;
	EN_cardError_t Local_CardErrorRetuen = OK;
	EN_terminalError_t Local_TerminalErrorReturn = TOK;
	EN_serverError_t Local_ServerErrorReturn = SOK;
	App_Steps_t Local_NextStep = INIT;
	printf("-----------------------------------------------------\n");
	printf("Welcom to our Payment Application\n");
	printf("-----------------------------------------------------\n");
	while (1)
	{
		switch (Local_NextStep)
		{
		case INIT: // Configure the maximum amount of the the system user can withdraw
			printf("-----------------------------------------------------\n");
			Local_TerminalErrorReturn = setMaxAmount(&ClientData.terminalData);
			printf("-----------------------------------------------------\n");
			if (Local_TerminalErrorReturn == TOK)
			{
				printf("***********************************************\n");
				printf("********* The Max Amount is saved ... *********\n");
				printf("***********************************************\n");
				Local_NextStep = START;
			}
			else
			{
				printf("*****************************************\n");
				printf("******* Wrong Max Amount **********\n");
				printf("******* Please, Try again... ******\n");
				printf("*****************************************\n");
			}
			break;
		case START:
			printf("**********************************************************************\n");
			printf("**************** Welcomme to Our Payment application *****************\n");
			printf("**********************************************************************\n");
			Local_CardErrorRetuen = getCardHolderName(&ClientData.cardHolderData);
			printf("-----------------------------------------------------------------------\n");
			if (Local_CardErrorRetuen == OK)
			{
				Local_NextStep = CARD_EXPIRATION;
			}
			else
			{
				printf("*****************************************\n");
				printf("******* Wrong Card holder name **********\n");
				printf("******* Please, Try again... ************\n");
				printf("*****************************************\n");
			}
			break;

		case CARD_EXPIRATION:
			printf("-----------------------------------------------------------------------\n");
			Local_CardErrorRetuen = getCardExpiryDate(&ClientData.cardHolderData);
			printf("-----------------------------------------------------------------------\n");
			if (Local_CardErrorRetuen == OK)
			{
				Local_NextStep = CARD_PAN;
			}
			else
			{
				printf("*****************************************\n");
				printf("******* Wrong Card Expiry Date  *********\n");
				printf("******* Please look at the Example ******\n");
				printf("******* Please, Try again...	*********\n");
				printf("*****************************************\n");
				Local_NextStep = START;
			}
			break;
		case CARD_PAN:
			printf("-----------------------------------------------------------------------\n");
			Local_CardErrorRetuen = getCardPAN(&ClientData.cardHolderData);
			printf("-----------------------------------------------------------------------\n");
			if (Local_CardErrorRetuen == OK)
			{
				Local_NextStep = IS_CARD_VALID;
			}
			else
			{
				printf("******************************************\n");
				printf("******* Wrong PAN Number format **********\n");
				printf("******* Please, Try again... *************\n");
				printf("******************************************\n");
				Local_NextStep = START;
			}
			break;
		case IS_CARD_VALID:
			Local_TerminalErrorReturn = isValidCardPAN(&ClientData.cardHolderData);
			if (Local_TerminalErrorReturn == TOK)
			{
				Local_NextStep = GET_TRANSACTION_DATE;
			}
			else
			{
				printf("***********************************************\n");
				printf("*********** Your CARD IS NOT VALID ************\n");
				printf("*********** Please Check Your PAN Number ******\n");
				printf("***********************************************\n");
				Local_NextStep = START;
			}
			break;
		case GET_TRANSACTION_DATE:
			printf("-----------------------------------------------------------------------\n");
			Local_TerminalErrorReturn = getTransactionDate(&ClientData.terminalData);
			printf("-----------------------------------------------------------------------\n");
			if (Local_TerminalErrorReturn == TOK)
			{
				Local_NextStep = IS_CARD_EXPIRED;
			}
			else
			{
				printf("*************************************************\n");
				printf("******* Wrong Transaction  Date format **********\n");
				printf("******* Please look at the Example	   **********\n");
				printf("******* Please, Try again...		   **********\n");
				printf("*************************************************\n");
				Local_NextStep = START;
			}
			break;
		case IS_CARD_EXPIRED:
			Local_TerminalErrorReturn = isCardExpired(ClientData.cardHolderData, ClientData.terminalData);
			printf("-----------------------------------------------------------------------\n");
			if (Local_TerminalErrorReturn == OK)
			{
				Local_NextStep = GET_TRANSACTION_AMOUNT;
			}
			else
			{
				printf("***********************************************\n");
				printf("********** Sorry Your Card Is Expired *********\n");
				printf("***********************************************\n");
				Local_NextStep = START;
			}
			break;
		
		case GET_TRANSACTION_AMOUNT:
			printf("-----------------------------------------------------------------------\n");
			Local_TerminalErrorReturn = getTransactionAmount(&ClientData.terminalData);
			printf("-----------------------------------------------------------------------\n");
			if (Local_TerminalErrorReturn == TOK)
			{
				Local_NextStep = IS_BELOW_MAX;
			}
			else
			{
				printf("*****************************************************\n");
				printf("******** error with Your Transaction amount *********\n");
				printf("******** Please try again.....				*********\n");
				printf("*****************************************************\n");
				Local_NextStep = START;
			}
			break;
		case IS_BELOW_MAX:
			Local_TerminalErrorReturn = isBelowMaxAmount(&ClientData.terminalData);
			if (Local_TerminalErrorReturn == OK)
			{
				Local_NextStep = SNED_DATA_SERVER;
			}
			else
			{
				printf("************************************************************************\n");
				printf("********* Your Transaction Exceeds the max transaction amount **********\n");
				printf("********* Please , try again.....                             **********\n");
				printf("************************************************************************\n");
				Local_NextStep = START;
			}
			break;
		case SNED_DATA_SERVER:
			ClientData.transactionSequenceNumber = 0;
			ClientData.transState = APPROVED;
			Local_ServerErrorReturn = recieveTransactionData(&ClientData);
			if (Local_ServerErrorReturn == SOK)
			{
				printf("*********************************************************************\n");
				printf("*************  Your Transaction is complete successfully ************\n");
				printf("************* Thanks for using Our System		 ************\n");
				printf("*********************************************************************\n");
			}
			else if (Local_ServerErrorReturn == DECLINED_INSUFFECIENT_FUND) {
				printf("*********************************************************************\n");
				printf("*************  Sorry, Your Transaction is Declined	 ************\n");
				printf("*************  You have insuffcient Fund		 ************\n");
				printf("*********************************************************************\n");
			}
			else if (Local_ServerErrorReturn == DECLINED_STOLEN_CARD)
			{
				printf("*********************************************************************\n");
				printf("*************  Sorry, Your PAN is not found		 ************\n");
				printf("*********************************************************************\n");
			}
			else
			{
				printf("*********************************************************************\n");
				printf("*************  Sorry, Your Transaction is Declined	 ************\n");
				printf("*************  Please try again later			 ************\n");
				printf("*********************************************************************\n");
			}
			Local_NextStep = START;
			break;
		}
	}
					
}
