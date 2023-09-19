#include<stdio.h>
#include <stdint.h>
#include<string.h>
#include"card.h"
#include"terminal.h"

EN_terminalError_t getTransactionDate(ST_terminalData_t *termData)
{
        EN_terminalError_t loc_EN_TerminalReturnData=TERMINAL_OK;
        uint8_t loc_str_length = 0;
        uint32_t local_day=0;
        uint32_t local_month=0;
        uint32_t local_year=0;
        printf(" Please, Enter  transaction Date :"); 
        gets(termData -> transactionDate);
        loc_str_length=strlen(termData-> transactionDate);   
        (void)sscanf(termData ->transactionDate, "%d/%d/%d",&local_day, &local_month, &local_year);
        if(loc_str_length<10 || loc_str_length==0) 
        {
                loc_EN_TerminalReturnData=WRONG_DATE;
        }
        if(local_month<0 || local_month>12 || local_year<0 || local_day>31 || local_day<0)
        {
                loc_EN_TerminalReturnData=WRONG_DATE;
        }
        return loc_EN_TerminalReturnData;
}


EN_terminalError_t isCardExpired(ST_cardData_t *cardData, ST_terminalData_t *termData)
{
        EN_terminalError_t loc_EN_TerminalReturnData=TERMINAL_OK;
        uint32_t local_month_transaction=0;
        uint32_t local_year_transaction =0;
        uint32_t local_month_expire=0;
        uint32_t local_year_expire=0;
        uint32_t local_day_transaction=0;
        (void)sscanf(termData ->transactionDate, "%d/%d/%d",&local_day_transaction, &local_month_transaction, &local_year_transaction);
        (void)sscanf(cardData->cardExpirationDate, "%d/%d", &local_month_expire, &local_year_expire);
        if((local_year_transaction%100)> local_year_expire )
        {
                loc_EN_TerminalReturnData =EXPIRED_CARD;
        } 
        else if(local_month_transaction>local_month_expire && (local_year_transaction % 100) == local_year_expire)
        {
                loc_EN_TerminalReturnData =EXPIRED_CARD;
        }
        return loc_EN_TerminalReturnData;
}


EN_terminalError_t getTransactionAmount(ST_terminalData_t *termData)
{
        EN_terminalError_t loc_EN_TerminalReturnData=TERMINAL_OK;
        printf("Please, Enter your Transaction amount :  ");
        (void)scanf("%f", &termData->transAmount);
        if(termData->transAmount <= 0)
        {
                loc_EN_TerminalReturnData = INVALID_AMOUNT;
        }
        return loc_EN_TerminalReturnData;
}
EN_terminalError_t isBelowMaxAmount(ST_terminalData_t *termData)
{
        EN_terminalError_t loc_EN_TerminalReturnData=TERMINAL_OK;
        if (termData->transAmount > termData->maxTransAmount)
        {
                loc_EN_TerminalReturnData = EXCEED_MAX_AMOUNT;
        }
        return loc_EN_TerminalReturnData;

}
EN_terminalError_t setMaxAmount(ST_terminalData_t *termData, float maxAmount)
{
        EN_terminalError_t loc_EN_TerminalReturnData=TERMINAL_OK;
       
        if(maxAmount <= 0)
        {
                loc_EN_TerminalReturnData = INVALID_MAX_AMOUNT;
        }
        else
        {
            termData->maxTransAmount = maxAmount;
        }
        return loc_EN_TerminalReturnData;
}
/*************************************************************************************/
void getTransactionDateTest(void)
{
    uint8_t iter = 0;
    char output[7][20] = { "TERMINAL_OK", "WRONG_DATE", "EXPIRED_CARD", "INVALID_CARD", "INVALID_AMOUNT", "EXCEED_MAX_AMOUNT", "INVALID_MAX_AMOUNT"};
    EN_terminalError_t return_Data = TERMINAL_OK;
    ST_terminalData_t loc_tempData[5] = {
            {1000.0, 50000.0 , "40/12/2022"},
            {0, 50000.0 , "20/15/2022"},
            {1000.0, 0 , "20/11/2022"},
            {-1.0, 50000.0 , "1/1/2022"},
            {1000.0, -1 , "20/07/2022"}
    };
    uint8_t Expected[5] = { WRONG_DATE ,WRONG_DATE ,TERMINAL_OK, WRONG_DATE, TERMINAL_OK };
    for (iter = 0; iter < 5; iter++)
    {
        printf("========================================================\n");
        printf("Tester Name: Taqwa Hamed\n");
        printf("Function Name: getTransactionDate\n");
        printf("Test Case : %d \n", iter + 1);
        printf("Input Data: %s\n", loc_tempData[iter].transactionDate);
        return_Data = getTransactionDate(&loc_tempData[iter]);
        printf("Expected Result: %s\n", output[Expected[iter]]);
        printf("Actual Result: %s\n", output[return_Data]);

    }

}
/******************************************************************/
void isCardExpiredTest(void)
{
    uint8_t iter = 0;
    char output[7][20] = { "TERMINAL_OK", "WRONG_DATE", "EXPIRED_CARD", "INVALID_CARD", "INVALID_AMOUNT", "EXCEED_MAX_AMOUNT", "INVALID_MAX_AMOUNT" };
    EN_terminalError_t return_Data = TERMINAL_OK;
    ST_terminalData_t loc_tempData[3] = {
            {1000.0, 50000.0 , "11/11/2022"},
            {0, 50000.0 , "10/08/2024"},
            {1000.0, 0 , "24/09/2030"}
    };
    ST_cardData_t loc_CtempData[3] = {
               {"xxxxxxxxxxxxxxxxxxxxx","555555555555555", "12/22"},
               {"xxxxxxxxxxxxxxxxxxxxx", "555555555555555", "07/24"},
               {"xxxxxxxxxxxxxxxxxxxxx", "555555555555555", "10/29"}
    };
    uint8_t Expected[3] = { TERMINAL_OK ,EXPIRED_CARD ,EXPIRED_CARD };
    for (iter = 0; iter < 3; iter++)
    {
        printf("========================================================\n");
        printf("Tester Name: Taqwa Hamed\n");
        printf("Function Name: isCardExpired\n");
        printf("Test Case : %d \n", iter + 1);
        printf("Input Data: Transaction Date = %s\tExpire Date = %s\n", loc_tempData[iter].transactionDate, loc_CtempData[iter].cardExpirationDate);
        return_Data = isCardExpired(&loc_CtempData[iter],&loc_tempData[iter]);
        printf("Expected Result: %s\n", output[Expected[iter]]);
        printf("Actual Result: %s\n", output[return_Data]);

    }

}


void getTransactionAmountTest(void)
{
    uint8_t iter = 0;
    char output[7][20] = { "TERMINAL_OK", "WRONG_DATE", "EXPIRED_CARD", "INVALID_CARD", "INVALID_AMOUNT", "EXCEED_MAX_AMOUNT", "INVALID_MAX_AMOUNT" };
    EN_terminalError_t return_Data = TERMINAL_OK;
    ST_terminalData_t loc_tempData[5] = {
            {1000.0, 50000.0 , "40/12/2022"},
            {0, 50000.0 , "20/15/2022"},
            {1000.0, 0 , "20/11/2022"},
            {-1.0, 50000.0 , "1/1/2022"},
            {1000.0, -1 , "20/07/2022"}
    };
    uint8_t Expected[5] = { TERMINAL_OK ,INVALID_AMOUNT ,TERMINAL_OK, INVALID_AMOUNT, TERMINAL_OK };
    for (iter = 0; iter < 5; iter++)
    {
        printf("========================================================\n");
        printf("Tester Name: Taqwa Hamed\n");
        printf("Function Name: getTransactionAmount\n");
        printf("Test Case : %d \n", iter + 1);
        printf("Input Data: %f\n", loc_tempData[iter].transAmount);
        return_Data = getTransactionAmount(&loc_tempData[iter]);
        printf("Expected Result: %s\n", output[Expected[iter]]);
        printf("Actual Result: %s\n", output[return_Data]);
    }

}
void isBelowMaxAmountTest(void)
{
    uint8_t iter = 0;
    char output[7][20] = { "TERMINAL_OK", "WRONG_DATE", "EXPIRED_CARD", "INVALID_CARD", "INVALID_AMOUNT", "EXCEED_MAX_AMOUNT", "INVALID_MAX_AMOUNT" };
    EN_terminalError_t return_Data = TERMINAL_OK;
    ST_terminalData_t loc_tempData[5] = {
            {1000.0, 50000.0 , "40/12/2022"},
            {10.0, 50000.0 , "20/15/2022"},
            {1000.0, 50.0 , "20/11/2022"},
            {1.0, 50000.0 , "1/1/2022"},
            {1000.0, 900.0 , "20/07/2022"}
    };
    uint8_t Expected[5] = { TERMINAL_OK ,TERMINAL_OK ,EXCEED_MAX_AMOUNT, TERMINAL_OK, EXCEED_MAX_AMOUNT };
    for (iter = 0; iter < 5; iter++)
    {
        printf("========================================================\n");
        printf("Tester Name: Taqwa Hamed\n");
        printf("Function Name: isBelowMaxAmount\n");
        printf("Test Case : %d \n", iter + 1);
        printf("Input Data: transaction amount = %f\t max amount  = %f\n", loc_tempData[iter].transAmount, loc_tempData[iter].maxTransAmount);
        return_Data = isBelowMaxAmount(&loc_tempData[iter]);
        printf("Expected Result: %s\n", output[Expected[iter]]);
        printf("Actual Result: %s\n", output[return_Data]);
    }

}
void setMaxAmountTest(void)
{
    uint8_t iter = 0;
    char output[7][20] = { "TERMINAL_OK", "WRONG_DATE", "EXPIRED_CARD", "INVALID_CARD", "INVALID_AMOUNT", "EXCEED_MAX_AMOUNT", "INVALID_MAX_AMOUNT" };
    EN_terminalError_t return_Data = TERMINAL_OK;
    ST_terminalData_t loc_tempData[5] = {
            {1000.0, 0 , "40/12/2022"},
            {0, 0 , "20/15/2022"},
            {1000.0, 0 , "20/11/2022"},
            {-1.0, 50000.0 , "1/1/2022"},
            {1000.0, -1 , "20/07/2022"}
    };
    float maxamount[5] = { 50000.0  , 50000.0 ,0.0,50000.0,-1.0 };
    uint8_t Expected[5] = { TERMINAL_OK ,TERMINAL_OK ,INVALID_MAX_AMOUNT, TERMINAL_OK, INVALID_MAX_AMOUNT };
    for (iter = 0; iter < 5; iter++)
    {
        printf("========================================================\n");
        printf("Tester Name: Taqwa Hamed\n");
        printf("Function Name: setMaxAmount\n");
        printf("Test Case : %d \n", iter + 1);
        printf("Input Data: %f\n", maxamount[iter]);
        return_Data = setMaxAmount(&loc_tempData[iter], maxamount[iter]);
        printf("Expected Result: %s\n", output[Expected[iter]]);
        printf("Actual Result: %s\n", output[return_Data]);
    }
}

EN_terminalError_t isValidCardPAN(ST_cardData_t *cardData); 