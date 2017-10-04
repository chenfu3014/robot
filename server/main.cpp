/**************************************************************************
  Copyright (C), 

  File:  

  Author:  tony.chen
  
    Version: 1.0  
  
  Date:    2017
  
  Description:   

** History:
**Author (core ID)        Date          Number     Description of Changes

**--------------------------------------------------------------------------------------------------------------------
**                    --           --
** 
** --------------------------------------------------------------------------------------------------------------------
**************************************************************************/
#define VERSION_INFO   2017-08/22-12:30

#include <stdio.h>
#include "netComm.h"
#include "stroeData.h"


// g++ main.cpp hello.cpp netComm.cpp stroeData.cpp -o robotQQServer
int main()
{
	printf("hello this is tony begin !!\r\n");
    createTaskNetCommService();
    createTaskStoreData();
	printf("hello this is tony end !!!\r\n");
}


/********************************** (C) *****END OF FILE**************************/