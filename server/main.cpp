/**************************************************************************
  Copyright (C), 

  File:  

  Author:  tony.chen
  
    Version: 1.0  
  
  Date:    2017
  
  Description:   

** History:
**Author (core ID)        Date          Number     Description of Changes

**----------------------------------------------------------- 仅供参考---------------------------------------------------------
**                    --           --
** 
** --------------------------------------------------------------------------------------------------------------------
**************************************************************************/
#define VERSION_INFO   2017-08/22-12:30

#include <stdio.h>
#include <unistd.h>
#include "netComm.h"
#include "stroeData.h"


//g++ main.cpp netComm.cpp stroeData.cpp -o robotQQServer -lpthread
int main()
{
	printf("hello this is tony begin !!   仅供参考 \r\n");
        while(1)
        {
            if( 0 == createTaskNetCommService() )
            {
                break;
            }
            sleep(3);
            continue;
        }

        createTaskStoreData();
	while(1)
	{
		printf("main loop... !!!\r\n");
		sleep(3);
	}
	printf("hello this is tony end !!!\r\n");
}


/********************************** (C) *****END OF FILE**************************/