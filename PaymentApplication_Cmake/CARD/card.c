#include<stdio.h>
#include <stdint.h>
#include<string.h>
#include"card.h"
EN_cardError_t getCardHolderName(ST_cardData_t *cardData)
{
  EN_cardError_t loc_EN_CardReturnData=CARD_OK; 
	uint8_t loc_str_length=0;
	printf(" Please, Enter Your Name : ");
	gets(cardData -> cardHolderName);
      
	loc_str_length=strlen(cardData -> cardHolderName);
      
    if(loc_str_length<20 || loc_str_length>24 || loc_str_length==0)	
	{
                loc_EN_CardReturnData=WRONG_NAME;
	}
    
     return loc_EN_CardReturnData;
}

EN_cardError_t getCardExpiryDate(ST_cardData_t *cardData)
{
        EN_cardError_t loc_EN_CardReturnData=CARD_OK; 
        uint32_t loc_str_length=0;
        uint32_t local_year=0;
        uint32_t local_month=0;

        printf(" Please, Enter EXPIRE DATE :\n format: MM/YY : ");
        gets(cardData -> cardExpirationDate);

        loc_str_length=strlen(cardData -> cardExpirationDate);
	 
        (void)sscanf(cardData->cardExpirationDate, "%d/%d", &local_month, &local_year);
        if(loc_str_length<5 || loc_str_length>5 || loc_str_length==0)
	{
                loc_EN_CardReturnData=WRONG_EXP_DATE;
	}
        if(local_month<0 || local_month>12 || local_year<0)
        {

                loc_EN_CardReturnData=WRONG_EXP_DATE;

        }
     return loc_EN_CardReturnData;
}



EN_cardError_t getCardPAN(ST_cardData_t *cardData)
{
        EN_cardError_t loc_EN_CardReturnData=CARD_OK; 
        uint8_t loc_str_length=0;
        printf(" Please, Enter Card PAN :");
        gets(cardData -> primaryAccountNumber);
        loc_str_length=strlen(cardData -> primaryAccountNumber);
        if(loc_str_length<16 || loc_str_length>19 || loc_str_length==0)       
        {
                loc_EN_CardReturnData=WRONG_PAN ;
        }
        return loc_EN_CardReturnData;
}



void getCardHolderNameTest(void)
{
        uint8_t iter = 0;
        char output[4][15] ={ "CARD_OK", "WRONG_NAME", "WRONG_EXP_DATE", "WRONG_PAN"};
        EN_cardError_t return_Data = CARD_OK;
        ST_cardData_t loc_tempData[4] = {
                {""},
                {"Taqwa Hamed Goda valid"},
                {"Taqwa Hamed Goda Wrongggg"},
                {"taqwa"}
        };
        uint8_t Expected[4] = { WRONG_NAME ,CARD_OK ,WRONG_NAME, WRONG_NAME };
        for (iter = 0; iter <4; iter++)
        {
                printf("========================================================\n");
                printf("Tester Name: Taqwa Hamed\n");
                printf("Function Name: getCardHolderName\n");
                printf("Test Case : %d \n", iter+1);
                printf("Input Data: %s\n",loc_tempData[iter].cardHolderName );
                return_Data = getCardHolderName(&loc_tempData[iter]);
                printf("Expected Result: %s\n", output[Expected[iter]]);
                printf("Actual Result: %s\n", output[return_Data]);
               
        }
}
void getCardExpiryDateTest(void)
{
        uint8_t iter = 0;
        char output[4][15] ={ "CARD_OK", "WRONG_NAME", "WRONG_EXP_DATE", "WRONG_PAN"};
        EN_cardError_t return_Data = CARD_OK;
        uint8_t Expected[4] = { WRONG_EXP_DATE ,WRONG_EXP_DATE ,CARD_OK, WRONG_EXP_DATE };
        ST_cardData_t loc_tempData[4] = {
                {"xxxxxxxxxxxxxxxxxxxxx","555555555555555", "14/22"},
                {"xxxxxxxxxxxxxxxxxxxxx", "555555555555555", "09/1"},
                {"xxxxxxxxxxxxxxxxxxxxx", "555555555555555", "09/22",},
                {"xxxxxxxxxxxxxxxxxxxxx", "5555555555555555", "09/0",}
        };
        for (iter = 0; iter <4; iter++)
        {
                printf("========================================================\n");
                printf("Tester Name: Taqwa Hamed\n");
                printf("Function Name: getCardExpiryDateTest\n");
                printf("Test Case : %d \n", iter+1);
                printf("Input Data: %s\n",loc_tempData[iter].cardExpirationDate );
                return_Data = getCardExpiryDate(&loc_tempData[iter]);
                printf("Expected Result: %s\n", output[Expected[iter]]);
                printf("Actual Result: %s\n", output[return_Data]);
            
        }
}
void getCardPANTest(void)
{
        uint8_t iter = 0;
        char output[4][15] ={ "CARD_OK", "WRONG_NAME", "WRONG_EXP_DATE", "WRONG_PAN"};
        EN_cardError_t return_Data = CARD_OK;
        uint8_t Expected[4] = { WRONG_PAN ,WRONG_PAN ,CARD_OK, WRONG_PAN };

        ST_cardData_t loc_tempData[4] = {
                {"xxxxxxxxxxxxxxxxxxxxx","555555", "14/22"},
                {"xxxxxxxxxxxxxxxxxxxxx", "55555555556555555565", "09/1"},
                {"xxxxxxxxxxxxxxxxxxxxx", "1234567891012131516", "09/22",},
                {"xxxxxxxxxxxxxxxxxxxxx", "12345678912345678911", "09/0",}
        };
        for (iter = 0; iter <4; iter++)
        {
                printf("========================================================\n");
                printf("Tester Name: Taqwa Hamed\n");
                printf("Function Name: getCardPANTest\n");
                printf("Test Case : %d\n", iter+1);
                printf("Input Data: %s\n",loc_tempData[iter].primaryAccountNumber );
                return_Data = getCardPAN(&loc_tempData[iter]);
                printf("Expected Result: %s\n", output[Expected[iter]]);
                printf("Actual Result: %s\n", output[return_Data]);

        }
}


