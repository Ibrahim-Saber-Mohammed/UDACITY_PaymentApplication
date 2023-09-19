#include <stdint.h>
#include <string.h>
#include "card.h"
#include "terminal.h"
#include "server.h"

ST_accountsDB_t accountsDB[255] = {
    {5000000.0, RUNNING, "8989374615436851"},
    {100000.0, BLOCKED,  "5807007076043875"},
    {15000.0, RUNNING,  "2499925799306994"}
};

ST_transaction_t transactionsDB[255] = {0};
static uint8_t Global_Sequence_Number = 0;

EN_transState_t recieveTransactionData(ST_transaction_t *transData)
{
    EN_serverError_t loc_SreverReturnData = SERVER_OK;
    ST_accountsDB_t Loc_ST_referenceAccount = {0};
    loc_SreverReturnData = isValidAccount(&transData->cardHolderData , &Loc_ST_referenceAccount);
    if (loc_SreverReturnData == SERVER_OK)
    {
        loc_SreverReturnData = isBlockedAccount(&Loc_ST_referenceAccount);
        if(loc_SreverReturnData == SERVER_OK)
        {
            loc_SreverReturnData = isAmountAvailable(&transData->terminalData, &Loc_ST_referenceAccount);
            if(loc_SreverReturnData == SERVER_OK)
            {
                Loc_ST_referenceAccount.state == APPROVED;
            }
            else
            {
                Loc_ST_referenceAccount.state = DECLINED_INSUFFECIENT_FUND;
                loc_SreverReturnData = LOW_BALANCE;
            }
        }
        else
        {
            Loc_ST_referenceAccount.state = DECLINED_STOLEN_CARD;
            loc_SreverReturnData = BLOCKED_ACCOUNT;
        }
    }
    else{
        Loc_ST_referenceAccount.state = FRAUD_CARD;
        loc_SreverReturnData = ACCOUNT_NOT_FOUND;
    }
    if (loc_SreverReturnData == SERVER_OK)
    {
        Loc_ST_referenceAccount.balance -= transData->terminalData.transAmount;
        printf("Transaction amount : %f\n", transData->terminalData.transAmount);
        printf("Your balance       : %f\n", Loc_ST_referenceAccount.balance);
    }
    else {
        
        printf("Your balance       : %f\n", Loc_ST_referenceAccount.balance);

    }
    saveTransaction(transData);
    return loc_SreverReturnData;
}

EN_serverError_t isValidAccount(ST_cardData_t *cardData, ST_accountsDB_t *accountRefrence)
{
    EN_serverError_t loc_SreverReturnData = SERVER_OK;
    uint32_t loc_bankaccountsIterator = 0;
    for (loc_bankaccountsIterator = 0; loc_bankaccountsIterator< 255 ; loc_bankaccountsIterator++)
    {
        if (strcmp(cardData->primaryAccountNumber , accountsDB[loc_bankaccountsIterator].primaryAccountNumber) != 0)
        {
            loc_SreverReturnData = ACCOUNT_NOT_FOUND;
        }
        else
        {
            memcpy(accountRefrence, &accountsDB[loc_bankaccountsIterator], sizeof(ST_accountsDB_t));
            loc_SreverReturnData = SERVER_OK;
            break;
        }
    }
    if (loc_SreverReturnData == ACCOUNT_NOT_FOUND)
    {
        accountRefrence = NULL;
    }
    return loc_SreverReturnData;
}
EN_serverError_t isBlockedAccount(ST_accountsDB_t *accountRefrence)
{
    EN_serverError_t loc_SreverReturnData = SERVER_OK;
    if (accountRefrence->state == BLOCKED)
    {
        loc_SreverReturnData = BLOCKED_ACCOUNT;
    }
    return loc_SreverReturnData;
}

EN_serverError_t isAmountAvailable(ST_terminalData_t *termData, ST_accountsDB_t *accountRefrence)
{
    EN_serverError_t loc_SreverReturnData = SERVER_OK;
    if(termData->transAmount > accountRefrence->balance)
    {
        loc_SreverReturnData = LOW_BALANCE;
    }
    return loc_SreverReturnData;
}
EN_serverError_t saveTransaction(ST_transaction_t *transData)
{
    EN_serverError_t loc_ServerReturnData = SERVER_OK;
   
    transData-> transactionSequenceNumber = Global_Sequence_Number;
    memcpy(&transactionsDB[Global_Sequence_Number++] , transData , sizeof(ST_transaction_t));
    
    return loc_ServerReturnData;
}

void listSavedTransactions(void)
{
    uint8_t loc_AccountsIterator = 0;
    char TransStates[5][30] = {"APPROVED","DECLINED_INSUFFECIENT_FUND", "DECLINED_STOLEN_CARD", "FRAUD_CARD", "INTERNAL_SERVER_ERROR"};
    EN_serverError_t transactionState= APPROVED;
    for(loc_AccountsIterator = 0; loc_AccountsIterator<Global_Sequence_Number; loc_AccountsIterator++ )
    {
        transactionState = accountsDB[loc_AccountsIterator].state;
        printf("#################################################################\n");
        printf("Transaction Sequence Number: %d\n", loc_AccountsIterator);
        printf("Transaction Date           : %s\n", transactionsDB[loc_AccountsIterator].terminalData.transactionDate);
        printf("Transaction Amount         : %f\n", transactionsDB[loc_AccountsIterator].terminalData.transAmount);
        printf("Transaction State          : %s\n", TransStates[transactionState]);
        printf("Terminal Max Amount        : %f\n", transactionsDB[loc_AccountsIterator].terminalData.maxTransAmount);
        printf("Card holder Name           : %s\n", transactionsDB[loc_AccountsIterator].cardHolderData.cardHolderName);
        printf("PAN                        : %s\n", transactionsDB[loc_AccountsIterator].cardHolderData.primaryAccountNumber);
        printf("Expiration Date            : %s\n", transactionsDB[loc_AccountsIterator].cardHolderData.cardExpirationDate);
        printf("#################################################################\n");
    }

}

void isBlockedAccountTest(void)
{
    uint8_t iter = 0;
    ST_accountsDB_t loc_tempData[3] = {
        {1000.0,BLOCKED, "55555555555555"},
        {1000.0,RUNNING, "4545454545454545"},
        {5000.0,BLOCKED, "4545454545454545" }
    };
    char output[7][25] = { "SERVER_OK", "SAVING_FAILED", "TRANSACTION_NOT_FOUND", "ACCOUNT_NOT_FOUND", "LOW_BALANCE", "BLOCKED_ACCOUNT" };
    char IP[2][10] = { "RUNNING", "BLOCKED" };
    EN_serverError_t return_Data = TERMINAL_OK;

    uint8_t Expected[3] = { BLOCKED_ACCOUNT ,SERVER_OK ,BLOCKED_ACCOUNT };
    for (iter = 0; iter < 3; iter++)
    {
        printf("========================================================\n");
        printf("Tester Name: Taqwa Hamed\n");
        printf("Function Name: isBlockedAccount\n");
        printf("Test Case : %d \n", iter + 1);
        printf("Input Data: Account state =  %s\n", IP[loc_tempData[iter].state]);
        return_Data = isBlockedAccount(&loc_tempData[iter]);
        printf("Expected Result: %s\n", output[Expected[iter]]);
        printf("Actual Result: %s\n", output[return_Data]);

    }
}

void recieveTransactionDataTest(void)
{

}
void isValidAccountTest(void)
{
    uint8_t iter = 0;
    ST_cardData_t loc_CtempData[3] = {
              {"xxxxxxxxxxxxxxxxxxxxx","8989374615436851", "12/22"},
              {"xxxxxxxxxxxxxxxxxxxxx", "5807007076043875", "07/24"},
              {"xxxxxxxxxxxxxxxxxxxxx", "4545454545454545", "10/29"}
    };
    char output[7][25] = { "SERVER_OK", "SAVING_FAILED", "TRANSACTION_NOT_FOUND", "ACCOUNT_NOT_FOUND", "LOW_BALANCE", "BLOCKED_ACCOUNT" };
    EN_serverError_t return_Data = SERVER_OK;

    uint8_t Expected[3] = { SERVER_OK ,SERVER_OK ,ACCOUNT_NOT_FOUND };
    for (iter = 0; iter < 3; iter++)
    {
        printf("========================================================\n");
        printf("Tester Name: Taqwa Hamed\n");
        printf("Function Name: isValidAccount\n");
        printf("Test Case : %d \n", iter + 1);
        printf("Input Data: PAN  =  %s\n", loc_CtempData[iter].primaryAccountNumber);
        return_Data = isValidAccount(&loc_CtempData[iter], &accountsDB);
        printf("Expected Result: %s\n", output[Expected[iter]]);
        printf("Actual Result: %s\n", output[return_Data]);

    }
}
void isAmountAvailableTest(void)
{
    uint8_t iter = 0;
    ST_terminalData_t loc_tempData[3] = {
             {1000.0, 50000.0 , "40/12/2022"},
             {10000.0, 50000.0 , "20/15/2022"},
             {500000.0, 0 , "20/11/2022"}
    };
    ST_accountsDB_t test[3] = { {40000.0},
                                {5000.0},
                                {500000.0}
    };
    char output[7][25] = { "SERVER_OK", "SAVING_FAILED", "TRANSACTION_NOT_FOUND", "ACCOUNT_NOT_FOUND", "LOW_BALANCE", "BLOCKED_ACCOUNT" };
    EN_serverError_t return_Data = SERVER_OK;

    EN_serverError_t Expected[3] = { SERVER_OK ,LOW_BALANCE ,SERVER_OK };
    for (iter = 0; iter < 3; iter++)
    {
        printf("========================================================\n");
        printf("Tester Name: Taqwa Hamed\n");
        printf("Function Name: isAmountAvailable\n");
        printf("Test Case : %d \n", iter + 1);
        printf("Input Data: trans amount  =  %f\t available amount = %f\n", loc_tempData[iter].transAmount, test[iter].balance);
        return_Data = isAmountAvailable(&loc_tempData[iter], &test[iter]);
        printf("Expected Result: %s\n", output[Expected[iter]]);
        printf("Actual Result: %s\n", output[return_Data]);

    }
}
void saveTransactionTest(void)
{
    uint8_t iter = 0;
    ST_cardData_t loc_CtempData[3] = {
                                    { "taqwa Hamed Goda sha7at","8989374615436851", "12/22" },
                                    { "manar Hamed Goda sha7at", "5807007076043875", "12/25"},
                                    { "mody Hamed Goda sha7at", "2499925799306994", "08/23"}
    };
    ST_terminalData_t loc_tempdata[3] = { 
                                            {1000.0, 50000.0 , "11/11/2022"},
                                            {25000.0, 50000.0 , "14/11/2022"},
                                            {13500.0, 50000.0 , "28/10/2022"}
    };
    ST_transaction_t test_data[3] = {
        {loc_CtempData[0], loc_tempdata[0], 0, 0},
        {loc_CtempData[1], loc_tempdata[1], 0, 0},
        {loc_CtempData[2], loc_tempdata[2], 0, 0},
    };


    char output[7][25] = { "SERVER_OK", "SAVING_FAILED", "TRANSACTION_NOT_FOUND", "ACCOUNT_NOT_FOUND", "LOW_BALANCE", "BLOCKED_ACCOUNT" };
    EN_serverError_t return_Data = SERVER_OK;

    EN_serverError_t Expected[3] = { SERVER_OK ,SERVER_OK ,SERVER_OK };
    for (iter = 0; iter < 3; iter++)
    {
        printf("========================================================\n");
        printf("Tester Name: Taqwa Hamed\n");
        printf("Function Name: saveTransaction\n");
        printf("Test Case : %d \n", iter + 1);
        printf("Input Data:\nCard Holder name = %s\tPAN=%s\tExpireDate=%s\n", loc_CtempData[iter].cardHolderName, loc_CtempData[iter].primaryAccountNumber, loc_CtempData[iter].cardExpirationDate);
        return_Data = saveTransaction(&test_data[iter]);
        printf("Expected Result: %s\n", output[Expected[iter]]);
        printf("Actual Result: %s\n", output[return_Data]);

    }



}
void listSavedTransactionsTest(void);