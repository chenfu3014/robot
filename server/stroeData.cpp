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

#define MAX_READ_BUF            10240
#define MAX_PHP_STORE_DATA      20480


char gRecvBufStoreData[MAX_READ_BUF] = {0};

char gPHPWriteData[MAX_PHP_STORE_DATA] = {0};
char gPHPDataHead[128] = {0};
char gPHPDataTrail[32] = {0};
char gPHPDataBody[MAX_PHP_STORE_DATA] = {0};


int gIsRecvDataFlag = 0;

char gBufStart[] = {"六码两期"};
char gBufStop[] = {"对错勿怪"};
char gBufBreakLine[] = {"\r\n"};





/****************************************************************
* @function name	  : 
* @brief			  : 
 * @input param 	   : 
* @output param 	  : 
* @retval			  : 
* @author			  : 
 * @date			   : 
* @others			  :
***************************************************************/
void combineThePHPHead()
{
    sprintf(gPHPDataHead, "%s", "<!DOCTYPE html>\r\n\
<html lang=\"en\">\r\n\
<head>\r\n\
    <meta charset=\"UTF-8\">\r\n\
    <title>Title</title>\r\n\
</head>\r\n\
<body>\r\n\
<?php\r\n");


    printf("%s", gPHPDataHead);
}




/****************************************************************
* @function name	  : 
* @brief			  : 
 * @input param 	   : 
* @output param 	  : 
* @retval			  : 
* @author			  : 
 * @date			   : 
* @others			  :
***************************************************************/
void combineThePHPTrail()
{
        sprintf(gPHPDataTrail, "%s", "?>\r\n\
</body>\r\n\
</html>\r\n");
    
    
        printf("%s", gPHPDataTrail);

}





void combineThePHPAllData()
{
    printf("combineThePHPAllData enter.\r\n");
    combineThePHPHead();
    strcat(gPHPWriteData, gPHPDataHead);

    // 
    strcat(gPHPWriteData, gPHPDataBody);

    combineThePHPTrail();
    strcat(gPHPWriteData, gPHPDataTrail);

    printf("\r\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\r\n");
    printf("\r\n%s\r\n", gPHPWriteData);
    printf("\r\n<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\r\n");
}



/****************************************************************
* @function name	  : 
* @brief			  : 
 * @input param 	   : 
* @output param 	  : 
* @retval			  : 
* @author			  : 
 * @date			   : 
* @others			  :
***************************************************************/
int formatEachLineDataToBuf( const char *eachLineBuf, int eachLineLen )
{
    printf("formatEachLineDataToBuf enter.\r\n");
    char eachPHPLine[64] = {0};   //   echo '125-126期 3-8名【04】 挂(2)<br>';
    char eachDataLine[64] = {0};   //  125-126期 3-8名【04】 挂(2)
    memcpy( eachDataLine, eachLineBuf, eachLineLen );
	printf("eachDataLine(%d)==>>\r\n%s\r\n<<==\r\n", eachLineLen, eachDataLine);


    // combine body data.
    //  echo '六码两期<br>';
    sprintf(eachPHPLine, "echo '%s<br>';",eachDataLine );

    int phpLen = (int)strlen(eachPHPLine);
    printf("eachPHPLine(%d)==>>\r\n%s\r\n<<==\r\n", phpLen, eachPHPLine);
    strcat(gPHPDataBody, eachPHPLine);
    strcat(gPHPDataBody, "\r\n");


    printf("formatEachLineDataToBuf exit.\r\n");
    return 0;
}














/****************************************************************
* @function name	  : 
* @brief			  : 
 * @input param 	   : 
* @output param 	  : 
* @retval			  : 
* @author			  : 
 * @date			   : 
* @others			  :
#if 0
					<!DOCTYPE html>
					<html lang="en">
					<head>
					    <meta charset="UTF-8">
					    <title>Title</title>
					</head>
					<body>
					<?php
						echo '六码两期<br>';
						echo '------------------------------<br>';
						echo '125-126期 3-8名【04】 挂(2)<br>';
						echo '127-128期 3-8名【07】 中(1)<br>';
						echo '128-129期 3-8名【01】 中(1)<br>';
						echo '129-130期 3-8名【02】 中(1)<br>';
						echo '130-131期 3-8名【02】 中(2)<br>';
						echo '132-133期 3-8名【09】 中(1)<br>';
						echo '133-134期 3-8名【02】 中(2)<br>';
						echo '135-136期 3-8名【03】 中(1)<br>';
						echo '136-137期 3-8名【06】 中(2)<br>';
						echo '138-139期 3-8名【05】 等开(1)<br>';
						echo '------------------------------<br>';
						echo '仅供参考   对错勿怪<br>';
					?>
					</body>
					</html>
#endif












* @function name	  : 
* @brief			  : 
 * @input param 	   : 
* @output param 	  : 
* @retval			  : 
* @author			  : 
 * @date			   : 
* @others			  :
#if 0
					<!DOCTYPE html>
					<html lang="en">
					<head>
					    <meta charset="UTF-8">
					    <title>Title</title>
					</head>
					<body>
					<?php
						echo '六码两期<br>
                              ------------------------------<br>
                              127-128期 3-8名【07】 中(1)<br>
                              128-129期 3-8名【01】 中(1)<br>
                              129-130期 3-8名【02】 中(1)<br>
                              130-131期 3-8名【02】 中(2)<br>
                              132-133期 3-8名【09】 中(1)<br>
                              133-134期 3-8名【02】 中(2)<br>
                              135-136期 3-8名【03】 中(1)<br>
                              136-137期 3-8名【06】 中(2)<br>
                              138-139期 3-8名【05】 中(1)<br>
                              139-140期 3-8名【09】 等开(2)<br>
                              ------------------------------<br>
                              仅供参考   对错勿怪<br>';
					?>
					</body>
					</html>
#endif

***************************************************************/
int formatEachRecordDataToBuf( const char *data, const int dataLen )
{
    if( NULL == data || 0 == dataLen )
    {
        return -1;
    }
    char eachRecordBuf[1024] = {"0"};
    memcpy( eachRecordBuf, data, dataLen );
	printf("eachRecordData(%d)+++++++++>>>\r\n%s\r\n<<<+++++++++\r\n", dataLen, eachRecordBuf);

    char *pStartPoint = NULL;
    char *pEndPoint = NULL;
    char *pCurPoint = (char *)eachRecordBuf;
    
    while( pCurPoint - eachRecordBuf <= dataLen)
    {
        pStartPoint = pCurPoint;

        pEndPoint = strstr( pCurPoint, gBufBreakLine );   // 指向   \r\n  中的  \r  .....
        if( NULL == pEndPoint )
        {
            return -1;
        }

        pCurPoint = pEndPoint + 2;    // 指向   \r\n  的下个字符  .....

        int eachLineDataLen = pEndPoint-pStartPoint;
        formatEachLineDataToBuf(pStartPoint,eachLineDataLen);  // data without \r\n
    }

    printf("formatEachRecordDataToBuf exit.\r\n");
	return 0;
}











/****************************************************************
* @function name	  : 
* @brief			  : 
 * @input param 	   : 
* @output param 	  : 
* @retval			  : 
* @author			  : 
 * @date			   : 
* @others			  :
						2017-09-27 16:46:41 正在连接服务器：123.151.77.183
						2017-09-27 16:46:46 成功连接到服务器
						2017-09-27 16:46:46 下载QQ列表
						2017-09-27 16:46:46 登陆成功!
						2017-09-27 16:46:47 协议版本：QQ_8_9
						2017-09-27 16:46:55 发送队列启动完成
						2017-09-27 16:50:45 拖泥(2353617310) 说：    六码两期
						六码两期
						------------------------------
						125-126期 3-8名【04】 挂(2)
						127-128期 3-8名【07】 中(1)
						128-129期 3-8名【01】 中(1)
						129-130期 3-8名【02】 中(1)
						130-131期 3-8名【02】 中(2)
						132-133期 3-8名【09】 中(1)
						133-134期 3-8名【02】 中(2)
						135-136期 3-8名【03】 中(1)
						136-137期 3-8名【06】 中(2)
						138-139期 3-8名【05】 等开(1)
						------------------------------
						仅供参考   对错勿怪
						六码两期
						------------------------------
						125-126期 3-8名【04】 挂(2)
						127-128期 3-8名【07】 中(1)
						128-129期 3-8名【01】 中(1)
						129-130期 3-8名【02】 中(1)
						130-131期 3-8名【02】 中(2)
						132-133期 3-8名【09】 中(1)
						133-134期 3-8名【02】 中(2)
						135-136期 3-8名【03】 中(1)
						136-137期 3-8名【06】 中(2)
						138-139期 3-8名【05】 等开(1)
						------------------------------
						仅供参考   对错勿怪
***************************************************************/

char temp_test[MAX_READ_BUF] = {" 2017-09-27 16:46:41 正在连接服务器：123.151.77.183\r\n\
2017-09-27 16:46:46 成功连接到服务器\r\n\
2017-09-27 16:46:46 下载QQ列表\r\n\
2017-09-27 16:46:46 登陆成功!\r\n\
2017-09-27 16:46:47 协议版本：QQ_8_9\r\n\
2017-09-27 16:46:55 发送队列启动完成\r\n\
2017-09-27 16:50:45 拖泥(2353617310) 说：    六码两期\r\n\
------------------------------\r\n\
125-126期 3-8名【04】 挂(2)\r\n\
127-128期 3-8名【07】 中(1)\r\n\
128-129期 3-8名【01】 中(1)\r\n\
129-130期 3-8名【02】 中(1)\r\n\
130-131期 3-8名【02】 中(2)\r\n\
132-133期 3-8名【09】 中(1)\r\n\
133-134期 3-8名【02】 中(2)\r\n\
135-136期 3-8名【03】 中(1)\r\n\
136-137期 3-8名【06】 中(2)\r\n\
138-139期 3-8名【05】 等开(1)\r\n\
------------------------------\r\n\
仅供参考   对错勿怪\r\n\
六码两期\r\n\
------------------------------\r\n\
140-141期 3-8名【04】 挂(2)\r\n\
142-143期 3-8名【07】 中(1)\r\n\
144-145期 3-8名【01】 中(1)\r\n\
146-147期 3-8名【02】 中(1)\r\n\
148-149期 3-8名【02】 中(2)\r\n\
150-151期 3-8名【09】 中(1)\r\n\
152-153期 3-8名【02】 中(2)\r\n\
154-155期 3-8名【03】 中(1)\r\n\
156-157期 3-8名【06】 中(2)\r\n\
158-159期 3-8名【05】 等开(1)\r\n\
------------------------------\r\n\
仅供参考   对错勿怪\r\n"};
int splitSegementData( /*const*/ char *dataBuf, /*const*/ int dataLen )
{
    
    // for test
	dataBuf = temp_test;	// for test
	dataLen = (int)strlen(dataBuf);
    // for test

    printf("splitSegementData enter.\r\n");
	if( NULL == dataBuf || 0 == dataLen )
	{	
		return -1;
	}
	char *pStartPoint = NULL;  // 指向   六码两期    的开始
	char *pEndPoint = NULL;    // 指向    对错勿怪    的末尾 
	char *pCurPoint = (char *)dataBuf;
    
	while( pCurPoint-dataBuf < dataLen )
	{	    
	    printf("splitSegementData %d:%d.\r\n", pCurPoint-dataBuf, dataLen);
		pStartPoint = strstr(pCurPoint, gBufStart);
		if( NULL == pStartPoint )
		{
			break;
		}

		pEndPoint = strstr(pCurPoint, gBufStop);
		if( NULL == pEndPoint )
		{
			break;
		}	

        int eachDataLen = (pEndPoint + 10) - pStartPoint;           // modifiy from 8 to 10 bls include \r\n
		formatEachRecordDataToBuf( pStartPoint, eachDataLen );
        pCurPoint = pEndPoint;
        pCurPoint = pCurPoint + 8;
	}
	
	//
    combineThePHPAllData();
    
	printf("splitSegementData exit.\r\n");
	return 0; // success.
}
















/****************************************************************
* @function name	  : 
* @brief			  : 
 * @input param 	   : 
* @output param 	  : 
* @retval			  : 
* @author			  : 
 * @date			   : 
* @others			  :
***************************************************************/
void *stroeDataFunProc(void *arg)
{
	printf("stroeDataFunProc enter.\r\n");
	while(1)
	{
		if( 0 == gIsRecvDataFlag )
		{
			
            printf("stroeDataFunProc no data.\r\n");
			sleep(2);
                        continue;
		}
		else
		{
            printf("stroeDataFunProc get data!!!\r\n");
            printf("==>gRecvBufStoreData(%d):\r\n%s. \r\n", (int)strlen(gRecvBufStoreData),  gRecvBufStoreData );
			splitSegementData(gRecvBufStoreData, (int)strlen(gRecvBufStoreData));	
					
		}
		//clean gRecvBufStoreData
		memset( gRecvBufStoreData, 0, MAX_READ_BUF );
        
		sleep(3);
	}
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
    // for test.
    combineThePHPHead();
    combineThePHPTrail();
    splitSegementData( (char *)"abc", strlen("abc"));
    //
    
    int ret;
    int errNum = 0;
    pthread_t writeXmlId;    
    errNum = pthread_create(&writeXmlId, NULL, stroeDataFunProc, NULL);
    if (errNum != 0)
    {
        printf("createTaskSaveXMLToLocal failed: %s\n", strerror(errNum));
    }
    else
    {
        printf("createTaskSaveXMLToLocal successfully: %s\n", strerror(errNum));
    }
    return ret; 
}


/********************************** (C) *****END OF FILE**************************/
