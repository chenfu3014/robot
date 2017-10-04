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



#include "stdio.h"
#include "string.h"
#include <sys/select.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <netdb.h>
#include <errno.h>
#include <net/if.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>



void *thr_fn(void *arg)
{
	printf("new thread:");
	return ((void *)0);
}





/****************************************************************
 * @function name      : 
 * @brief              : 
  * @input param        : 
 * @output param       : 
 * @retval             : 
 * @author             : 
  * @date               : 
 * @others             :
 ***************************************************************/
int createTaskStoreData()
{
    //
    int ret;
    int errNum = 0;
    pthread_t writeXmlId;
    errNum = pthread_create(&writeXmlId, NULL, thr_fn, NULL);
    if (errNum != 0)
    {
        printf("createTaskSaveXMLToLocal failed: %s\n", strerror(errNum));
    }

    return ret; 
}


/********************************** (C) *****END OF FILE**************************/