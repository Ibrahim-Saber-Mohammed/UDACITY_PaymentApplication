#include<stdio.h>
#include<string.h>
#include<stdint.h>
#include"card.h"
#include"terminal.h"
#include"server.h"
#include"app.h"

void appStart(void)
{
    /* Card Test */
    /*****************************************************
    * getCardHolderNameTest();
    * getCardExpiryDateTest();
    * getCardPANTest();
    **************************************************/
    /* Terminal Test */
   /*****************************************************
   * 
   * ****************************************************/
    /*
    getTransactionDateTest();
    getTransactionAmountTest();
    setMaxAmountTest();
    isBelowMaxAmountTest();
    isCardExpiredTest();
    */
    /*========================Server Test=================================================*/
    /*
    printf("=============================================================================\n");
    isBlockedAccountTest();
    printf("=============================================================================\n");
    isValidAccountTest();
    printf("=============================================================================\n");
    isAmountAvailableTest();
    printf("=============================================================================\n");
    saveTransactionTest();
    printf("=============================================================================\n");
    listSavedTransactions();
    printf("=============================================================================\n");
    */




    uint32_t choice = 0;
    ST_transaction_t loc_AccountInfo;
    EN_cardError_t loc_cardErrorState = CARD_OK;
    EN_terminalError_t loc_terminalErrorState = TERMINAL_OK;
    EN_serverError_t loc_serverErrorState = SERVER_OK;
    do {
        loc_terminalErrorState = setMaxAmount(&loc_AccountInfo.terminalData, 50000.0);
    } while (loc_terminalErrorState == INVALID_MAX_AMOUNT);
    printf("==================================================================================\n");
    printf("===================Welcome to our Payment application=============================\n");
    printf("==================================================================================\n");
    printf("Please, take into consideration that maximum allowed value is 50000\n");
    printf("Please Select one of the Following options\n");
    printf("1- Use the System \n");
    printf("2- List all transactions \n");
    printf("0- Exit\n");
    printf("Your Choice : ");
    (void)scanf("%d%*C", &choice);
    switch (choice)
    {
    case 0 :
        printf("\nThanks for using Our System ....\n");
        printf("Closing....");
        exit(0);
        break;

    case 1:
        loc_cardErrorState = getCardHolderName(&loc_AccountInfo.cardHolderData);
        if (loc_cardErrorState == CARD_OK)
        {
            loc_cardErrorState = getCardExpiryDate(&loc_AccountInfo.cardHolderData);
            if (loc_cardErrorState == CARD_OK)
            {
                loc_cardErrorState = getCardPAN(&loc_AccountInfo.cardHolderData);
                if (loc_cardErrorState == CARD_OK)
                {
                    loc_terminalErrorState = getTransactionDate(&loc_AccountInfo.terminalData);
                    if (loc_terminalErrorState == TERMINAL_OK)
                    {
                        loc_terminalErrorState = isCardExpired(&loc_AccountInfo.cardHolderData, &loc_AccountInfo.terminalData);
                        if (loc_terminalErrorState == TERMINAL_OK)
                        {
                            loc_terminalErrorState = getTransactionAmount(&loc_AccountInfo.terminalData);
                            if (loc_terminalErrorState == TERMINAL_OK)
                            {
                                loc_terminalErrorState = isBelowMaxAmount(&loc_AccountInfo.terminalData);
                                if (loc_terminalErrorState == TERMINAL_OK)
                                {
                                    loc_serverErrorState = recieveTransactionData(&loc_AccountInfo);
                                    if (loc_serverErrorState == SERVER_OK)
                                    {
                                        printf("====================================================================\n");
                                        printf(" Your transaction is processing....\n");
                                        printf("your transaction is complete\n");
                                        printf("thank you for using our payment application\n");
                                        printf("====================================================================\n");
                                        break;
                                    }
                                    else if (loc_serverErrorState == ACCOUNT_NOT_FOUND)
                                    {
                                        printf("====================================================================\n");
                                        printf("Sorry your account is invalid....\n");
                                        printf("Thank you for using our payment application\n ");
                                        printf("====================================================================\n");
                                        exit(0);
                                    }
                                    else if (loc_serverErrorState == LOW_BALANCE)
                                    {
                                        printf("====================================================================\n");
                                        printf("Sorry you have insuffecient funds\n");
                                        printf("Thank you for using our payment application\n ");
                                        printf("====================================================================\n");
                                        exit(0);
                                    }
                                    else if(loc_serverErrorState== DECLINED_STOLEN_CARD)
                                    {
                                        printf("====================================================================\n");
                                        printf("valid and not expired but stolen card\n");
                                        printf("Thank you for using our payment application\n ");
                                        printf("====================================================================\n");
                                        exit(0);
                                    }
                                }
                                else
                                {
                                    printf("====================================================================\n");
                                    printf("sorry you excedded tha maximum allowed amount...\n");
                                    printf("====================================================================\n");
                                    exit(0);
                                }
                            }
                            else
                            {
                                printf("====================================================================\n");
                                printf("Wrong transaction amount...\n");
                                printf("====================================================================\n");
                            }
                        }
                        else
                        {
                            printf("====================================================================\n");
                            printf(" Sorry your card is expired ..\n");
                            printf("====================================================================\n");
                            exit(0);
                        }
                    }
                    else
                    {
                        printf("====================================================================\n");
                        printf("Wrong Transaction Date Formate\n");
                        printf("====================================================================\n");
                    }
                }
                else
                {
                    printf("====================================================================\n");
                    printf("Wrong PAN formate\n");
                    printf("====================================================================\n");
                }
            }
            else
            {
                printf("====================================================================\n");
                printf("Wrong Card Expiry Date formate\n");
                printf("====================================================================\n");
            }
        }
        else
        {
            printf("====================================================================\n");
            printf("Wrong Card holder name formate\n");
            printf("====================================================================\n");
        }
        break;
    case 2:
        printf("====================================================================\n");
        listSavedTransactions();
        printf("====================================================================\n");
        break;
    default:
        break;
    }

}