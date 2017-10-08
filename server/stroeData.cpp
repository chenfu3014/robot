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

char gBufStart[] = {"��������"};
char gBufStop[] = {"�Դ����"};
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
    char eachPHPLine[64] = {0};   //   echo '125-126�� 3-8����04�� ��(2)<br>';
    char eachDataLine[64] = {0};   //  125-126�� 3-8����04�� ��(2)
    memcpy( eachDataLine, eachLineBuf, eachLineLen );
	printf("eachDataLine(%d)==>>\r\n%s\r\n<<==\r\n", eachLineLen, eachDataLine);


    // combine body data.
    //  echo '��������<br>';
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
						echo '��������<br>';
						echo '------------------------------<br>';
						echo '125-126�� 3-8����04�� ��(2)<br>';
						echo '127-128�� 3-8����07�� ��(1)<br>';
						echo '128-129�� 3-8����01�� ��(1)<br>';
						echo '129-130�� 3-8����02�� ��(1)<br>';
						echo '130-131�� 3-8����02�� ��(2)<br>';
						echo '132-133�� 3-8����09�� ��(1)<br>';
						echo '133-134�� 3-8����02�� ��(2)<br>';
						echo '135-136�� 3-8����03�� ��(1)<br>';
						echo '136-137�� 3-8����06�� ��(2)<br>';
						echo '138-139�� 3-8����05�� �ȿ�(1)<br>';
						echo '------------------------------<br>';
						echo '�����ο�   �Դ����<br>';
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
						echo '��������<br>
                              ------------------------------<br>
                              127-128�� 3-8����07�� ��(1)<br>
                              128-129�� 3-8����01�� ��(1)<br>
                              129-130�� 3-8����02�� ��(1)<br>
                              130-131�� 3-8����02�� ��(2)<br>
                              132-133�� 3-8����09�� ��(1)<br>
                              133-134�� 3-8����02�� ��(2)<br>
                              135-136�� 3-8����03�� ��(1)<br>
                              136-137�� 3-8����06�� ��(2)<br>
                              138-139�� 3-8����05�� ��(1)<br>
                              139-140�� 3-8����09�� �ȿ�(2)<br>
                              ------------------------------<br>
                              �����ο�   �Դ����<br>';
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

        pEndPoint = strstr( pCurPoint, gBufBreakLine );   // ָ��   \r\n  �е�  \r  .....
        if( NULL == pEndPoint )
        {
            return -1;
        }

        pCurPoint = pEndPoint + 2;    // ָ��   \r\n  ���¸��ַ�  .....

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
						2017-09-27 16:46:41 �������ӷ�������123.151.77.183
						2017-09-27 16:46:46 �ɹ����ӵ�������
						2017-09-27 16:46:46 ����QQ�б�
						2017-09-27 16:46:46 ��½�ɹ�!
						2017-09-27 16:46:47 Э��汾��QQ_8_9
						2017-09-27 16:46:55 ���Ͷ����������
						2017-09-27 16:50:45 ����(2353617310) ˵��    ��������
						��������
						------------------------------
						125-126�� 3-8����04�� ��(2)
						127-128�� 3-8����07�� ��(1)
						128-129�� 3-8����01�� ��(1)
						129-130�� 3-8����02�� ��(1)
						130-131�� 3-8����02�� ��(2)
						132-133�� 3-8����09�� ��(1)
						133-134�� 3-8����02�� ��(2)
						135-136�� 3-8����03�� ��(1)
						136-137�� 3-8����06�� ��(2)
						138-139�� 3-8����05�� �ȿ�(1)
						------------------------------
						�����ο�   �Դ����
						��������
						------------------------------
						125-126�� 3-8����04�� ��(2)
						127-128�� 3-8����07�� ��(1)
						128-129�� 3-8����01�� ��(1)
						129-130�� 3-8����02�� ��(1)
						130-131�� 3-8����02�� ��(2)
						132-133�� 3-8����09�� ��(1)
						133-134�� 3-8����02�� ��(2)
						135-136�� 3-8����03�� ��(1)
						136-137�� 3-8����06�� ��(2)
						138-139�� 3-8����05�� �ȿ�(1)
						------------------------------
						�����ο�   �Դ����
***************************************************************/

char temp_test[MAX_READ_BUF] = {" 2017-09-27 16:46:41 �������ӷ�������123.151.77.183\r\n\
2017-09-27 16:46:46 �ɹ����ӵ�������\r\n\
2017-09-27 16:46:46 ����QQ�б�\r\n\
2017-09-27 16:46:46 ��½�ɹ�!\r\n\
2017-09-27 16:46:47 Э��汾��QQ_8_9\r\n\
2017-09-27 16:46:55 ���Ͷ����������\r\n\
2017-09-27 16:50:45 ����(2353617310) ˵��    ��������\r\n\
------------------------------\r\n\
125-126�� 3-8����04�� ��(2)\r\n\
127-128�� 3-8����07�� ��(1)\r\n\
128-129�� 3-8����01�� ��(1)\r\n\
129-130�� 3-8����02�� ��(1)\r\n\
130-131�� 3-8����02�� ��(2)\r\n\
132-133�� 3-8����09�� ��(1)\r\n\
133-134�� 3-8����02�� ��(2)\r\n\
135-136�� 3-8����03�� ��(1)\r\n\
136-137�� 3-8����06�� ��(2)\r\n\
138-139�� 3-8����05�� �ȿ�(1)\r\n\
------------------------------\r\n\
�����ο�   �Դ����\r\n\
��������\r\n\
------------------------------\r\n\
140-141�� 3-8����04�� ��(2)\r\n\
142-143�� 3-8����07�� ��(1)\r\n\
144-145�� 3-8����01�� ��(1)\r\n\
146-147�� 3-8����02�� ��(1)\r\n\
148-149�� 3-8����02�� ��(2)\r\n\
150-151�� 3-8����09�� ��(1)\r\n\
152-153�� 3-8����02�� ��(2)\r\n\
154-155�� 3-8����03�� ��(1)\r\n\
156-157�� 3-8����06�� ��(2)\r\n\
158-159�� 3-8����05�� �ȿ�(1)\r\n\
------------------------------\r\n\
�����ο�   �Դ����\r\n"};
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
	char *pStartPoint = NULL;  // ָ��   ��������    �Ŀ�ʼ
	char *pEndPoint = NULL;    // ָ��    �Դ����    ��ĩβ 
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
